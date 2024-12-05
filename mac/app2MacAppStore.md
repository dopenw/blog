
<!-- Mac App Store上架与内购集成实操经验分享 -->
  ---

  <!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

  <!-- code_chunk_output -->

  - [1. 简介](#1-简介)
  - [2. 基本流程](#2-基本流程)
    - [2.1 支持 In-App Purchase (内购)](#21-支持-in-app-purchase-内购)
      - [2.1.1 App Store Connect 上配置内购](#211-app-store-connect-上配置内购)
      - [2.1.2 ActiveMgr（内购模块）源代码](#212-activemgr内购模块源代码)
        - [2.1.2.1 如何调用ActiveMgr购买、恢复购买，获取价格信息？](#2121-如何调用activemgr购买-恢复购买获取价格信息)
    - [2.2 需要多久可以上架呢？](#22-需要多久可以上架呢)
  - [3. 遇到的一些问题](#3-遇到的一些问题)
    - [3.1 校验订单，但校验结果一直返回 21003](#31-校验订单但校验结果一直返回-21003)
    - [3.2 Could not find the Qt platform plugin "cocoa" in ""](#32-could-not-find-the-qt-platform-plugin-cocoa-in-)
    - [3.3 使用真实账号登陆沙盒测试账号购买异常](#33-使用真实账号登陆沙盒测试账号购买异常)
    - [3.4 偶现程序已启动就进入到  paymentQueue](#34-偶现程序已启动就进入到--paymentqueue)
    - [3.5 修复 “appstore 版本开启沙盒”后无法让多进程通信](#35-修复-appstore-版本开启沙盒后无法让多进程通信)
  - [4. 参考链接](#4-参考链接)

  <!-- /code_chunk_output -->
  ---

  # 1. 简介

  本文档旨在为开发人员提供关于如何将应用程序上架到App Store并集成内购的详细指导。通过阅读本指南，开发人员将能够快速掌握上架流程，扩展产品的收入渠道，增加产品的收入。

  # 2. 基本流程

  1. 在 App Store Connect 后台创建并填写待上架的 app 必要信息 
  2. 配置并进行签名操作（必须启用Apple的应用沙箱）
  3. 上传打包好的应用程序文件
  4. 提交审核 

  详情可参考 [ Electron - Mac App Store 应用程序提交指南](https://www.electronjs.org/zh/docs/latest/tutorial/mac-app-store-submission-guide)

  ## 2.1 支持 In-App Purchase (内购) 

  ### 2.1.1 App Store Connect 上配置内购

  可参考 [苹果内购（IAP）从入门到精通（1）-内购商品类型与配置 ](https://juejin.cn/post/7046969127205863438)

  ### 2.1.2 ActiveMgr（内购模块）源代码

  以下代码使用 Qt + Objective-c 开发。如果使用其他技术方案需要进行一些适配。

  IActiveMgr.h:

  ```c++
  #ifndef IACTIVEMGR_H
  #define IACTIVEMGR_H
  
  #include <boost/noncopyable.hpp>
  
  #include <QObject>
  #include <atomic>
  #include <thread>
  
  class IActiveMgr : public QObject, private boost::noncopyable {
      Q_OBJECT
  
  public:
      explicit IActiveMgr( QObject* parent = nullptr );
      virtual ~IActiveMgr();
      static IActiveMgr* create( QObject* parent );
      QString getActiveTypeStr() const;
      Q_INVOKABLE void setActiveTypeStr( const QString& activeType );
  
  signals:
      void activeFinish( qint64 result );
      void startActive();
      void activeCancel();
      void checkActiveComplete();
      void activeFailed( int activeRetCode, int errCode, QString errString );
      void restorePurchaseFailed();
      void findedPriceInfo( QString month, QString year, QString lifetime );
  
  public:
      virtual Q_INVOKABLE void beginActive( QString sn, bool uptoLifeTime ) = 0;
      virtual Q_INVOKABLE QString getSn() = 0;
      virtual Q_INVOKABLE QString getHardCode() = 0;
      virtual Q_INVOKABLE bool isFree() = 0;
      virtual Q_INVOKABLE bool isPro( bool* versionNotSupport = NULL ) = 0;
      virtual Q_INVOKABLE bool islifeTime( bool* isTimeLisence = NULL ) = 0;
      virtual Q_INVOKABLE void restorePurchase() = 0;
      virtual Q_INVOKABLE void checkPriceInfo();
  
      virtual bool CheckActive() = 0;
      virtual bool IsActived() = 0;
      virtual bool IsValidActived() = 0;
      virtual void SetValidActived( bool valid ) = 0;
      virtual void SyncServer() = 0;
      virtual void Reload() = 0;
      virtual std::uint64_t GetResiduSeconds() = 0;
  
      virtual void getLisenceCodeVersion( int& major, int& mini, int& least ) = 0;
  
  protected:
      enum ActiveType {
          oneMonth,
          oneYear,
          lifetime,
          unknow,
      };
      ActiveType getActiveType() const { return m_activeType; }
      void setActiveType( const ActiveType& activeType ) {
          m_activeType = activeType;
      }
      std::thread m_td_active;
      std::atomic_bool m_valid_active;
      QString m_activeTypeString;
      ActiveType m_activeType;
  };
  
  #endif // IACTIVEMGR_H
  ```

  IActiveMgr.cpp:

  ```c++
  #include "IActiveMgr.h"
  
  #include "ActiveMgr.h"
  #include "ActiveMgr_macStore.h"
  
  IActiveMgr::IActiveMgr( QObject* parent ):
      QObject( parent ), m_valid_active( false ) {}
  
  IActiveMgr::~IActiveMgr() {}
  
  IActiveMgr* IActiveMgr::create( QObject* parent ) {
  #ifdef MAC_APP_STORE
      return new ( std::nothrow ) ActiveMgr_macStore( parent );
  #endif
      return new ( std::nothrow ) CActiveMgr( parent );
  }
  
  QString IActiveMgr::getActiveTypeStr() const { return m_activeTypeString; }
  
  void IActiveMgr::setActiveTypeStr( const QString& activeType ) {
      m_activeTypeString = activeType;
  }
  
  void IActiveMgr::checkPriceInfo() {}
  
  ```

  ActiveMgr_macStore.h 源码如下：

  ```objectivec
  #ifndef ACTIVEMGR_MACSTORE_H
  #define ACTIVEMGR_MACSTORE_H
  
  #include <QLoggingCategory>
  #include <map>
  #include <memory>
  
  #include "IActiveMgr.h"
  
  Q_DECLARE_LOGGING_CATEGORY( macStoreActiveMgrLogCategory );
  
  class IAPCtl;
  class ActiveMgr_macStore : public IActiveMgr {
      Q_OBJECT
  public:
      using ProductInfo = QPair<QString, QString>;
  
      ActiveMgr_macStore( QObject* parent = nullptr );
      ~ActiveMgr_macStore();
  signals:
      void activeComplete( const QString& productID );
      void verfiyComplete( bool ok );
      void handleProductInfo( QVector<ProductInfo> datas );
      void reVerifyBuy( int msec );
  
  public slots:
      void onActiveComplete( const QString& productID );
      void onVerfiyComplete( bool ok );
      void onHandleProductInfo( QVector<ProductInfo> datas );
      void onReVerifyBuy( int msec );
  
  public:
      Q_INVOKABLE void beginActive( QString sn, bool uptoLifeTime ) override;
      Q_INVOKABLE QString getSn() override;
      Q_INVOKABLE QString getHardCode() override;
      Q_INVOKABLE bool isPro( bool* versionNotSupport = NULL ) override;
      Q_INVOKABLE bool islifeTime( bool* isTimeLisence = NULL ) override;
      Q_INVOKABLE void restorePurchase() override;
      Q_INVOKABLE void checkPriceInfo() override;
      Q_INVOKABLE bool isFree() override;
      Q_INVOKABLE void
      getLisenceCodeVersion( int& major, int& mini, int& least ) override;
  
      std::uint64_t GetResiduSeconds() override;
  
      bool CheckActive() override;
      bool IsActived() override;
      bool IsValidActived() override;
      void SyncServer() override;
      void Reload() override;
      QString getIAP_ID();
      void setActiveResult( bool result ) { m_valid_active = result; }
  
      virtual void SetValidActived( bool valid ) override {
          m_valid_active = valid;
      }
  
      void emitActiveCancel();
      void emitCheckActiveComplte();
      long long calIdentifier();
  
      bool inPurchase() { return m_inPurchase; }
      void setInPurchase( bool inPurchase ) { m_inPurchase = inPurchase; }
  
      QString getLifeTimeProductId() { return m_activeTypeMap["Lifetime"]; }
  
  private:
      std::map<QString, QString> m_activeTypeMap;
      QString m_productId;
      bool m_needBuy = false;
      bool m_inPurchase = false;
      std::shared_ptr<IAPCtl> m_pIAPCtl;
  };
  
  #endif // ACTIVEMGR_MACSTORE_H
  ```


  ActiveMgr_macStore.mm 源码如下：
  ```objectivec
  #include "ActiveMgr_macStore.h"
  
  #include <QApplication>
  #include <QDateTime>
  #include <QDebug>
  #include <QFile>
  #include <QTimeZone>
  #include <QTimer>
  #import <StoreKit/StoreKit.h>
  
  #include "../UserInfoColle/UserInfoColle_Qt.h"
  #include "../Utils/systemctlmac.h"
  
  #define DEBUG
  
  Q_LOGGING_CATEGORY( macStoreActiveMgrLogCategory,
                      "application.ActiveMgr_macStore" )
  
  //--- care begin ---//
  // SHARED_SECRET，PRODUCT_MONTH ，PRODUCT_YEAR ，PRODUCT_LIFETIME 
  // 需要根据 App Store Connect 后台配置做相应的修改。
  const QString SHARED_SECRET = "";
  const QString PRODUCT_MONTH = "com.company.product.auto.month";
  const QString PRODUCT_YEAR = "com.company.product.auto.year";
  const QString PRODUCT_LIFETIME = "com.company.product.lifetime";
  //--- care end ---//
  
  // #define STOREKIT_TEST
  
  NSString* SAVED_RECEIPT_DATA = @"receiptData";
  NSString* IDENTIFIER = @"identifier";
  
  // put the name of your view controller in place of MyViewController
  @interface MyViewController
      : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
      ActiveMgr_macStore* m_ativeMgr;
      NSURLSession* m_session;
      bool _checkProductInfo;
  }
  - (void)setActiveMgr:(ActiveMgr_macStore*)notified;
  - (void)restore;
  - (void)activePro;
  - (void)verify:(NSString*)receipt withFlag:(BOOL)flag;
  - (void)verifyOrderInformation:(BOOL)flag;
  - (void)checkActiveStatus:(BOOL)flag;
  - (void)activeCompleted:(NSString*)productId;
  - (bool)isActived;
  - (void)saveCurrTransationOrderInfo;
  - (void)saveReceiptData:(NSDictionary*)receiptData;
  - (NSDictionary*)getReceiptData;
  - (void)removeLocReceiptData;
  - (void)initIAP;
  - (void)checkPriceInfo;
  - (void)cancelURLSessionTask;
  @end
  
  @implementation MyViewController // the name of your view controller (same as
                                   // above) the code below will be added here
  
  - (void)setActiveMgr:(ActiveMgr_macStore*)notified {
      m_ativeMgr = notified;
  }
  
  - (void)activePro {
      NSLog( @"User requests to active pro version" );
      auto tmp = m_ativeMgr->getIAP_ID();
      NSString* productID = tmp.toNSString();
      if ( productID == nil || tmp.isEmpty() ) {
          qCDebug( macStoreActiveMgrLogCategory )
              << "-----------------------------Error,Get IAP ID is "
                 "failed---------------------------------------------------------"
                 "-";
          m_ativeMgr->emitActiveCancel();
          return;
      }
      qCDebug( macStoreActiveMgrLogCategory ) << "ProductID is " << productID;
  
      if ( [SKPaymentQueue canMakePayments] ) {
          qCDebug( macStoreActiveMgrLogCategory ) << "User can make payments";
  
          _checkProductInfo = false;
          SKProductsRequest* productsRequest = [[SKProductsRequest alloc]
              initWithProductIdentifiers:[NSSet setWithObject:productID]];
          productsRequest.delegate = self;
          [productsRequest start];
  
      } else {
          qCDebug( macStoreActiveMgrLogCategory )
              << "User cannot make payments due to parental controls";
          // this is called the user cannot make payments, most likely due to
          // parental controls
      }
  }
  
  - (void)productsRequest:(SKProductsRequest*)request
       didReceiveResponse:(SKProductsResponse*)response {
      if ( _checkProductInfo ) {
          NSArray* products = response.products;
  
          if ( products.count > 0 ) {
              QVector<ActiveMgr_macStore::ProductInfo> datas;
              for ( NSUInteger i = 0; i < products.count; ++i ) {
                  SKProduct* product = products[i];
  
                  // 获取价格
                  NSNumberFormatter* numberFormatter =
                      [[NSNumberFormatter alloc] init];
                  [numberFormatter
                      setFormatterBehavior:NSNumberFormatterBehavior10_4];
                  [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                  [numberFormatter setLocale:product.priceLocale];
  
                  NSString* priceString =
                      [numberFormatter stringFromNumber:product.price];
  
                  ActiveMgr_macStore::ProductInfo info;
                  info.first = QString::fromNSString( product.productIdentifier );
                  info.second = QString::fromNSString( priceString );
  
                  qCInfo( macStoreActiveMgrLogCategory )
                      << "product info " << info.first << ", price is "
                      << info.second;
                  datas.push_back( info );
              }
              if ( m_ativeMgr ) { emit m_ativeMgr->handleProductInfo( datas ); }
          }
      } else {
          SKProduct* validProduct = nil;
          int count = [response.products count];
          NSLog( @"IAP count is %i", count );
          if ( count > 0 ) {
              validProduct = [response.products objectAtIndex:0];
              NSLog( @"Products Available!" );
              [self purchase:validProduct];
          } else if ( !validProduct ) {
              NSLog( @"No products available" );
              // this is called if your product id is not valid
              //, this shouldn't be called unless that happens.
              m_ativeMgr->emitActiveCancel();
          }
      }
  }
  
  - (void)purchase:(SKProduct*)product {
      SKPayment* payment = [SKPayment paymentWithProduct:product];
  
      [[SKPaymentQueue defaultQueue] addPayment:payment];
  }
  
  - (void)restore {
      // this is called when the user restores purchases, you should hook this up
      // to a button
      [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
  }
  
  - (void)saveCurrTransationOrderInfo {
      NSURL* recepitURL = [[NSBundle mainBundle] appStoreReceiptURL];
      NSData* receipt = [NSData dataWithContentsOfURL:recepitURL];
      NSString* receiptData = [receipt base64EncodedStringWithOptions:0];
      if ( receiptData != nil ) {
          [self saveReceiptData:@{ @"receipt" : receiptData }];
      } else {
          qCDebug( macStoreActiveMgrLogCategory )
              << "Warning, receipt Data is nil";
      }
  }
  - (void)verifyOrderInformation:(BOOL)flag {
      auto receiptData = [self getReceiptData];
      NSString* receipt = nil;
      if ( receiptData != nil ) {
          receipt = receiptData[@"receipt"];
  #ifndef STOREKIT_TEST
          [self verify:receipt withFlag:NO];
  #else
          [self activeCompleted:m_ativeMgr->getIAP_ID().toNSString()];
  #endif
      }
  }
  
  // 用户做 restore 操作，会调用下面的代码
  - (void)paymentQueueRestoreCompletedTransactionsFinished:
      (SKPaymentQueue*)queue {
      bool isLifetimeVersion = false;
      qCDebug( macStoreActiveMgrLogCategory )
          << "received restored transactions:" << queue.transactions.count;
      for ( SKPaymentTransaction* transaction in queue.transactions ) {
          if ( transaction.transactionState ==
               SKPaymentTransactionStateRestored ) {
              // called when the user successfully restores a purchase
              NSLog( @"Transaction state -> Restored" );
  
              NSString* productID = transaction.payment.productIdentifier;
  
              NSLog( @"Restore product ID is %@", productID );
              [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
          }
      }
      if ( 0 == queue.transactions.count ) {
          emit m_ativeMgr->restorePurchaseFailed();
      } else {
          [self saveCurrTransationOrderInfo];
          [self verifyOrderInformation:NO];
      }
      m_ativeMgr->emitCheckActiveComplte();
  }
  
  - (void)request:(SKRequest*)request didFailWithError:(NSError*)error {
      NSLog( @"--------error message pops up---------" );
      emit m_ativeMgr->verfiyComplete( false );
  }
  
  - (void)paymentQueue:(SKPaymentQueue*)queue
      restoreCompletedTransactionsFailedWithError:(NSError*)error {
      NSLog( @"Restore error = %@", error );
      emit m_ativeMgr->restorePurchaseFailed();
  }
  
  - (void)paymentQueue:(SKPaymentQueue*)queue
      updatedTransactions:(NSArray*)transactions {
      for ( SKPaymentTransaction* transaction in transactions ) {
          // if you have multiple in app purchases in your app,
          // you can get the product identifier of this transaction
          // by using transaction.payment.productIdentifier
          //
          // then, check the identifier against the product IDs
          // that you have defined to check which product the user
          // just purchased
  
          NSString* productId = transaction.payment.productIdentifier;
          NSLog( @"Buy product identifier [ %@ ] transaction ", productId );
  
          switch ( transaction.transactionState ) {
          case SKPaymentTransactionStatePurchasing:
              NSLog( @"Transaction state -> Purchasing,time is %lli",
                     QDateTime::currentMSecsSinceEpoch() );
              // called when the user is in the process of purchasing, do not add
              // any of your own code here.
              m_ativeMgr->setInPurchase( true );
              break;
          case SKPaymentTransactionStatePurchased: {
              // this is called when the user has successfully purchased the
              // package (Cha-Ching!)
              emit m_ativeMgr->startActive();
              NSLog( @"Transaction state -> Purchased,time is %lli",
                     QDateTime::currentMSecsSinceEpoch() );
              [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
              if ( m_ativeMgr->inPurchase() ) {
                  [self saveCurrTransationOrderInfo];
                  [self verifyOrderInformation:NO];
              }
          } break;
          case SKPaymentTransactionStateRestored: {
              NSLog( @"Transaction state -> Restored" );
              // add the same code as you did from
              // SKPaymentTransactionStatePurchased here
              [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
              // [ self saveCurrTransationOrderInfo];
              //                    [ self verifyOrderInformation:NO];
          } break;
          case SKPaymentTransactionStateFailed: {
              // called when the transaction does not finish
              if ( transaction.error.code == SKErrorPaymentCancelled ) {
                  NSLog( @"Transaction state -> Cancelled" );
                  // the user cancelled the payment ;
              }
              qCDebug( macStoreActiveMgrLogCategory )
                  << "error information is "
                  << transaction.error.localizedDescription;
  
              m_ativeMgr->emitActiveCancel();
              [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
              NSLog( @"transaction error code is [ %li ]",
                     transaction.error.code );
          } break;
          default:
              qCDebug( macStoreActiveMgrLogCategory )
                  << "Transaction state is " << transaction.transactionState;
              break;
          }
      }
  }
  
  - (void)verify:(NSString*)receipt withFlag:(BOOL)flag {
      if ( nil == receipt ) {
          if ( m_ativeMgr ) { emit m_ativeMgr->verfiyComplete( false ); }
          return;
      }
  
      NSLog( @"mainBundle is %@", [NSBundle mainBundle] );
  
      if ( !receipt ) {
          qCDebug( macStoreActiveMgrLogCategory )
              << "The transaction voucher is empty, the verification failed";
          if ( m_ativeMgr ) { emit m_ativeMgr->verfiyComplete( false ); }
          return;
      }
      // 购买成功将交易凭证发送给服务端进行再次校验
      //    [self handleActionWithType:SIAPPurchSuccess data:receipt]; //todo
  
      NSError* error;
      NSDictionary* requestContents = @{
          @"receipt-data" : receipt,
          @"password" : SHARED_SECRET.toNSString()
      };
      NSData* requestData =
          [NSJSONSerialization dataWithJSONObject:requestContents
                                          options:0
                                            error:&error];
  
      if ( !requestData ) { // 交易凭证为空验证失败
          qCDebug( macStoreActiveMgrLogCategory )
              << "The transaction voucher is empty, the verification failed, "
                 "error is "
              << error.localizedDescription;
          if ( m_ativeMgr ) { emit m_ativeMgr->verfiyComplete( false ); }
          return;
      }
  
      // In the test environment, use
      // https://sandbox.itunes.apple.com/verifyReceipt In the real environment,
      // use https://buy.itunes.apple.com/verifyReceipt
  
      NSString* serverString = @"https://buy.itunes.apple.com/verifyReceipt";
      if ( flag ) {
          serverString = @"https://sandbox.itunes.apple.com/verifyReceipt";
      }
      NSURL* storeURL = [NSURL URLWithString:serverString];
      NSMutableURLRequest* storeRequest =
          [NSMutableURLRequest requestWithURL:storeURL];
      [storeRequest setHTTPMethod:@"POST"];
      [storeRequest setHTTPBody:requestData];
  
      m_session = [NSURLSession sharedSession];
  
      NSURLSessionDataTask* dataTask = [m_session
          dataTaskWithRequest:storeRequest
            completionHandler:^( NSData* __nullable data,
                                 NSURLResponse* __nullable response,
                                 NSError* __nullable error ) {
              // 拿到响应头信息
              NSHTTPURLResponse* res = (NSHTTPURLResponse*)response;
  
              bool useLifetime = false;
  
              if ( error ) {
                  qCDebug( macStoreActiveMgrLogCategory )
                      << "Unable to connect to the server, purchase verification "
                         "failed ,error is "
                      << error.localizedDescription;
                  if ( m_ativeMgr ) {
                      // restore old verfiy result
                      emit m_ativeMgr->verfiyComplete( m_ativeMgr->isPro() );
                  }
              } else {
  
                  NSError* error;
                  NSDictionary* jsonResponse =
                      [NSJSONSerialization JSONObjectWithData:data
                                                      options:0
                                                        error:&error];
                  if ( !jsonResponse ) {
                      qCDebug( macStoreActiveMgrLogCategory )
                          << "Apple server verification data returned as null "
                             "Verification failed";
                      emit m_ativeMgr->verfiyComplete( false );
                      return;
                  }
  #ifdef DEBUG
                  auto receiptResPath =
                      QString( "%1/receiptRes.json" ).arg( getenv( "TMPDIR" ) );
                  QFile logFile( receiptResPath );
                  if ( !logFile.open( QIODevice::WriteOnly | QIODevice::Text |
                                      QIODevice::Append ) ) {
                      NSLog( @"open file receiptRes log is faied,and path is %@",
                             receiptResPath.toNSString() );
                  }
                  QTextStream logout( &logFile );
  #endif
  
                  // 先验证正式服务器,如果正式服务器返回21007再去苹果测试服务器验证,
                  // 沙盒测试环境苹果用的是测试服务器
                  NSString* status =
                      [NSString stringWithFormat:@"%@", jsonResponse[@"status"]];
                  if ( status && [status isEqualToString:@"21007"] ) {
                      [self verify:receipt withFlag:YES];
                  } else if ( status && [status isEqualToString:@"0"] ) {
                      NSArray* pendingRenewInfo =
                          jsonResponse[@"pending_renewal_info"];
                      NSArray* allTransactions =
                          jsonResponse[@"receipt"][@"in_app"];
                      NSArray* lastest_receipt_info =
                          jsonResponse[@"latest_receipt_info"];
                      bool verifyRet = false;
  
                      if ( allTransactions != nil ) {
  #ifdef DEBUG
                          NSLog( @"allTransactions count is %li",
                                 [allTransactions count] );
  #endif
                          for ( NSUInteger i = 0; i < [allTransactions count];
                                ++i ) {
                              NSString* productId =
                                  allTransactions[i][@"product_id"];
                              //                            NSLog(@"Transcation
                              //                            index %li ,product id
                              //                            is %@",i,productId);
                              if ( m_ativeMgr ) {
                                  if ( QString::fromNSString( productId ) ==
                                       m_ativeMgr->getLifeTimeProductId() ) {
                                      verifyRet = true;
                                      useLifetime = true;
                                      break;
                                  }
                              }
                          }
                      }
                      qCDebug( macStoreActiveMgrLogCategory )
                          << "useLifeTime is " << useLifetime;
                      if ( !verifyRet ) {
                          qCInfo( macStoreActiveMgrLogCategory )
                              << "verifyRet is failed - 397";
                          if ( pendingRenewInfo != nil ) {
                              for ( NSUInteger i = 0;
                                    i < [pendingRenewInfo count];
                                    ++i ) {
                                  NSInteger autoRenewStatus =
                                      [pendingRenewInfo[i][@"auto_renew_status"]
                                          intValue];
                                  NSString* expirationIntent =
                                      pendingRenewInfo[i][@"expiration_intent"];
  #ifdef DEBUG
                                  NSLog(
                                      @"pendingRenewInfo Item ,product id is %@",
                                      pendingRenewInfo[i][@"product_id"] );
                                  NSLog( @"pendingRenewInfo Item "
                                         @",auto_renew_status is %li",
                                         autoRenewStatus );
                                  NSLog( @"pendingRenewInfo Item "
                                         @",expiration_intent is %@",
                                         expirationIntent );
  #endif
                                  if ( nil == expirationIntent ) {
                                      verifyRet = true;
                                      break;
                                  } else {
                                      qCDebug( macStoreActiveMgrLogCategory )
                                          << "product is "
                                          << pendingRenewInfo[i][@"product_id"]
                                          << " ,expirationIntent is "
                                          << [expirationIntent intValue];
                                  }
                              }
  
                          } else {
                              verifyRet = true;
                          }
                      }
                      if ( m_ativeMgr ) {
                          if ( !useLifetime && lastest_receipt_info != nil &&
                               lastest_receipt_info.count > 0 ) {
                              long long expires_date_ms =
                                  [lastest_receipt_info[0][@"expires_date_ms"]
                                      longLongValue];
  
                              //                            expires_date demo is
                              //                            "2024-01-09 08:19:17
                              //                            Etc/GMT";
                              QDateTime dateTime =
                                  QDateTime::fromMSecsSinceEpoch(
                                      expires_date_ms, QTimeZone::utc() )
                                      .addSecs( 60 );
                              auto curDateTime =
                                  QDateTime::currentDateTime().toUTC();
                              auto interval = curDateTime.msecsTo( dateTime );
                              qCInfo( macStoreActiveMgrLogCategory )
                                  << "expires_date is " << dateTime
                                  << ", cur date time is " << curDateTime;
                              if ( interval > 0 ) {
                                  qCInfo( macStoreActiveMgrLogCategory )
                                      << "expires_date is " << dateTime
                                      << " , interval msecs is " << interval
                                      << ",try begin timer";
                                  emit m_ativeMgr->reVerifyBuy(
                                      static_cast<int>( interval ) );
                              } else {
                                  verifyRet = false;
                              }
                          }
                          qCInfo( macStoreActiveMgrLogCategory )
                              << "verifyRet is " << verifyRet << " , 439";
                          emit m_ativeMgr->verfiyComplete( verifyRet );
                      }
  
                  } else {
                      qCInfo( macStoreActiveMgrLogCategory )
                          << "The error code returned by the check is " << status;
                      if ( m_ativeMgr ) {
                          emit m_ativeMgr->verfiyComplete( false );
                      }
                  }
  #ifdef DEBUG
                  NSLog( @"----Validation results-----pending_renewal_info %@",
                         jsonResponse[@"pending_renewal_info"] );
                  // 解析拿到的响应数据
                  NSError* err;
                  NSData* jsonData =
                      [NSJSONSerialization dataWithJSONObject:jsonResponse
                                                      options:0
                                                        error:&err];
                  logout << QString::fromNSString( [[NSString alloc]
                      initWithData:jsonData
                          encoding:NSUTF8StringEncoding] );
  #endif
              }
            }];
  
      // 刚创建出来的task默认是挂起状态的，需要调用该方法来启动任务（执行任务）
      [dataTask resume];
  }
  
  - (void)checkActiveStatus:(BOOL)flag {
  #ifndef STOREKIT_TEST
      NSDictionary* orderInfo = [self getReceiptData];
      if ( orderInfo != nil ) {
          NSString* receipt = orderInfo[@"receipt"];
  
          [self verify:receipt withFlag:flag];
      } else {
          emit m_ativeMgr->verfiyComplete( false );
      }
  #endif
  }
  
  - (void)activeCompleted:(NSString*)productId {
      emit m_ativeMgr->activeComplete( QString::fromNSString( productId ) );
  
      //    //从沙盒中获取交易凭证并且拼接成请求体数据
      //        NSURL *receiptUrl = [[NSBundle mainBundle] appStoreReceiptURL];
      //        NSData *receiptData = [NSData dataWithContentsOfURL:receiptUrl];
      //        NSString *receiptString = [receiptData
      //        base64EncodedStringWithOptions:0];
      //
      //    QFile tmp("base64_encodeed_receipt");
      //    if(!tmp.open(QIODevice::WriteOnly | QIODevice::Text)){
      //        return;
      //    }
      //    tmp.write(QString::fromNSString(receiptString).toLocal8Bit());
  
      [self setActiveStatus:YES];
  }
  
  - (void)setActiveStatus:(BOOL)isActive {
      long long value = isActive ? m_ativeMgr->calIdentifier() : 0;
      [[NSUserDefaults standardUserDefaults]
          setValue:[NSString stringWithFormat:@"%lld", value]
            forKey:IDENTIFIER];
      // use NSUserDefaults so that you can load whether or not they bought it
      // it would be better to use KeyChain access, or something more secure
      // to store the user data, because NSUserDefaults can be changed.
      // You're average downloader won't be able to change it very easily, but
      // it's still best to use something more secure than NSUserDefaults.
      // For the purpose of this tutorial, though, we're going to use
      // NSUserDefaults
      [[NSUserDefaults standardUserDefaults] synchronize];
  }
  
  - (bool)isActived {
      NSString* identifier =
          [[NSUserDefaults standardUserDefaults] valueForKey:IDENTIFIER];
      [[NSUserDefaults standardUserDefaults] synchronize];
      auto expectIdentifier = m_ativeMgr->calIdentifier();
      return ( QString::fromNSString( identifier ).toLongLong() ==
               expectIdentifier );
  }
  
  - (void)saveReceiptData:(NSDictionary*)receiptData {
      [[NSUserDefaults standardUserDefaults] setValue:receiptData
                                               forKey:SAVED_RECEIPT_DATA];
      [[NSUserDefaults standardUserDefaults] synchronize];
  }
  
  - (NSDictionary*)getReceiptData {
      return
          [[NSUserDefaults standardUserDefaults] valueForKey:SAVED_RECEIPT_DATA];
  }
  
  - (void)removeLocReceiptData {
      [[NSUserDefaults standardUserDefaults]
          removeObjectForKey:SAVED_RECEIPT_DATA];
      [[NSUserDefaults standardUserDefaults] synchronize];
  }
  
  - (void)initIAP {
      [self checkPriceInfo];
      [self verifyOrderInformation:NO];
      [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
  }
  
  - (void)checkPriceInfo {
      _checkProductInfo = false;
      if ( [SKPaymentQueue canMakePayments] ) {
          NSLog( @"User can make payments" );
  
          _checkProductInfo = true;
          NSArray* array =
              [NSArray arrayWithObjects:PRODUCT_MONTH.toNSString(),
                                        PRODUCT_YEAR.toNSString(),
                                        PRODUCT_LIFETIME.toNSString(),
                                        nil];
          SKProductsRequest* productsRequest = [[SKProductsRequest alloc]
              initWithProductIdentifiers:[NSSet setWithArray:array]];
          productsRequest.delegate = self;
          [productsRequest start];
      }
  }
  
  - (void)cancelURLSessionTask {
      [m_session getAllTasksWithCompletionHandler:^(
                     NSArray<__kindof NSURLSessionTask*>* _Nonnull tasks ) {
        for ( NSUInteger i = 0; i < [tasks count]; ++i ) {
            if ( NSURLSessionTaskStateRunning == tasks[i].state ) {
                [tasks[i] cancel];
            }
        }
      }];
  }
  
  @end
  
  class IAPCtl {
  public:
      IAPCtl() {
          if ( m_ctl == nil ) { m_ctl = [[MyViewController alloc] init]; }
      }
      ~IAPCtl() {
          [m_ctl setActiveMgr:nil];
          [m_ctl cancelURLSessionTask];
          [m_ctl release];
      }
      MyViewController* getCtl() { return m_ctl; }
  
  private:
      MyViewController* m_ctl = nil;
  };
  
  ActiveMgr_macStore::ActiveMgr_macStore( QObject* parent ):
      IActiveMgr( parent ),
      m_activeTypeMap{ { "1-Month", PRODUCT_MONTH },
                       { "1-Year", PRODUCT_YEAR },
                       { "Lifetime", PRODUCT_LIFETIME } } {
      m_pIAPCtl = std::make_shared<IAPCtl>();
      [m_pIAPCtl->getCtl() setActiveMgr:this];
      [m_pIAPCtl->getCtl() initIAP];
      connect( this,
               &ActiveMgr_macStore::activeComplete,
               this,
               &ActiveMgr_macStore::onActiveComplete,
               Qt::QueuedConnection );
      connect( this,
               &ActiveMgr_macStore::verfiyComplete,
               this,
               &ActiveMgr_macStore::onVerfiyComplete,
               Qt::QueuedConnection );
      connect( this,
               &ActiveMgr_macStore::handleProductInfo,
               this,
               &ActiveMgr_macStore::onHandleProductInfo,
               Qt::QueuedConnection );
      connect( this,
               &ActiveMgr_macStore::reVerifyBuy,
               this,
               &ActiveMgr_macStore::onReVerifyBuy,
               Qt::QueuedConnection );
  }
  
  ActiveMgr_macStore::~ActiveMgr_macStore() { m_pIAPCtl.reset(); }
  
  void ActiveMgr_macStore::beginActive( QString sn, bool uptoLifeTime ) {
      Q_UNUSED( sn );
      // 先检测客户端与服务器的漏单情况
      [m_pIAPCtl->getCtl() checkActiveStatus:NO];
      m_needBuy = true;
  #ifdef STOREKIT_TEST
      emit verfiyComplete( false );
  #endif
  }
  
  QString ActiveMgr_macStore::getSn() { return ""; }
  
  // 针对mac 应用市场版本，不再需要硬件码
  QString ActiveMgr_macStore::getHardCode() { return ""; }
  
  bool ActiveMgr_macStore::isPro( bool* versionNotSupport ) {
      return [m_pIAPCtl->getCtl() isActived];
  }
  
  bool ActiveMgr_macStore::islifeTime( bool* isTimeLisence ) {
      return m_activeTypeMap["Lifetime"] == m_productId;
  }
  
  void ActiveMgr_macStore::restorePurchase() { [m_pIAPCtl->getCtl() restore]; }
  
  std::uint64_t ActiveMgr_macStore::GetResiduSeconds() { return 0; }
  
  bool ActiveMgr_macStore::CheckActive() { return isPro(); }
  
  bool ActiveMgr_macStore::IsActived() { return CheckActive(); }
  
  bool ActiveMgr_macStore::IsValidActived() { return m_valid_active; }
  
  void ActiveMgr_macStore::SyncServer() {
      // 定时校验订单，来检测订单是否有效
      [m_pIAPCtl->getCtl() checkActiveStatus:NO];
  }
  
  void ActiveMgr_macStore::Reload() {
      // todo
  }
  
  QString ActiveMgr_macStore::getIAP_ID() {
      auto search = m_activeTypeMap.find( getActiveTypeStr() );
      if ( search != m_activeTypeMap.end() ) { return search->second; }
      return "";
  }
  
  void ActiveMgr_macStore::onActiveComplete( const QString& productID ) {
      m_productId = productID;
      qCDebug( macStoreActiveMgrLogCategory ) << "m_productId is" << m_productId;
      emit activeFinish( 0LL );
  }
  
  void ActiveMgr_macStore::onVerfiyComplete( bool ok ) {
      if ( ok ) {
          [m_pIAPCtl->getCtl() activeCompleted:getIAP_ID().toNSString()];
      }
      if ( !ok ) {
          [m_pIAPCtl->getCtl() setActiveStatus:NO];
          if ( m_needBuy ) {
              [m_pIAPCtl->getCtl() activePro];
              m_needBuy = false;
          } else {
              emit activeCancel();
              emit activeFinish( 666LL );
          }
      }
      qCInfo( macStoreActiveMgrLogCategory )
          << "Verfiy complete,and result is" << ok;
  }
  
  void ActiveMgr_macStore::onHandleProductInfo( QVector<ProductInfo> datas ) {
      QString monthPrice, yearPrice, lifetimePrice;
      for ( auto& item : datas ) {
          if ( PRODUCT_MONTH == item.first ) {
              monthPrice = item.second;
          } else if ( PRODUCT_YEAR == item.first ) {
              yearPrice = item.second;
          } else if ( PRODUCT_LIFETIME == item.first ) {
              lifetimePrice = item.second;
          }
      }
      emit findedPriceInfo( monthPrice, yearPrice, lifetimePrice );
  }
  
  void ActiveMgr_macStore::emitActiveCancel() { emit activeCancel(); }
  
  void ActiveMgr_macStore::emitCheckActiveComplte() {
      emit checkActiveComplete();
  }
  
  long long ActiveMgr_macStore::calIdentifier() {
      auto uuid = SystemCtlMac::get_platform_uuid();
      long long ret = 0;
      for ( auto i = 0u; i < uuid.size(); ++i ) {
          ret += uuid[i];
      }
      ret = ret * 5333 * 6907;
      return ret;
  }
  
  void ActiveMgr_macStore::checkPriceInfo() {
      qCDebug( macStoreActiveMgrLogCategory )
          << "ActiveMgr_macStore::checkPriceInfo()";
      [m_pIAPCtl->getCtl() checkPriceInfo];
  }
  
  void ActiveMgr_macStore::onReVerifyBuy( int msec ) {
      qCDebug( macStoreActiveMgrLogCategory )
          << " ActiveMgr_macStore::onReVerifyBuy(int msec) ,msec is " << msec;
      QTimer::singleShot( msec, this, [this]() {
          qCInfo( macStoreActiveMgrLogCategory )
              << "reVerify buy time is out,and verifyOrderInformation";
          if ( m_pIAPCtl->getCtl() ) {
              [m_pIAPCtl->getCtl() verifyOrderInformation:NO];
          }
      } );
  }
  bool ActiveMgr_macStore::isFree() { return !isPro(); }
  void ActiveMgr_macStore::getLisenceCodeVersion( int& major,
                                                  int& mini,
                                                  int& least ) {}
  
  ```

  备注： 上述代码有一部分重载函数是使用激活码激活的模块所必须的，appstore 端不需要，只是为了兼容而做出的实现。

  #### 2.1.2.1 如何调用ActiveMgr购买、恢复购买，获取价格信息？

  ```c++
  auto active = IActiveMgr::create(parent);
  // 开始购买
  active->beginActive("",false);
  // 恢复购买
  active->restorePurchase();
  // 获取后台配置的价格信息
  connect(active,&IActiveMgr::findedPriceInfo,qApp,[](QString month,
                                                      QString year, 
                                                      QString lifetime){
      //will get price info
  });
  active->checkPriceInfo();
  ```

  ## 2.2 需要多久可以上架呢？

刚刚加入公司时，对Mac开发一无所知，现开发完成 mac 版本的应用，才开始着手将其上架至App Store。在上架过程中，遇到了沙盒限制、内购集成和苹果审核等问题，耗时超过两个月才最终成功上架。现在有了这份详尽的文档，相信即将开始上架应用的开发伙伴们，将在2周内成功提交应用至App Store。提交后，等待苹果商店的反馈和问题修复（预计需要两周时间），便可期待App Store带来的收益。

  # 3. 遇到的一些问题

  ## 3.1 校验订单，但校验结果一直返回 21003 

  最开始怀疑是代码不正确、机器异常、沙盒测试账号有异常，但实际上是发送校验请求有误（oops）。解决办法的链接可参见 

  ["The receipt could not be authenticated" - should it be checked again?](https://stackoverflow.com/questions/58615404/the-receipt-could-not-be-authenticated-should-it-be-checked-again)

  ## 3.2 Could not find the Qt platform plugin "cocoa" in ""

  在 qmake 工程中添加内购流程后一切正常，但使用 qmake 装换成 xcode 工程后调试，一直提示 
  ```
  Could not find the Qt platform plugin "cocoa" in ""
  This application failed to start because no 
  Qt platform plugin could be initialized. 
  Reinstalling the application may fix this problem.
  ```

  解决方法：安装 *.app/Contents/embedded.provisionprofile 文件即可，该文件的一些信息可见
  - [细谈证书与Provisioning Profile](https://www.jianshu.com/p/abd3c55db48e)

  ## 3.3 使用真实账号登陆沙盒测试账号购买异常
  （这是正确的，oops），详见[【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)

  ## 3.4 偶现程序已启动就进入到  paymentQueue
  ```cpp
  (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:
          (NSArray *)transactions{
                          ...
          }
  ```
  若上次的 Transaction 未调用 
  ```cpp
  [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
  ```
  就会出现该情况

  ## 3.5 修复 “appstore 版本开启沙盒”后无法让多进程通信

  主程序使用原 权利文件进行签名

  需要对子程序单独使用 权利文件进行签名，使用这个：

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" 
      "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
  <plist version="1.0">
  <dict>
  	<key>com.apple.security.app-sandbox</key>
  	<true/>
  	<key>com.apple.security.inherit</key>
  	<true/>
  </dict>
  </plist>
  ```

  来解决这个问题

---

  # 4. 参考链接
  - [iOS Handle Refunds 处理退款 --- WWDC20（Session 10661） ](https://juejin.cn/post/6845166890420011022)
  - [ 【WWDC20】 10659 - 介绍 Xcode 中 StoreKit 测试 ](https://juejin.cn/post/6857502956317736968)
  - [Apple Developer Documentation](https://developer.apple.com/documentation/storekit/original_api_for_in-app_purchase/subscriptions_and_offers/handling_subscriptions_billing?language=objc)
  - [ios - 如何在应用重启或重新安装时检查iOS中的订阅是否处于事件状态](https://www.coder.work/article/7152298)
  - [Overview for configuring in-app purchases](https://help.apple.com/app-store-connect/#/devb57be10e7)
  - [How do you add an in-app purchase to an iOS application?](https://stackoverflow.com/questions/19556336/how-do-you-add-an-in-app-purchase-to-an-ios-application)
  - [In-App Purchase Tutorial: Getting Started](https://www.raywenderlich.com/5456-in-app-purchase-tutorial-getting-started)
  - [In-App Purchase（iap）快速指南](https://www.cnblogs.com/kris888/archive/2012/09/21/2696754.html)
  - [Apple App Store Receipt Checker - RevenueCat](https://www.revenuecat.com/apple-receipt-checker)
  - [App 内购买项目 - Apple Developer](https://developer.apple.com/cn/in-app-purchase/)
  - [开发者账户帮助](https://developer.apple.com/cn/help/account/)
  - [How to Publish Qt Applications in the Mac App Store](https://www.qt.io/blog/2012/04/03/how-to-publish-qt-applications-in-the-mac-app-store-2)
  - [Qt程序上架Mac App Store 基本流程_u013820121的专栏-CSDN博客](https://blog.csdn.net/u013820121/article/details/106624994)
  - [Mac下将QT程序打包成能发布到Mac App store的pkg安装包_casun_li的专栏-CSDN博客](https://blog.csdn.net/casun_li/article/details/71741968)
  - [How do you fix "code object is not signed at all In subcomponent:" in Xcode 6, Mac OS X Yosemite or Mavericks?](https://stackoverflow.com/questions/29076321/how-do-you-fix-code-object-is-not-signed-at-all-in-subcomponent-in-xcode-6-m)
  - [Mac下将QT程序打包成能发布到Mac App store的pkg安装包](https://www.geek-share.com/detail/2706784546.html)
  - [松果压制上架 Mac App Store 及沙盒机制的部分说明](https://walkginkgo.com/ffmpeg/2018/07/15/MAS-sanbox.html)
  - [发布Unity项目到macOS AppStore和Steam](https://yemi.me/2020/02/17/submit-unity-macos-build-to-steam-appstore/)
  - [在 Sandboxed Mac App 中嵌入第三方可执行文件](https://xujiwei.com/blog/2013/02/embed-third-party-binary-in-sandboxed-mac-app/)
  - [3、苹果应用商店上架流程](https://www.jianshu.com/p/9dff65d391d3)
  - [App Store 审核指南](https://developer.apple.com/cn/app-store/review/guidelines/)
  - [【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)
  - [ Electron - Mac App Store 应用程序提交指南](https://www.electronjs.org/zh/docs/latest/tutorial/mac-app-store-submission-guide)

---


- [上一级](README.md)
- 下一篇 -> [mac 开发资料](macDevelopSome.md)
