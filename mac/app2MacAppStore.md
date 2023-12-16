<!-- 上架 mac 应用程序到苹果商店  -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [上架 mac 应用程序到苹果商店](#上架-mac-应用程序到苹果商店)
  - [证书](#证书)
  - [密钥](#密钥)
  - [标识符](#标识符)
  - [App Store 审核指南](#app-store-审核指南)
    - [简介](#简介)
    - [提交之前](#提交之前)
    - [安全](#安全)
    - [性能](#性能)
    - [商务](#商务)
    - [设计](#设计)
    - [法律](#法律)
    - [提交之后](#提交之后)
- [支持应用内购](#支持应用内购)
    - [遇到的问题](#遇到的问题)
    - [从IAP获取内购的价格](#从iap获取内购的价格)
  - [参考链接](#参考链接)
  - [支持首用户免费使用一段时间](#支持首用户免费使用一段时间)
  - [修复 “appstore 版本开启沙盒”后无法让多进程通信](#修复-appstore-版本开启沙盒后无法让多进程通信)
  - [adding login items for macos](#adding-login-items-for-macos)
- [链接](#链接)

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

### 简介

App Store 中的每个 app 都会经过专家审核，而且还有编辑团队每天帮助广大用户发现新的 app。

请将以下几点谨记在心：

- 很多儿童会从我们这里大量下载各种 app。尽管家长控制功能能为儿童提供有效保护，但您也必须做好自己份内的工作。因此，您要知道，我们时刻都在关注这些儿童。
- 在 App Store 上发布的所有观点，我们都非常支持 — 只要这些 app 尊重用户的不同意见，并能带来良好的 app 体验。如果我们认为 app 的任何内容或行为超出了可接受的范围，我们将拒绝该 App。
- 如果您试图欺骗系统 (例如，试图在审核流程中弄虚作假，窃取用户数据，抄袭其他开发者的作品，操纵评分或 App Store 上的发现方法)，我们会从商店中移除您的 app，并将您从 Developer Program 中除名。
- 您要确保 app 中所含内容全部符合这些准则的要求，包括广告网络、分析服务和第三方 SDK 等；因此，在审核和选择这些内容时务必要慎重

### 提交之前

请确保：

- 测试 app 是否会发生崩溃、是否存在错误
- 确保所有 app 信息及元数据完整且正确
- 更新您的联系信息，以便 App Review 团队在需要时与您取得联系
- 提供有效的演示帐户和登录信息，以及审核 app 时所需的任何其他硬件或资源 (例如，登录凭证或示例二维码)
- 启用后台服务，以使其在审核期间处于活动和可用状态
- 在 App Review 备注中附上与非明显功能及 App 内购买项目相关的详细说明，包括支持文稿 (如适用)
- 检查 app 是否遵循了其他文稿中的相关指南，如
    - **开发指南**
        - [App Store Connect 帮助](https://help.apple.com/app-store-connect/?lang=zh-cn#/)
        - [开发者帐户帮助](https://help.apple.com/developer-account/?lang=zh-cn#/)
    - **设计指南**
        - [Human Interface Guidelines (英文)](https://developer.apple.com/design/human-interface-guidelines/)
    - **品牌和营销指南**
        - [营销资源和识别标志指南](https://developer.apple.com/cn/app-store/marketing/guidelines/)
        - [Apple Pay 营销准则](https://developer.apple.com/cn/apple-pay/marketing/)
        - [“添加到 Apple 钱包”指南](https://developer.apple.com/cn/wallet/add-to-apple-wallet-guidelines/)
        - [Apple 商标及版权使用准则 (英文)](https://www.apple.com.cn/legal/intellectual-property/guidelinesfor3rdparties.html)
    

### 安全

当用户通过 App Store 安装 app 时，他们希望获得安全的体验：app 不含令人不快或具有攻击性的内容，不会损坏他们的设备，不会在使用中造成人身伤害。我们在下方列出了主要的安全隐患。如果您想恐吓、攻击他人，则您的 app 不适合出现在 App Store 中。

- App 不应包含具有攻击性、不顾及他人感受、令人不安、惹人厌恶、低俗不堪或只是让人感到毛骨悚然的内容。
- 对于包含用户生成内容的 App，有特定的难题需要解决，比如知识产权侵权、匿名欺凌等。
- “儿童类别”可帮助用户轻松找到专为儿童设计的 app。如果您希望参与“儿童类别”，则应该致力于为年纪较小的用户量身打造卓越的使用体验。这些 app 不得提供 app 外链接、购买机会或其他会对儿童造成干扰的内容，除非其保留在受家长监控的指定区域中。请谨记，一旦顾客认为您的 app 能够满足“儿童类别”要求，您的 app 就需要一直满足后续更新中的相应准则；即使您决定取消选择此类别，也是如此。
- 如果 app 的行为方式可能会造成人身伤害，苹果官方可能会拒绝该 app。
- 用户需要知道如何就疑问和支持问题与您取得联系。确保您的 app 及其支持 URL 中包含能轻松联系到您的联系信息；
- App 应实施适当的安全举措，确保按照“Apple Developer Program 许可协议”和这些准则 (更多信息见 todo - “准则 5.1”) 妥善处理收集到的用户信息，防止对这些信息进行未经授权使用、披露或者被第三方访问。

### 性能

- App 完成度
    - 提交至 App Review 的申请 (包括可供预订的 app) 应为该 app 的最终版本，并应包含所有必要的元数据和有效网址。所有占位符文本、空白网站和其他临时内容应在提交前移除。在提交 app 之前，请务必在设备上对 app 的错误和稳定性进行测试；如果您的 app 需要登录，请提供演示帐户信息 (并打开您的后台服务！)。如果您在 app 中提供了 App 内购买项目，请确保审核人员能够看到这些内容，并确保这些内容处于完整且最新的状态，否则请在审核备注中说明相关原因。
- 准确的元数据
    
    顾客在下载或购买您的 app 时，应该知道他们将得到什么，所以请确保您的所有 app 元数据，包括隐私信息、app 描述、截屏和预览，能够准确反映 app 的核心体验；并请记得更新，以保持与新版本相应的最新状态。
    
    - 请勿在 app 中包含隐藏、休眠或未记录的功能；不管是对于终端用户还是 App Review 团队，app 功能都应清晰可见。所有新的特性、功能和产品变更内容都必须在 App Store Connect 的“审核备注”部分中予以详细描述 (笼统的描述会导致 app 被拒绝)，并且可供审核。同样，您不应该在 App Store 或离线情况下，营销您的 app 中实际并不提供的内容或服务 (例如基于 iOS 的病毒和恶意软件扫描工具)。
    - 如果您的应用包含 App 内购买项目，请确保应用的描述、截屏和预览清楚地指明是否有需要另行购买的精选项目、关卡和订阅等。如果您决定在 App Store 中推广 App 内购买项目，请确保 App 内购买项目的显示名称、截屏和描述适合所有公众，并遵循“[推广您的 App 内购买项目](https://developer.apple.com/cn/app-store/promoting-in-app-purchases/)”中的准则；此外，您的 app 也应正确使用 [SKPaymentTransactionObserver 方法 (英文)](https://developer.apple.com/documentation/storekit/skpaymenttransactionobserver/2877502-paymentqueue)，以便顾客在 app 内无缝完成购买。
    - 截屏应展示 app 的使用情况，而非仅显示标题封面、登录页面或初始屏幕。截屏还可以包括文本及图像说明 (例如：演示输入机制，如触控点或 Apple Pencil 的动画)，并展示设备上的扩展功能，如触控栏。
    - 预览是让顾客了解 app 外观和功能的绝佳方式。为了确保顾客清楚他们将在 app 中获得的体验，预览只可使用从 app 中采集的视频屏幕。Stickers 和 iMessage 信息扩展可以将用户体验展示在“信息”app 中。您也可以添加旁白和视频，或添加文本说明，以帮助说明任何无法仅通过视频进行阐明的内容。
    - 请为 app 选择最适合的类别，并在需要帮助时参考“[App Store 类别定义](https://developer.apple.com/cn/app-store/categories/)”。
    - 请在 App Store Connect 中诚实地回答年龄分级问题，以使 app 与家长控制功能的分级保持一致。
    - 请选择一个独特的 app 名称，指定能够准确描述 app 的关键词，不要试图用商标术语、流行 app 的名称、定价信息或其他不相关的短语来包装任何元数据，以此欺骗系统。App 名称必须限制在 30 个字符以内。App 名称、副标题、截屏和预览等元数据不得包含并非对应该元数据的价格、词语或描述。App 副标题是详细介绍 app 背景信息的绝佳之处。副标题必须遵循我们的标准元数据规则，且不得包含不当内容、提及其他 app 或做出无法证实的产品声明。Apple 可以随时修改不合适的关键字或采取其他相应步骤，以防止不当使用。
    - 元数据应适合所有受众，所以请确保您的 app 和 App 内购买项目的相关图标、截屏和预览保持在 4+ 年龄分级；即使您的 app 分级更高，也应如此。请务必确保包括 app 名称和图标 (小图标、大图标、Apple Watch app 和备用图标等) 在内的元数据彼此相似，以免引起困惑。
    - 您应负责确保有权使用 app 图标、截屏和预览中的所有材料，并应显示虚构的帐户信息，而非真实个人的数据。
    - 请确保您的 app 注重 iOS、Mac、Apple TV 或 Apple Watch 体验，并且不在 app 或元数据中包含其他移动平台的名称、图标或图像，除非存在已获批的特定互动功能。确保您的 app 元数据注重 app 本身及其体验。不要包含无关的信息，包括但不限于关于 Apple 或开发流程的信息。
    - 您提交至 App Store 可供预订的 app 必须为完整且可发布的状态。请确保您最终发布的 app 与其在预购状态时所宣传的内容没有实质性差异。如果您对该 app 进行了重大更改 (例如更改其商业模式)，则应重新开始其预订销售。
    - App 必须在其“新功能”文本中清楚地描述新功能和产品更改情况。一些简单的错误修复、安全更新和性能改进可以通过一般描述来说明，但较为重大的更改必须列明在备注中。
- 硬件兼容性
    - App 不得建议或要求重新启动设备，或者修改与 app 核心功能无关的系统设置。例如，请勿鼓励用户关闭 Wi-Fi 或停用安全功能等。
    - 对于通过 Mac App Store 分发的 app，还有几个额外要求需要您留意：
        - 这些 app 必须妥当地沙盒化，并遵循“[macOS 文件系统文档 (英文)](https://developer.apple.com/library/archive/documentation/FileManagement/Conceptual/FileSystemProgrammingGuide/Introduction/Introduction.html#//apple_ref/doc/uid/TP40010672)”。另外，这些 app 只应使用相应的 macOS API 来修改其他 app 存储的用户数据 (如书签、“地址簿”或“日历”条目)。
        - 这些 app 必须使用 Xcode 中提供的技术来进行打包和提交；不允许使用第三方安装器。另外，这些 app 必须是单个的自包含 app 安装包，不能将代码或资源安装在共享位置。
        - 这些 app 不得自动启动或者在启动时包含其他自动运行的代码，不得在未经同意的情况下登录，也不得大量生成在用户退出 app 后仍在未经同意的情况下继续运行的进程。这些 app 不得将图标自动添加到程序坞中，或在用户桌面上留下快捷方式。
        - 这些 app 不得下载或安装独立的 app、kext、额外代码或资源，以向我们在审核过程中看到的 app 添加功能，或进行大幅更改。
        - 这些 app 不得申请升级至 root 特权或使用 setuid 属性。
        - 这些 app 不得在启动时显示许可证屏幕、需要使用许可证密匙或实施自己的拷贝保护措施。
        - 这些 app 必须使用 Mac App Store 分发更新；不允许使用其他更新机制。
        - 这些 app 应在当前发布的 OS 上运行，不得使用已停用或选装的技术 (如 Java)。
        - 这些 app 必须在单个 app 套装内包含所有的语言和本地化支持。
        
    - 软件要求
        - App 仅可使用公共 API，并且必须在当前发布的 OS 上运行。进一步了解 [public APIs](https://developer.apple.com/cn/documentation/)。及时更新您的 app，在未来的操作系统版本中不再支持的任何过时功能、框架或技术皆应被淘汰。App 使用的 API 和框架应该是为了实现预期用途，并在 app 描述中说明集成详情。
        - App 应自包含在自己的套装中，不得在指定容器范围外读取或写入数据，也不得下载、安装或执行会引入或更改 app 特性或功能的代码，包括其他 app。仅在特殊情况下，用于教授、开发或允许学生测试可执行代码的教育类 app 可以下载所提供的代码，但这类代码不得用于其他用途。这类 app 必须开放 app 提供的源代码，让顾客可以完全查看和编辑这些源代码。
        - 如果 app 传输的病毒、文件、计算机代码或程序会对操作系统和/或硬件功能 (包括推送通知和 Game Center) 的正常运行造成负面影响或导致其中断，则该 app 会被拒绝。
        - 多任务处理 app 只允许在实现预期用途时使用后台服务：VoIP、音频播放、地理位置、任务完成记录和本地通知等。如果应用使用定位后台模式，请提醒用户，这么做会大幅降低电池续航能力
        - App 必须能够在仅支持 IPv6 的网络上完全正常地运作。
        - 如果 app 会浏览网页，则必须使用相应的 WebKit 框架和 WebKit Javascript。
        - 如果 app 会创建替代的桌面/主屏幕环境，或者模拟多 app 插件体验，则该 app 会遭到拒绝。
        - 如果 app 会改变或停用标准开关 (如调高/调低音量和铃声/静音开关) 的功能，或者改变或停用其他的原生用户界面元素或行为，则该 app 会遭到拒绝。例如，app 不应屏蔽转向其他 app 的链接，或用户希望以某种特定方式运行的功能。进一步了解如何正确处理[链接](https://developer.apple.com/cn/design/)。
        - 不得提交包含空白广告横幅或测试广告的 app
        - 在录像、记录日志或以其他方式记录用户活动时，app 必须征得用户的明确同意，而且要提供清晰的视觉和/或听觉指示。这亦包括任何对设备摄像头、麦克风、屏幕录像或其他用户输入方式的使用。
        - 轻 App、小组件、扩展和通知应当与 app 的内容和功能相关。此外，所有的轻 App 特性和功能都必须包含在主 app 二进制文件中。轻 App 不能包含广告

### 商务

在 App Store 中，您可以通过多种方式让自己的 app 实现盈利。

如果我们发现您试图操纵评价，通过付费、提供奖励、经过筛选或伪造的反馈来提高排名，或者要求第三方服务代您这样做，我们将采取相应措施以保持 App Store 的公正性，其中可能包括将您从 Apple Developer Program 中除名。

- 付款
    - App 内购买项目
    - 订阅
    - 其他购买方式：以下 app 可以使用 App 内购买项目以外的购买方式。此部分中的 app 不得在 app 内或通过向用户在 app 内注册帐户时提供的联系方式发送通信内容 (如电子邮件或文本)，来鼓励用户使用 App 内购买项目以外的购买方式。
        - “阅读器”类型的 App
        - 多平台服务：跨平台运行的 app 可以允许用户访问他们在其他平台上的相应 app 中或您的网站上获取的内容、订阅或功能，包括多平台游戏中的消耗品，前提是这些项目也在 app 中以 App 内购买项目的形式提供。
        - 企业服务
        - 一对一服务
        - App 之外的商品和服务
        - 免费的独立 App
    - 硬件相关内容
    - 加密货币
    - **Apple Pay：**
        
        如果 app 使用 Apple Pay，则在销售任何商品或服务之前，必须先向用户提供所有的基本购买信息，并且必须正确使用 Apple Pay 品牌和用户界面元素，具体要求可参考“[Apple Pay 识别标志指南](https://developer.apple.com/cn/apple-pay/marketing/)”和“[Human Interface Guidelines (英文)](https://developer.apple.com/design/human-interface-guidelines/apple-pay/)”。使用 Apple Pay 提供重复付款服务的 App 至少需要披露以下信息：
        
        - 续订周期的时长；除非被取消，否则续订将会继续
        - 每个周期中会提供哪些服务
        - 将向顾客收取的实际费用
        - 如何取消
    - 广告
- 其他业务模式问题（详见官方文档）
    - 可以接受
    - 不可以接受
        - App 应当允许用户直接获得付费购买的项目而无需执行额外的任务，如在社交媒体上发帖、上传通讯录，以及在 app 内签到特定次数等。App 不得要求用户必须先为 app 评分或点评、观看视频、下载其他 app、点击广告、启用跟踪功能或进行其他类似操作，然后才能访问 app 的功能、内容或使用 app，或者收到现金或其他补偿 (包括但不限于礼品卡和代码)

### 设计

- 抄袭者
    
    请拿出您自己的想法。我们知道您有自己的奇思妙想，那么请将它们付诸实际。请不要简单照搬 App Store 上的热门 app，或只是细微修改其他 app 的名称或 UI，就将其挪为己用。这么做不但有引发知识产权侵权索赔的风险，更会加剧在 App Store 中浏览的难度，并对您的开发者同仁来说也很不公平。
    
- 最低功能要求 - App 应包含功能、内容和 UI，而不仅仅是一个经过重新包装的网站。
- 重复 App
    - 请不要为同一个 app 创建多个套装 ID。如果您的 app 打算为特定位置、运动队、大学等提供不同版本，请考虑只提交一个 app，并使用 App 内购买项目来提供不同的功能。同时，请避免继续在已有大量类似 app 的类别下进行开发；App Store 上已经有太多模拟放屁、打嗝声音的 app，以及手电筒、算命、约会和爱经等 app。除非这类 app 会提供独特、高质量的体验，否则我们将会予以拒绝。上传大量相似版本 app 的开发者会遭到 Apple Developer Program 的除名。
- **扩展**
    - 托管或包含扩展的 app 必须遵循“[App 扩展编程指南 (英文)](https://developer.apple.com/library/archive/documentation/General/Conceptual/ExtensibilityPG/index.html#//apple_ref/doc/uid/TP40014214)”或“[Safari 浏览器 App 扩展指南 (英文)](https://developer.apple.com/documentation/safariservices/safari_app_extensions)”；如果可行，还应包含诸如帮助屏幕和设置界面在内的一系列功能。您应当在 app 的市场营销文本中清晰且准确地披露提供了哪些扩展，扩展中不可包含营销、广告或 App 内购买项目。
- Apple 站点和服务
    - App 不得将推送通知列为必需条件，并且不应将这项功能用来发送敏感的个人或机密信息。推送通知不得用于促销或直接营销目的，除非用户已通过 app UI 中显示的同意语句明确选择接收此类信息，并且您在 app 中提供了让用户可以选择不接收此类信息的方法。不当使用这些服务可能会导致撤销您的权限。
- 备选 App 图标
- HTML5 游戏与聊天机器人 (Bot) 等
- 通过 Apple 登录
- 流媒体游戏

### 法律

只要 app 向某个地区的用户提供，那么就必须遵守该地区的所有法律要求。

- 隐私

在 Apple 生态体系中，保护用户隐私总是第一要务。在处理个人数据时，请务必小心谨慎，确保您已遵守[隐私保护最佳做法](https://developer.apple.com/cn/documentation/uikit/protecting_the_user_s_privacy)、适用的法律和“[Apple Developer Program 许可协议 (英文)](https://developer.apple.com/terms/)”中的条款，并符合顾客的预期。尤其是：

- **数据收集和存储**
    - **隐私政策：**所有 app 必须在 App Store Connect 元数据栏位和 app 内部包含可轻松访问的隐私政策链接。隐私政策必须明确而清楚地：
        - 指明 app/服务所收集的数据 (若有)、收集数据的方式，以及这些数据的所有用途。
        - 确认与 app 共享用户数据 (遵从这些准则) 的任何第三方 (例如，分析工具、广告网络和第三方 SDK，以及能够访问用户数据的任何母公司、子公司或其他相关实体) 会提供与 app 隐私政策所述及这些准则所要求相同或等同的用户数据保护措施。
        - 解释数据保留/删除政策，并且说明用户可以如何撤销同意和/或请求删除用户数据。
    - **许可** 如果 app 会收集用户数据或使用数据，即使此类数据在收集当时或收集后即刻被匿名处理，app 也必须征得用户的同意才能收集。付费功能不得依赖于或要求用户授予访问这些数据的权限。App 也必须为顾客提供简单易懂且易于操作的方式来撤销同意。确保您在用途说明中清楚且完整地阐述您对数据的使用。如果 app 依据欧盟《一般数据保护条例》(“GDPR”) 或类似法规，出于合法权益而不经事先同意就收集数据，则必须遵循此类法律的所有条款。进一步了解[请求许可](https://developer.apple.com/cn/documentation/uikit/protecting_the_user_s_privacy)。
    - **数据最少化：**App 仅可请求访问与 app 核心功能相关的数据，并且仅可收集和使用完成相关任务所需的数据。若有可能，请使用进程外选取器或共享列表，而不要请求“照片”或“通讯录”等受保护资源的完整访问权限。
    - **访问权限：**App 必须尊重用户的权限设置，不得操纵、欺骗或强迫用户同意不必要的数据访问。例如，可发布照片到社交网络的 app 不得在允许用户上传照片前要求麦克风访问权限。若有可能，请为不同意的用户提供替代解决方案。例如，如果用户拒绝共享位置，请提供手动输入地址的功能。
    - **帐户登录：**如果 app 不包含基于帐户的重要功能，请允许用户在不登录的情况下使用。App 不得要求用户提供个人信息才能正常使用，除非个人信息与 app 的核心功能直接相关，或是法律要求时。如果您的核心 app 功能与特定的社交网络 (如 Facebook、微信、微博或 Twitter 等) 不相关，您必须提供无需登录或其他类似机制的访问权限。调取基本档案信息、分享到社交网络或邀请朋友使用 app 等不视为核心 app 功能。App 还必须包含用于撤销社交网络凭证的机制，以及从 app 内停用 app 与社交网络之间数据访问的机制。App 不可在设备外存储社交网络的凭证或令牌，而且只能使用此类凭证或令牌来在 app 使用期间从 app 本身直接连接社交网络。
    - ****如果开发者开发的 app 试图暗中收集用户密码或其他用户私人数据，那么开发者会被从 Apple Developer Program 中除名。
    - 必须使用 SafariViewController 在显著位置向用户显示信息；不得隐藏这个控制器，也不能被其他视图或图层遮挡。此外，未经用户的知情和同意，app 不得私下利用 Safari 浏览器 ViewController 来跟踪用户。
    - 汇编个人信息的 app，如果其来源未经用户的明确同意，或并非直接源自用户 (即使是公共数据库)，则不可在 App Store 中发布。
    - 在受到严格监管的领域 (如银行和金融服务、医疗保健、赌博和航空旅行) 提供服务或需要敏感用户信息的 app，应由提供相应服务的法律实体提交，而不能由个人开发者提交。
- **数据使用和共享**
    - 除非法律另有许可，否则您不得未经他人允许而使用、传输或共享他们的个人数据。您必须提供相应的信息，说明以何种方式在哪里使用这些数据。App 收集的数据只有在为了改进 app 或用于广告投放用途 (遵守 [Apple Developer Program 许可协议 (英文)](https://developer.apple.com/terms/)) 的前提下，才能与第三方共享。您必须通过 App Tracking Transparency API 征得用户明确许可后，才能跟踪他们的活动。进一步了解[跟踪](https://developer.apple.com/cn/app-store/user-privacy-and-data-use/)。如果 app 在未经用户同意或未能符合数据隐私保护法律的情况下共享用户数据，则 app 可能会被下架，并且可能会导致您从 Apple Developer Program 中除名。
    - ****除非法律另有明确许可，否则未经用户的额外同意，为一个用途而收集的数据不可用于其他用途。
    - ****App 不得试图暗中基于收集的数据构建用户资料，也不得尝试、协助或鼓励他人根据从 Apple 提供的 API 收集的数据，或您所谓以“匿名”、“汇总”或其他不可识别的方式收集的数据来识别匿名用户的身份或重建用户资料。
    - 请勿使用来自“通讯录”、“照片”或能访问用户数据的其他 API 的信息来构建联系人数据库，以供自己使用或出售/分发给第三方，也不要收集关于用户设备上安装有哪些 app 的信息，以用于分析或投放广告/市场营销。
    - 请勿使用通过“通讯录”或“照片”收集的信息来联系用户，除非用户以个人方式明确主动发起联系；请勿包含“全部选择”选项，也不要默认选中所有联系人。在信息发送之前，您必须向用户清楚说明这个信息会如何呈现给收件人 (例如，信息中包含什么内容？发件人显示为谁？)。
    - 从 HomeKit API、HealthKit、临床健康记录 API、MovementDisorder API、ClassKit 或深度图和/或面谱绘制工具 (例如 ARKit、相机 API 或照片 API) 收集的数据，不得用于市场营销、投放广告或基于使用情况进行其他数据挖掘，包括第三方在内。进一步了解实施 [CallKit (英文)](https://developer.apple.com/documentation/callkit)、[HealthKit (英文)](https://developer.apple.com/documentation/healthkit)、[ClassKit (英文)](https://developer.apple.com/documentation/classkit) 和 [ARKit](https://developer.apple.com/cn/documentation/arkit) 的最佳做法。
    - 使用 Apple Pay 的 app 只能与第三方共享通过 Apple Pay 获得的用户数据，以帮助或改进商品或服务的交付。
- **健康和健康研究**
    
    健康、健身和医疗数据特别敏感，涵盖这些领域的 app 必须符合额外的规定，以确保顾客隐私受到保护：
    
    - App 在健康、健身和医疗研究背景下收集的数据 (包括从临床健康记录 API、HealthKit API、“运动与健身”、MovementDisorderAPI 或健康领域人体研究中收集的数据)，不得因广告投放、市场营销或其他基于使用情况进行的数据挖掘 (除非获得批准，并出于改进健康管理或健康研究的目的) 而使用或向第三方披露。不过，App 可以使用用户的健康或健身数据直接向该用户提供权益 (如保费减免)，前提是 app 须由提供相应权益的实体提交，而且其数据不得与第三方共享。同时，app 必须清楚说明将从设备收集的具体健康数据。
    - App 不得将虚假或错误数据写入 HealthKit 或其他任何医疗研究/健康管理 App，不得在 iCloud 中存储个人健康信息。
    - 开展健康领域人体研究的 app 必须获得参与人员提供的知情同意书，如果涉及未成年人，则必须获得由其家长或监护人提供的知情同意书。上述知情同意书必须涵盖以下内容：(a) 研究的性质、目的和时长；(b) 具体规程，给参与人员带来的风险和益处；(c) 关于保密和数据处理 (包括与第三方共享信息的情况) 的信息；(d) 用于回答参与人员问题的联系人；以及 (e) 退出流程。
    - 用于开展健康领域人体研究的 app 必须获得一家独立伦理审查委员会的批准。一经要求，必须提供此类批准的证明。
- **5.1.4 儿童**
    
    出于诸多原因，您在处理儿童的个人数据时请务必小心谨慎。我们建议您仔细阅读所有要求，以遵循相关法律，如《儿童在线隐私保护法》(“COPPA”)、欧盟《一般数据保护条例》(“GDPR”) 以及任何其他适用的法律法规。
    
    App 只能出于遵守适用儿童隐私法规的目的要求用户提供出生日期或家长联系信息，但必须提供一些适用于各年龄层用户的实用功能或娱乐价值。
    
    主要面向儿童的 app 不应包含第三方数据分析或第三方广告。这些做法可为儿童提供更安全的体验。在少数情况下，可能允许包含第三方数据分析和第三方广告，前提是这些服务遵守上文“[准则 1.3](https://developer.apple.com/cn/app-store/review/guidelines/#1.3)”中所述的条款。
    
    此外，“儿童类别”中的 app，以及向未成年人收集个人信息 (例如姓名、地址、电子邮件、位置、照片、视频、图画、能否聊天、其他个人数据，或是将永久标识符与以上任何信息组合使用)、传输此类信息或能够共享此类信息的 app，则必须拥有隐私政策，且必须遵守适用的儿童隐私保护法规。为了清楚起见，“儿童类别”的[家长监控要求](https://developer.apple.com/cn/app-store/review/guidelines/#kids-category)，通常并不完全等同于在这些隐私法规下征得家长的同意后收集个人数据。
    
    特此提醒，“[准则 2.3.8](https://developer.apple.com/cn/app-store/review/guidelines/#2.3.8)”要求只有“儿童”类别的 app 才能在元数据中使用类似“适合幼儿”和“适合儿童”等词语。不属于“儿童”类别的 app 不得在 app 名称、副标题、图标、截屏或描述中包含任何暗示 app 主要受众为儿童的词汇。
    
- **定位服务**
    
    只有在定位服务与 app 提供的功能和服务直接相关时，才能在 app 中使用定位服务。基于位置的 API 不得用于提供紧急服务，不得对汽车、飞机和其他设备进行自主控制 (小型设备，如轻量无人机和玩具除外)，不得遥控汽车防盗系统等。在收集、传输或使用位置数据之前，务必进行通知并获得用户同意。如果 app 会使用定位服务，请务必在 app 中说明相应的原因；请参考“[Human Interface Guidelines (英文)](https://developer.apple.com/design/human-interface-guidelines/ios/app-architecture/requesting-permission/)”，了解相应的最佳做法。
    
- **知识产权**
    
    请确保 app 只包含由您创建或拥有使用许可的内容。如果您已越线并在未经许可的情况下使用了内容，您的 app 可能会被移除。当然，这也意味着如果他人抄袭了您的作品，则他们的 app 也可能会被移除。如果您认为自己的知识产权在 App Store 上受到了其他开发者的侵犯，请通过[此网页表格](https://www.apple.com/legal/internet-services/itunes/appstorenotices/#?lang=zh)提交权利主张。各个国家/地区的法律互不相同，但请务必避免以下常见错误：
    
    - 一般性：不得在未经授权的情况下，在 app 中使用受保护的第三方材料 (例如商标、版权作品、专利设计)；也不得在 app 套装或开发者名称中包含虚假、抄袭或误导性的演示、名称或元数据。App 提交方应当是拥有或获授权使用知识产权及其他相关权利的个人或法律实体。
    - 第三方站点/服务：如果您的 app 会使用、访问第三方服务、通过访问第三方服务盈利或是显示第三方服务的内容，请确保您获得在该服务的使用条款下进行此类操作的特别许可。如有相应要求，则必须提供相关授权。
    - 音频/视频下载：App 不得促进非法文件共享，或在没有获得这些资源的明确授权的情况下，提供从第三方来源 (如 Apple Music、YouTube、SoundCloud、Vimeo 等) 保存、转换或下载媒体资源的功能或帮助。视频/音频内容流也有可能触犯使用条款，所以请务必在 app 访问这些服务前，进行检查。如有相应要求，则必须提供相关文稿。
    - 受 Apple 认可：不得误导或暗示 Apple 是 app 的来源或提供商，或者 Apple 以任何形式表示认可其质量或功能。如果您的 app 被选为“编辑选荐”，Apple 将自动显示相应徽章。
    - Apple 产品：不得创建与现有 Apple 产品、界面 (如访达)、app (如 App Store、iTunes Store 或“信息”) 或广告主题外观相似或容易混淆的 app。App 和扩展功能 (包括第三方键盘和贴纸包) 不得含有 Apple 表情符号。iTunes 音乐预览内容不得用于其娱乐价值 (如用作照片拼贴画的背景音乐或游戏配音) 或其他未获授权的方式。如果 app 显示健身记录圆环，则不应以类似于“健身记录”控件的方式展示“活动”，“锻炼”或“站立”数据。请参考“[Human Interface Guidelines (英文)](https://developer.apple.com/design/human-interface-guidelines/watchos/system-capabilities/health-and-fitness/#activity-rings)”以了解关于如何使用健身记录圆环的更多信息。
- **游戏、赌博和彩票**
    
    游戏、赌博和彩票的管理难度大，是 App Store 上受到最多管制的 app 类别之一。只有全面核实了即将发布您的 app 的所有国家/地区的相关法律要求后，才能包含此功能，并且要做好准备此功能的审核流程需要更长的时间。您需要谨记以下事项：
    
    - 抽奖和比赛必须由 app 的开发者赞助。
    - 抽奖、比赛和抽彩的正式规则必须在 app 中注明，并且必须明确表示 Apple 不是赞助者，也没有以任何形式参与活动。
    - App 不得通过 App 内购买项目购买点数或货币，以用于任何种类的真实货币游戏；不得向用户出售彩票或抽彩券；不得在 app 内进行资金转账。
    - 提供真实货币游戏（例如体育下注、扑克、赌场游戏、赛马）或彩票的 App 必须在使用该 App 的地区获得必要的许可和批准，且只能在这些地区发布，此类 App 在 App Store 中必须免费提供。不允许在 App Store 上发布非法的赌博辅助工具，包括记牌器。彩票 App 必须有报酬、几率及奖品。
- **VPN App**
    
    提供 VPN 服务的 app 必须利用 [NEVPNManager API (英文)](https://developer.apple.com/documentation/networkextension/nevpnmanager)，并且仅可由登记为企业的开发者提供。在用户进行任何操作来购买或以其他方式使用该服务之前，您必须在 app 屏幕上清楚地声明会收集哪些用户数据，以及将如何使用这些数据。无论出于任何目的，提供 VPN 服务的 App 不得向第三方出售、使用或披露任何数据；并且必须在隐私政策中做出这一承诺。VPN app 不得违反当地法律，如果您选择在需要 VPN 许可证的地区发布，则必须在 App Review 注释栏位中提供您的许可证信息。除此之外，经批准的提供商所提供的家长控制、内容拦截和安全 app 也可以使用 NEVPNManager API。不遵循这项准则的 app 会被从 App Store 中移除，您也可能会被从 Apple Developer Program 中除名。
    
- **移动设备管理**
    
    提供移动设备管理 (MDM) 服务的 MDM App 必须向 Apple 请求此功能。此类 app 仅可由商业企业 (如商业组织、教育机构或政府机构) 提供；在少数情况下，也可由使用 MDM 提供家长控制服务或设备安全性的公司提供。在用户进行任何操作来购买或以其他方式使用该服务之前，您必须在 app 屏幕上清楚地声明会收集哪些用户数据，以及将如何使用这些数据。MDM app 不得违反任何适用法律。无论出于任何目的，提供 MDM 服务的 app 都不得出售、使用或向第三方披露任何数据；并且必须在隐私政策中做出这一承诺。在少数情况下，可能允许包含第三方数据分析，前提是相关服务仅收集或传输关于开发者的 MDM app 性能的数据，而不会收集关于用户、用户设备或该设备上其他 app 的任何数据。提供配置描述文件的 app 也必须遵守这些要求。不遵循这项准则的 app 会被从 App Store 中移除，您也可能会从 Apple Developer Program 中除名。
    
- **开发者行为准则**
    
    请尊重每一个人，无论是在 App Store 中回复用户评论、回应用户支持请求，还是与 Apple 沟通时 (包括您在解决方案中心的回复)，都应做到这一点。请勿涉及任何形式的骚扰、歧视、恐吓或霸凌行为，也不要鼓励他人实施任何上述行为。
    
    顾客的信任是 App Store 获得成功的基石。App 不得存在以下行为：掠夺用户或试图勒索用户；诱导用户进行非自愿的购买；强迫用户共享不必要的数据；以欺骗的方式抬高价格；针对未交付的功能或内容收取费用；或者在 app 内部或外部实施任何其他操纵行为。
    
    - **App Store 评论**
        
        App Store 的用户评论是 app 体验中不可或缺的一部分。在回复用户的评论时，请对他们保持尊重。另外，您的回复应直接回应用户评论的主题，请勿在回复中包含个人信息、垃圾信息或营销广告。
        
        请使用我们提供的 API 提示用户评价您的 app：通过这项便利功能，用户无需离开 app，就可直接在 App Store 中留下评分和评论；不允许使用自定义的评论提示。
        

### 提交之后

在 App Store Connect 中提交 app 和元数据之后，您随即就会进入审核流程。请谨记以下几点：

- **时间**：App Review 团队会尽快检查您的 app。不过，如果 app 比较复杂或者存在新的问题，则可能需要更深入的审查和考量。另请注意，如果 app 因为违反同一准则而一再被拒绝，或者您曾经试图操纵 App Review 流程，您的 app 将需要更长时间才能完成审核。进一步了解 [App Review](https://developer.apple.com/cn/app-store/review/)。
- **状态更新**：App 的当前状态会反映在 App Store Connect 中，所以请多留意此处。
- **加急请求**：如果您遇到了严重的时间问题，可以[申请加急审核 (英文)](https://developer.apple.com/contact/app-store/?topic=expedite)。请仅在您真的需要加快审核时才提出申请，以便其他开发者的加急请求不受影响。如果我们发现您滥用此系统，从此以后我们可能都会拒绝您的申请。
- **发布日期**：如果您为 app 设定了在未来某个日期发布，即使该 app 提前通过了 App Review 团队的审核，在设定的发布日期前也不会显示在 App Store 上。请注意，您的 app 可能需要长达 24 小时才能显示在所有选定的商店中。
- **拒绝**：我们的目标是公平、一以贯之地遵循这些准则，但是人无完人。如果您的 app 被拒绝，但您存在疑问，或希望提供其他信息，请通过解决方案中心与 App Review 团队直接沟通。这样不但有助于您的 app 在商店中上架，也能帮助我们改进 App Review 流程，并在我们的政策中发现需要阐明的部分。如果您仍然不认同审核结果，或是想要建议修改相关准则，请[提交申诉 (英文)](https://developer.apple.com/contact/app-store/?topic=appeal)。
- **申诉**：如果您仍然不认同审核结果，或是想要建议修改相关准则，请[提交申诉 (英文)](https://developer.apple.com/contact/app-store/?topic=appeal)。这样不但有助于您的 app 在商店中上架，也能帮助我们改进 App Review 流程，并在我们的政策中发现需要阐明的部分。
- **错误修复提交**：对于已在 App Store 中上架的 app，除非涉及法律问题，否则错误修复不会再因违反准则而被延迟。如果您的 app 已被拒绝，但它符合此流程的条件，请通过解决方案中心直接与 App Review 团队沟通，表明您想要使用此流程并计划在下次提交时解决 app 中存在的问题。

# 支持应用内购

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

1. 校验订单，但校验结果一直返回 21003 ，最开始怀疑是代码不正确、机器异常、沙盒测试账号有异常，但实际上是发送校验请求有误（oops）。解决办法的链接可参见 

["The receipt could not be authenticated" - should it be checked again?](https://stackoverflow.com/questions/58615404/the-receipt-could-not-be-authenticated-should-it-be-checked-again)

1. 在 qmake 工程中添加内购流程后一切正常，但使用 qmake 装换成 xcode 工程后调试，一直提示 

```
Could not find the Qt platform plugin "cocoa" in ""
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.
```

解决方法：安装 *.app/Contents/embedded.provisionprofile 文件即可，该文件的一些信息可见

[细谈证书与Provisioning Profile](https://www.jianshu.com/p/abd3c55db48e)

1. 使用真实账号登陆沙盒测试账号购买异常（这是正确的，oops），详见

[【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)

1. 偶现程序已启动就进入到 

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

## 参考链接

[掘金](https://juejin.cn/post/6845166890420011022)

[掘金](https://juejin.cn/post/6857502956317736968)

[](https://blog.csdn.net/box_kun/article/details/85243602)

[Apple Developer Documentation](https://developer.apple.com/documentation/storekit/original_api_for_in-app_purchase/subscriptions_and_offers/handling_subscriptions_billing?language=objc)

[ios - 如何在应用重启或重新安装时检查iOS中的订阅是否处于事件状态](https://www.coder.work/article/7152298)

[](https://help.apple.com/app-store-connect/#/devb57be10e7)

[How do you add an in-app purchase to an iOS application?](https://stackoverflow.com/questions/19556336/how-do-you-add-an-in-app-purchase-to-an-ios-application)

[In-App Purchase Tutorial: Getting Started](https://www.raywenderlich.com/5456-in-app-purchase-tutorial-getting-started)

[](https://www.cnblogs.com/kris888/archive/2012/09/21/2696754.html)

[Get Shared Secret](https://admost.github.io/amrapi/subscription/apple/shared-secret/)

[Apple App Store Receipt Checker - RevenueCat](https://www.revenuecat.com/apple-receipt-checker)

[App 内购买项目 - Apple Developer](https://developer.apple.com/cn/in-app-purchase/)

## 支持首用户免费使用一段时间

可参考 ****在 App 中实现推介促销优惠****：

[在 App 中实现推介促销优惠 - 简体中文文档 - Apple Developer](https://developer.apple.com/cn/documentation/storekit/in-app_purchase/subscriptions_and_offers/implementing_introductory_offers_in_your_app/)

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

## adding login items for macos

[Adding Login Items for macOS](https://hoishing.medium.com/adding-login-items-for-macos-7d76458f6495)

# 链接

- 

[](https://help.apple.com/developer-account/?lang=zh-cn#/dev7a05c5ce1)

[How to Publish Qt Applications in the Mac App Store](https://www.qt.io/blog/2012/04/03/how-to-publish-qt-applications-in-the-mac-app-store-2)

[Qt程序上架Mac App Store 基本流程_u013820121的专栏-CSDN博客](https://blog.csdn.net/u013820121/article/details/106624994)

[Mac下将QT程序打包成能发布到Mac App store的pkg安装包_casun_li的专栏-CSDN博客](https://blog.csdn.net/casun_li/article/details/71741968)

[How do you fix "code object is not signed at all In subcomponent:" in Xcode 6, Mac OS X Yosemite or Mavericks?](https://stackoverflow.com/questions/29076321/how-do-you-fix-code-object-is-not-signed-at-all-in-subcomponent-in-xcode-6-m)

[Mac下将QT程序打包成能发布到Mac App store的pkg安装包](https://www.geek-share.com/detail/2706784546.html)

[松果压制上架 Mac App Store 及沙盒机制的部分说明](https://walkginkgo.com/ffmpeg/2018/07/15/MAS-sanbox.html)

[发布Unity项目到macOS AppStore和Steam](https://yemi.me/2020/02/17/submit-unity-macos-build-to-steam-appstore/)

[在 Sandboxed Mac App 中嵌入第三方可执行文件](https://xujiwei.com/blog/2013/02/embed-third-party-binary-in-sandboxed-mac-app/)

下面的是针对 ios 应用程序上架的情况：

[3、苹果应用商店上架流程](https://www.jianshu.com/p/9dff65d391d3)

[ios app真机测试到上架App Store详细教程-必看](http://blog.applicationloader.net/blog/zh/88.html)

[](https://www.notion.so/iOS-App-be0119a8d6e0475a8869c4471dc2f080)

https://www.notion.so/iOS-App-be0119a8d6e0475a8869c4471dc2f080

apple 上架指南：

[App Store 审核指南](https://developer.apple.com/cn/app-store/review/guidelines/)

沙盒测试账号的说明：

[【iOS】苹果IAP(内购)中沙盒账号使用注意事项](https://www.jianshu.com/p/1ef61a785508)

[Copy of 图文详解丨iOS App上架全流程及审核避坑指南 - 知乎](https://www.notion.so/Copy-of-iOS-App-364f4ce0e2d44bf8a2a1aad194eef8c2?pvs=21)

---


- [上一级](README.md)
- 下一篇 -> [mac 开发资料](macDevelopSome.md)
