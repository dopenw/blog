<!-- log4cplus 学习笔记 -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

- [使用配置文件将不同level 的日志输出到不同的文件中](#使用配置文件将不同level-的日志输出到不同的文件中)

<!-- /code_chunk_output -->

`log4cplus is a simple to use C++ logging API providing thread-safe, flexible, and arbitrarily granular control over log management and configuration. It is modelled after the Java log4j API.`

[log4cplus home](https://sourceforge.net/p/log4cplus/wiki/Home/)

log4cplus 使用示例详见 `$log4cplus/tests` 目录；

## 使用配置文件将不同level 的日志输出到不同的文件中
```c++

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;


static Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("log"));

void
printDebug()
{
    LOG4CPLUS_TRACE_METHOD(logger, LOG4CPLUS_TEXT("::printDebug()"));
    LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
    LOG4CPLUS_INFO(logger, "This is a INFO message");
    LOG4CPLUS_WARN(logger, "This is a WARN message");
    LOG4CPLUS_ERROR(logger, "This is a ERROR message");
    LOG4CPLUS_FATAL(logger, "This is a FATAL message");
}


int
main()
{
    tcout << "Entering main()..." << endl;
    log4cplus::initialize ();
    LogLog::getLogLog()->setInternalDebugging(true);
    Logger root = Logger::getRoot();
    try {
        PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

        LOG4CPLUS_WARN(root, "Testing....");
        printDebug();
    }
    catch(...) {
        tcout << "Exception..." << endl;
        LOG4CPLUS_FATAL(root, "Exception occured...");
    }

    tcout << "Exiting main()..." << endl;
    return 0;
}
```

log4cplus.properties:
```sh
log4cplus.rootLogger=TRACE, STDOUT, ALL_MSGS, TRACE_MSGS, DEBUG_INFO_MSGS, FATAL_MSGS

log4cplus.appender.STDOUT=log4cplus::ConsoleAppender

log4cplus.appender.ALL_MSGS=log4cplus::RollingFileAppender
log4cplus.appender.ALL_MSGS.File=all_msgs.log
log4cplus.appender.ALL_MSGS.layout=log4cplus::TTCCLayout
# 限制文件大小和备份文件数量
log4cplus.appender.ALL_MSGS.MaxFileSize=500KB
log4cplus.appender.ALL_MSGS.MaxBackupIndex=5

log4cplus.appender.TRACE_MSGS=log4cplus::RollingFileAppender
log4cplus.appender.TRACE_MSGS.File=trace_msgs.log
log4cplus.appender.TRACE_MSGS.layout=log4cplus::TTCCLayout
log4cplus.appender.TRACE_MSGS.filters.1=log4cplus::spi::LogLevelMatchFilter
log4cplus.appender.TRACE_MSGS.filters.1.LogLevelToMatch=TRACE
log4cplus.appender.TRACE_MSGS.filters.1.AcceptOnMatch=true
log4cplus.appender.TRACE_MSGS.filters.2=log4cplus::spi::DenyAllFilter

log4cplus.appender.DEBUG_INFO_MSGS=log4cplus::RollingFileAppender
log4cplus.appender.DEBUG_INFO_MSGS.File=debug_info_msgs.log
log4cplus.appender.DEBUG_INFO_MSGS.layout=log4cplus::TTCCLayout
log4cplus.appender.DEBUG_INFO_MSGS.filters.1=log4cplus::spi::LogLevelRangeFilter
log4cplus.appender.DEBUG_INFO_MSGS.filters.1.LogLevelMin=DEBUG
log4cplus.appender.DEBUG_INFO_MSGS.filters.1.LogLevelMax=INFO
log4cplus.appender.DEBUG_INFO_MSGS.filters.1.AcceptOnMatch=true
log4cplus.appender.DEBUG_INFO_MSGS.filters.2=log4cplus::spi::DenyAllFilter

log4cplus.appender.FATAL_MSGS=log4cplus::RollingFileAppender
log4cplus.appender.FATAL_MSGS.File=fatal_msgs.log
log4cplus.appender.FATAL_MSGS.layout=log4cplus::TTCCLayout
log4cplus.appender.FATAL_MSGS.filters.1=log4cplus::spi::StringMatchFilter
log4cplus.appender.FATAL_MSGS.filters.1.StringToMatch=FATAL
log4cplus.appender.FATAL_MSGS.filters.1.AcceptOnMatch=true
log4cplus.appender.FATAL_MSGS.filters.2=log4cplus::spi::DenyAllFilter
```

---
- [上一级](README.md)
- 上一篇 -> [链表](linkList.md)
- 下一篇 -> [c++并发](multiThread.md)
