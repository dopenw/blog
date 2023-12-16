<!-- 上架 mac 应用程序到苹果商店  -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [上架 mac 应用程序到苹果商店](#上架-mac-应用程序到苹果商店)
  - [证书](#证书)
  - [密钥](#密钥)
  - [标识符](#标识符)
  - [App Store 审核指南](#app-store-审核指南)
  - [支持应用内购](#支持应用内购)
    - [遇到的问题](#遇到的问题)
      - [校验订单，但校验结果一直返回 21003](#校验订单但校验结果一直返回-21003)
      - [Could not find the Qt platform plugin "cocoa" in ""](#could-not-find-the-qt-platform-plugin-cocoa-in-)
      - [使用真实账号登陆沙盒测试账号购买异常](#使用真实账号登陆沙盒测试账号购买异常)
      - [偶现程序已启动就进入到  paymentQueue](#偶现程序已启动就进入到--paymentqueue)
    - [从IAP获取内购的价格](#从iap获取内购的价格)
    - [相关链接](#相关链接)
  - [支持首用户免费使用一段时间](#支持首用户免费使用一段时间)
  - [修复 “appstore 版本开启沙盒”后无法让多进程通信](#修复-appstore-版本开启沙盒后无法让多进程通信)
- [参考链接](#参考链接)

<!-- /code_chunk_output -->



# 上架 mac 应用程序到苹果商店

## 证书

证书分为两种：

- 开发证书 - 开发证书用于在设备上运行 app 以及使用 app 功能
- 分发证书 - 分发证书用于分发 app 以进行测试和上传到 App Store Connect

开发证书属于个人。您总共可以创建两个 iOS 开发证书和两个 Mac 开发证书。在您的开发者帐户中，电脑名称会追加到开发证书名称后面 (例如，`Gita Kumar (Work Mac)`，其中 `Work Mac` 是电脑名称)，便于您识别证书。

分发证书属于团队，每个团队只能拥有一种类型的分发证书 ([Developer ID](https://help.apple.com/developer-account/?lang=zh-cn#/dev84de6b2b4) 证书除外)。只有[帐户持有人](https://help.apple.com/developer-account/?lang=zh-cn#/devb9535fd68)或[管理](https://help.apple.com/developer-account/?lang=zh-cn#/deva73372ae0)职能可以创建分发证书 (如果您以个人身份注册，您即为帐户持有人)。

您可以使用 Xcode 或开发者帐户创建和撤销证书。

- Developer ID 证书
    - ***Developer ID App***：用于对 Mac app 进行签名的证书。
    - ***Developer ID 安装器：***用于对包含已签名 app 的 Mac 安装器软件包进行签名的证书。
- 企业分发证书
- WatchKit 服务证书让您可以使用 Apple 推送通知服务 (APNs) 在 Apple Watch 上推送复杂功能的更新。
- VoIP 服务证书让您能使用 Apple 推送通知服务 (APNs) 为 VoIP app 推送更新。
- Mac 上的“钥匙串访问”让您可以创建证书签名请求 (CSR)。

[](https://help.apple.com/developer-account/?lang=zh-cn#/devbfa00fef7)

## 密钥

- 您可以用私钥访问某些 app 服务，并验证与这些服务之间的通信，如 APNs、MusicKit 和 DeviceCheck。您将在服务请求的 JSON 网络令牌 (JWT) 中使用私钥。
- 在创建 JSON 网络令牌 (JWT) 来与启用的服务通信时，您需要密钥标识符。
- 创建密钥后，您可以撤销、编辑或下载密钥。一个密钥只能下载一次。在 iOS、Apple tvOS 和 watchOS 上的 app 之间共享私钥。

## 标识符

- App ID 可识别预置描述文件中的 app。它是一个由两部分组成的字符串，用于标识来自某一开发团队的一个或多个 app。App ID 共有两类：适用于单个 app 的精确 App ID，以及适用于一组 app 的通配符 App ID。在一个 App ID 中启用的 app 功能将作为一个白名单，为一个或多个 app 提供可用功能。您可以在创建 App ID 时启用 app 功能，也可以稍后修改这些设置。默认情况下，精确 App ID 会启用 Game Center 和“App 内购买项目”。自 Xcode 11.4 起，单个 App ID 可用于构建 iOS、macOS、Apple tvOS 和 watchOS app。
- 您可以在“Certificates, Identifiers & Profiles”(证书、标识符和描述文件) 中为您的 App ID 查看和启用功能。包含已修改 App ID 的预置描述文件将变为无效。您需要重新生成使用该 App ID 的预置描述文件。
- 您可以在“Certificates, Identifiers & Profiles”(证书、标识符和描述文件) 中为您的 App ID 查看和启用功能。包含已修改 App ID 的预置描述文件将变为无效。您需要重新生成使用该 App ID 的预置描述文件。

## App Store 审核指南

详见 

[App Store 审核指南](https://developer.apple.com/cn/app-store/review/guidelines/)

## 支持应用内购

```objectivec
#include "ActiveMgr_macStore.h"
#import <StoreKit/StoreKit.h>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QApplication>
#include <QDateTime>

Q_LOGGING_CATEGORY(macStoreActiveMgrLogCategory,"application.ActiveMgr_macStore")
const QString SHARED_SECRET="YOUR_APP_SHARED_SECRET";

//link https://stackoverflow.com/questions/19556336/how-do-you-add-an-in-app-purchase-to-an-ios-application
//link2 https://blog.csdn.net/box_kun/article/details/85243602
//link3 https://www.cnblogs.com/kris888/archive/2012/09/21/2696754.html

//#define STOREKIT_TEST

NSString * SAVED_RECEIPT_DATA = @"receiptData";
NSString * IDENTIFIER =@"identifier";

//put the name of your view controller in place of MyViewController
@interface MyViewController:NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>{
    ActiveMgr_macStore * m_ativeMgr;
}
+ (MyViewController *)getInstance;
- (void)setActiveMgr:(ActiveMgr_macStore *)notified;
- (void)restore;
- (void)activePro;
- (void)verify:(NSString *)receipt withFlag:(BOOL)flag;
- (void)verifyOrderInformation:(BOOL)flag;
- (void)checkActiveStatus:(BOOL)flag;
- (void)activeCompleted:(NSString *)productId;
- (bool)isActived;
- (void)saveReceiptData:(NSDictionary *)receiptData;
- (NSDictionary *)getReceiptData;
- (void)removeLocReceiptData;
@end

static MyViewController *instance = nil;

@implementation MyViewController //the name of your view controller (same as above)
  //the code below will be added here
+(MyViewController *)getInstance
{
    if (instance == nil) {
        instance = [[MyViewController alloc]init];
    }
    return instance;
}

- (void) setActiveMgr:(ActiveMgr_macStore *)notified{
    m_ativeMgr=notified;
}
//If you have more than one in-app purchase, you can define both of
//of them here. So, for example, you could define both kRemoveAdsProductIdentifier
//and kBuyCurrencyProductIdentifier with their respective product ids
//
//for this example, we will only use one product

// put your product id (the one that we just made in App Store Connect) in here
#define kRemoveAdsProductIdentifier @"com.company.appName.month.auto"

- (void)activePro{
    NSLog(@"User requests to active pro version");
    auto tmp =m_ativeMgr->getIAP_ID();
    if(tmp.isEmpty()){
        NSLog(@"Get IAP ID is failed");
        return ;
    }
    NSString * productID=tmp.toNSString();
    NSLog(@"ProductID is %@",productID);

    if([SKPaymentQueue canMakePayments]){
        NSLog(@"User can make payments");

        //If you have more than one in-app purchase, and would like
        //to have the user purchase a different product, simply define
        //another function and replace kRemoveAdsProductIdentifier with
        //the identifier for the other product

        SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:productID]];
        productsRequest.delegate = self;
        [productsRequest start];

    }
    else{
        NSLog(@"User cannot make payments due to parental controls");
        //this is called the user cannot make payments, most likely due to parental controls
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    SKProduct *validProduct = nil;
    int count = [response.products count];
    NSLog(@"IAP count is %i",count);
    if(count > 0){
        validProduct = [response.products objectAtIndex:0];
        NSLog(@"Products Available!");
        [self purchase:validProduct];
    }
    else if(!validProduct){
        NSLog(@"No products available");
        //this is called if your product id is not valid, this shouldn't be called unless that happens.
    }
}

- (void)purchase:(SKProduct *)product{
    SKPayment *payment = [SKPayment paymentWithProduct:product];

    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void) restore{
    //this is called when the user restores purchases, you should hook this up to a button
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}
- (void)verifyOrderInformation:(BOOL)flag{
    NSURL *recepitURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receipt = [NSData dataWithContentsOfURL:recepitURL];
    NSString * receiptData = [receipt base64EncodedStringWithOptions:0];
    [self saveReceiptData:@{@"receipt":receiptData}];
#ifndef STOREKIT_TEST
    [self verify:receiptData withFlag:NO];
#else
    [self activeCompleted:m_ativeMgr->getIAP_ID().toNSString()];
#endif
}

- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    NSLog(@"received restored transactions:%lu",queue.transactions.count);
    for(SKPaymentTransaction *transaction in queue.transactions){
        if(transaction.transactionState == SKPaymentTransactionStateRestored){
            //called when the user successfully restores a purchase
            NSLog(@"Transaction state -> Restored");

            //if you have more than one in-app purchase product,
            //you restore the correct product for the identifier.
            //For example, you could use
            //if(productID == kRemoveAdsProductIdentifier)
            //to get the product identifier for the
            //restored purchases, you can use productID

            NSString *productID = transaction.payment.productIdentifier;
            
            NSLog(@"Restore product ID is %@",productID);
            [self verifyOrderInformation:NO];
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            break;
        }
    }
    if(0 == queue.transactions.count){
        emit m_ativeMgr->restorePurchaseFailed();
    }
    m_ativeMgr->emitCheckActiveComplte();
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
   NSLog(@"-------弹出错误信息----------");
   emit m_ativeMgr->verfiyComplete(false);
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"恢复error = %@",error);
    emit m_ativeMgr->restorePurchaseFailed();
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for(SKPaymentTransaction *transaction in transactions){
        //if you have multiple in app purchases in your app,
        //you can get the product identifier of this transaction
        //by using transaction.payment.productIdentifier
        //
        //then, check the identifier against the product IDs
        //that you have defined to check which product the user
        //just purchased

        NSString * productId=transaction.payment.productIdentifier;
        NSLog(@"Buy product identifier [ %@ ] transaction ",productId);

        switch(transaction.transactionState){
            case SKPaymentTransactionStatePurchasing:
                NSLog(@"Transaction state -> Purchasing,time is %lli",QDateTime::currentMSecsSinceEpoch());
                //called when the user is in the process of purchasing, do not add any of your own code here.
                break;
            case SKPaymentTransactionStatePurchased:
                {
                    //this is called when the user has successfully purchased the package (Cha-Ching!)
                    NSLog(@"Transaction state -> Purchased,time is %lli",QDateTime::currentMSecsSinceEpoch());
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    [ self verifyOrderInformation:NO];
                    
                }
                break;
            case SKPaymentTransactionStateRestored:
                {
                    NSLog(@"Transaction state -> Restored");
                    //add the same code as you did from SKPaymentTransactionStatePurchased here
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    [ self verifyOrderInformation:NO];
                }
                break;
            case SKPaymentTransactionStateFailed:
                {
                    //called when the transaction does not finish
                    if(transaction.error.code == SKErrorPaymentCancelled){
                        NSLog(@"Transaction state -> Cancelled");
                        //the user cancelled the payment ;
                    }

                    m_ativeMgr->emitActiveCancel();
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    NSLog(@"transaction error code is [ %li ]",transaction.error.code);
                }
                break;
            default:
                NSLog(@"Transaction state is [ %li ]",transaction.transactionState);
                break;
                
        }
    }
}

- (void)verify:(NSString *)receipt withFlag:(BOOL)flag{
        if(nil == receipt){
            emit m_ativeMgr->verfiyComplete(false);
            return;
        }
        
        NSLog(@"mainBundle is %@",[NSBundle mainBundle]);
         
        if(!receipt){
            NSLog(@"交易凭证为空验证失败");
            emit m_ativeMgr->verfiyComplete(false);
            return;
        }
        // 购买成功将交易凭证发送给服务端进行再次校验
    //    [self handleActionWithType:SIAPPurchSuccess data:receipt]; //todo
         
        NSError *error;
        NSDictionary *requestContents = @{
                                          @"receipt-data": receipt
                                          ,@"password":SHARED_SECRET.toNSString()
                                          };
        NSData *requestData = [NSJSONSerialization dataWithJSONObject:requestContents
                                                              options:0
                                                                error:&error];
         
        if (!requestData) { // 交易凭证为空验证失败
            NSLog(@"交易凭证为空验证失败");
            emit m_ativeMgr->verfiyComplete(false);
            return;
        }
         
        //In the test environment, use https://sandbox.itunes.apple.com/verifyReceipt
        //In the real environment, use https://buy.itunes.apple.com/verifyReceipt
         
        NSString *serverString = @"https://buy.itunes.apple.com/verifyReceipt";
        if (flag) {
            serverString = @"https://sandbox.itunes.apple.com/verifyReceipt";
        }
        NSURL *storeURL = [NSURL URLWithString:serverString];
        NSMutableURLRequest *storeRequest = [NSMutableURLRequest requestWithURL:storeURL];
        [storeRequest setHTTPMethod:@"POST"];
        [storeRequest setHTTPBody:requestData];
        
        NSURLSession *session = [NSURLSession sharedSession];
        
        NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:storeRequest completionHandler:^(NSData * __nullable data, NSURLResponse * __nullable response, NSError * __nullable error) {
                //拿到响应头信息
                NSHTTPURLResponse *res = (NSHTTPURLResponse *)response;

                //解析拿到的响应数据
                NSLog(@"%@\n%@",[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding],res.allHeaderFields);
            
            if (error) {
                NSLog(@"无法连接服务器,购买校验失败");
                emit m_ativeMgr->verfiyComplete(false);
            } else {
                
                NSError *error;
                NSDictionary *jsonResponse = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
                if (!jsonResponse) {
                    NSLog(@"苹果服务器校验数据返回为空校验失败");
                    emit m_ativeMgr->verfiyComplete(false);
                    return;
                }
                 
                // 先验证正式服务器,如果正式服务器返回21007再去苹果测试服务器验证,沙盒测试环境苹果用的是测试服务器
                NSString *status = [NSString stringWithFormat:@"%@",jsonResponse[@"status"]];
                if (status && [status isEqualToString:@"21007"]) {
                    [self verify:receipt withFlag:YES];
                }else if(status && [status isEqualToString:@"0"]){
                    emit m_ativeMgr->verfiyComplete(true);
                }
                else {
                    NSLog(@"校验返回错误码为 %@",status);
                    emit m_ativeMgr->verfiyComplete(false);
                }
    #if DEBUG
                NSLog(@"----验证结果 %@",jsonResponse);
    #endif
            }
            
            }];

        //刚创建出来的task默认是挂起状态的，需要调用该方法来启动任务（执行任务）
        [dataTask resume];
}

- (void)checkActiveStatus:(BOOL)flag{
#ifndef STOREKIT_TEST
    NSDictionary * orderInfo = [self getReceiptData];
    if (orderInfo != nil) {
        NSString *receipt = orderInfo[@"receipt"];
        
        [self verify:receipt withFlag:flag];
    }else{
        emit m_ativeMgr->verfiyComplete(false);
    }
#endif
}

- (void)activeCompleted:(NSString *)productId{
    emit m_ativeMgr->activeComplete(QString::fromNSString(productId));
    
//    //从沙盒中获取交易凭证并且拼接成请求体数据
//        NSURL *receiptUrl = [[NSBundle mainBundle] appStoreReceiptURL];
//        NSData *receiptData = [NSData dataWithContentsOfURL:receiptUrl];
//        NSString *receiptString = [receiptData base64EncodedStringWithOptions:0];
//
//    QFile tmp("base64_encodeed_receipt");
//    if(!tmp.open(QIODevice::WriteOnly | QIODevice::Text)){
//        return;
//    }
//    tmp.write(QString::fromNSString(receiptString).toLocal8Bit());
    
    [self setActiveStatus:YES];
}

-(void) setActiveStatus:(BOOL)isActive{
    long long value=isActive ? m_ativeMgr->calIdentifier():0;
    [[NSUserDefaults standardUserDefaults] setValue:[NSString stringWithFormat:@"%lld",value] forKey:IDENTIFIER];
    //use NSUserDefaults so that you can load whether or not they bought it
    //it would be better to use KeyChain access, or something more secure
    //to store the user data, because NSUserDefaults can be changed.
    //You're average downloader won't be able to change it very easily, but
    //it's still best to use something more secure than NSUserDefaults.
    //For the purpose of this tutorial, though, we're going to use NSUserDefaults
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (bool) isActived{
    NSString * identifier = [[NSUserDefaults standardUserDefaults] valueForKey:IDENTIFIER];
    [[NSUserDefaults standardUserDefaults] synchronize];
    auto expectIdentifier = m_ativeMgr->calIdentifier();
    return (QString::fromNSString(identifier).toLongLong() == expectIdentifier);
}

- (void)saveReceiptData:(NSDictionary *)receiptData
{
    [[NSUserDefaults standardUserDefaults] setValue:receiptData forKey:SAVED_RECEIPT_DATA];
    [[NSUserDefaults standardUserDefaults]synchronize];
}
 
- (NSDictionary *)getReceiptData
{
    return [[NSUserDefaults standardUserDefaults] valueForKey:SAVED_RECEIPT_DATA];
}
 
- (void)removeLocReceiptData
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:SAVED_RECEIPT_DATA];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

@end

ActiveMgr_macStore::ActiveMgr_macStore(QObject *parent):IActiveMgr(parent)
    , m_activeTypeMap{{"1-Month",QString::fromNSString(IAP_)}
                      ,{"1-Year","com.company.appName.auto.year"}
                      ,{"Lifetime","com.company.appName.lifetime"}}
{
     [ [MyViewController getInstance] setActiveMgr:this ];
     connect(this,&ActiveMgr_macStore::activeComplete,this,&ActiveMgr_macStore::onActiveComplete,Qt::QueuedConnection);
     connect(this,&ActiveMgr_macStore::verfiyComplete, this, &ActiveMgr_macStore::onVerfiyComplete,Qt::QueuedConnection);
}

ActiveMgr_macStore::~ActiveMgr_macStore()
{

}

void ActiveMgr_macStore::beginActive(QString sn)
{
    Q_UNUSED(sn);
//    // 先检测客户端与服务器的漏单情况
//    [[MyViewController getInstance] checkActiveStatus:NO];
//    m_needBuy=true;
//#ifdef STOREKIT_TEST
//    emit verfiyComplete(false);
//#endif
    [[MyViewController getInstance] activePro];
}

QString ActiveMgr_macStore::getSn()
{
    return "";
}

// 针对mac 应用市场版本，不再需要硬件码
QString ActiveMgr_macStore::getHardCode()
{
    return "";
}

bool ActiveMgr_macStore::isPro()
{
    return [ [MyViewController getInstance] isActived];
}

bool ActiveMgr_macStore::islifeTime()
{
    return m_activeTypeMap["Lifetime"]==m_productId;
}

void ActiveMgr_macStore::restorePurchase()
{
    [ [MyViewController getInstance] restore ];
}

bool ActiveMgr_macStore::CheckActive()
{
    return isPro();
}

bool ActiveMgr_macStore::IsActived()
{
    return CheckActive();
}

bool ActiveMgr_macStore::IsValidActived()
{
    return m_valid_active;
}

void ActiveMgr_macStore::SyncServer()
{
    // 定时校验订单，来检测订单是否有效
    [[MyViewController getInstance] checkActiveStatus:NO];
}

QString ActiveMgr_macStore::getIAP_ID()
{
    auto search = m_activeTypeMap.find(getActiveTypeStr());
    if(search != m_activeTypeMap.end()){
        return search->second;
    }
    return "";
}

void ActiveMgr_macStore::onActiveComplete(const QString &productID)
{
    m_productId=productID;
    qCDebug(macStoreActiveMgrLogCategory)<<"m_productId is"<<m_productId;
    emit activeFinish(0LL);
}

void ActiveMgr_macStore::onVerfiyComplete(bool ok){
    if(ok && !isPro()){
        [[MyViewController getInstance] activeCompleted:getIAP_ID().toNSString()];
    }
    if(!ok){
       [[MyViewController getInstance] setActiveStatus:NO];
        if(m_needBuy){
            [[MyViewController getInstance] activePro];
            m_needBuy=false;
        }else{
            emit activeCancel();
        }
    }
    qCInfo(macStoreActiveMgrLogCategory)<<"Verfiy complete,and result is"<<ok;
}

void ActiveMgr_macStore::emitActiveCancel()
{
    emit activeCancel();
}

void ActiveMgr_macStore::emitCheckActiveComplte()
{
    emit checkActiveComplete();
}

long long ActiveMgr_macStore::calIdentifier() {
    extern std::string get_platform_uuid();
    auto uuid = get_platform_uuid();
    long long ret=0;
    for(auto i=0u;i<uuid.size();++i){
        ret+=uuid[i];
    }
    // other Method
    return ret;
}
```

### 遇到的问题

####  校验订单，但校验结果一直返回 21003 
最开始怀疑是代码不正确、机器异常、沙盒测试账号有异常，但实际上是发送校验请求有误（oops）。解决办法的链接可参见 

["The receipt could not be authenticated" - should it be checked again?](https://stackoverflow.com/questions/58615404/the-receipt-could-not-be-authenticated-should-it-be-checked-again)

#### Could not find the Qt platform plugin "cocoa" in ""

在 qmake 工程中添加内购流程后一切正常，但使用 qmake 装换成 xcode 工程后调试，一直提示 
```
Could not find the Qt platform plugin "cocoa" in ""
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.
```

解决方法：安装 *.app/Contents/embedded.provisionprofile 文件即可，该文件的一些信息可见
- [细谈证书与Provisioning Profile](https://www.jianshu.com/p/abd3c55db48e)

#### 使用真实账号登陆沙盒测试账号购买异常
（这是正确的，oops），详见[【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)

#### 偶现程序已启动就进入到  paymentQueue
```cpp
(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
				...
}
```
若上次的 Transaction 未调用 
```cpp
[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
```
就会出现该情况

### 从IAP获取内购的价格

```objectivec
bool g_checkProductPrice=false;

- (void)checkProductPrice{
    if([SKPaymentQueue canMakePayments]){
        NSLog(@"User can make payments");

        //If you have more than one in-app purchase, and would like
        //to have the user purchase a different product, simply define
        //another function and replace kRemoveAdsProductIdentifier with
        //the identifier for the other product

        g_checkProductPrice=true;
        NSArray * array=[NSArray arrayWithObjects:@"com.company.appName.auto.month",@"com.company.appName.auto.year",nil];
        SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithArray:array]];
        productsRequest.delegate = self;
        [productsRequest start];

    }
    else{
        NSLog(@"User cannot make payments due to parental controls");
        //this is called the user cannot make payments, most likely due to parental controls
    }
};
```

### 相关链接
- [iOS Handle Refunds 处理退款 --- WWDC20（Session 10661） ](https://juejin.cn/post/6845166890420011022)
- [ 【WWDC20】 10659 - 介绍 Xcode 中 StoreKit 测试 ](https://juejin.cn/post/6857502956317736968)
- [Apple Developer Documentation](https://developer.apple.com/documentation/storekit/original_api_for_in-app_purchase/subscriptions_and_offers/handling_subscriptions_billing?language=objc)
- [ios - 如何在应用重启或重新安装时检查iOS中的订阅是否处于事件状态](https://www.coder.work/article/7152298)
- [Overview for configuring in-app purchases](https://help.apple.com/app-store-connect/#/devb57be10e7)
- [How do you add an in-app purchase to an iOS application?](https://stackoverflow.com/questions/19556336/how-do-you-add-an-in-app-purchase-to-an-ios-application)
- [In-App Purchase Tutorial: Getting Started](https://www.raywenderlich.com/5456-in-app-purchase-tutorial-getting-started)
- [In-App Purchase（iap）快速指南](https://www.cnblogs.com/kris888/archive/2012/09/21/2696754.html)
- [Get Shared Secret](https://admost.github.io/amrapi/subscription/apple/shared-secret/)
- [Apple App Store Receipt Checker - RevenueCat](https://www.revenuecat.com/apple-receipt-checker)
- [App 内购买项目 - Apple Developer](https://developer.apple.com/cn/in-app-purchase/)

## 支持首用户免费使用一段时间

可参考 [在 App 中实现推介促销优惠 - 简体中文文档 - Apple Developer](https://developer.apple.com/cn/documentation/storekit/in-app_purchase/subscriptions_and_offers/implementing_introductory_offers_in_your_app/)

## 修复 “appstore 版本开启沙盒”后无法让多进程通信

主程序使用原 权利文件进行签名

需要对子程序单独使用 权利文件进行签名，使用这个：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
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

# 参考链接

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
- [ios app真机测试到上架App Store详细教程-必看](http://blog.applicationloader.net/blog/zh/88.html)
- [App Store 审核指南](https://developer.apple.com/cn/app-store/review/guidelines/)
- [【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)

---


- [上一级](README.md)
- 下一篇 -> [mac 开发资料](macDevelopSome.md)
