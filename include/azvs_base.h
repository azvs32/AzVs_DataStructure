#ifndef AZVS_BASE_H
#define AZVS_BASE_H

#include <stdio.h>

//////////////////////////////////////////////////
//     日志系统
//////////////////////////////////////////////////

//  #idea ...
//  - 设置日志文件路径没有任何验证（strdup）
//      + 路径设置有问题，对日志是毁灭性的
//      + 若路径不存在，则创建文件夹（属于系统调用）
//  - 考虑线程安全问题
//      + Linux 和 Windows 的线程函数不同
//      + C11 开始才有通用的线程函数标准库
//      + 学习完后，考虑跨平台的线程解决方案
//  - 将日志分类，存储在不同文件中（无实现思路）

/**
 * @brief 日志级别枚举
 */
typedef enum logger_level
{
    // 0000 0001 详细信息
    A_TRACE = 1 << 0,
    // 0000 0010 调试信息
    A_DEBUG = 1 << 1,
    // 0000 0100 一般信息
    A_INFO = 1 << 2,
    // 0000 1000 警告信息
    A_WARN = 1 << 3,
    // 0001 0000 错误信息
    A_ERROR = 1 << 4,
    // 0010 0000 严重错误信息
    A_FATAL = 1 << 5,
} A_LoggerLevel;

/**
 * @brief 日志输出方式
 */
typedef enum logger_output_type
{
    // 不输出日志文件
    A_NONE = 0,
    // 0000 0001 输出到控制台
    A_CONSOLE = 1 << 0,
    // 0000 0010 输出到文件
    A_FILE = 1 << 1,
} A_LoggerOutputType;

typedef struct azvs_logger
{
    // 当前日志输出级别
    A_LoggerLevel __currentLoggerLevel;
    // 当前日志输出方式
    A_LoggerOutputType __currentLoggerOutputType;
    // 日志文件指针
    FILE *__outputFile;
    // 日志文件路径
    char *__filePath;

    // 修改当前日志输出级别
    void (*setLoggerLevel)(const int level);
    // 修改当前日志输出方式
    void (*setLoggerOutputType)(const int type);
    // 设置日志文件路径，请确定路径存在
    void (*setFilePath)(const char *path);
    // 输出日志信息，并设置当前日志行的级别
    void (*loggerMessage)(const A_LoggerLevel level, const char *function, const int line, const char *format, ...);

    // 初始化日志结构体
    // （需要给函数指针赋值，只能放在结构体外部）
    // void (*init)();
    // 销毁日志结构体
    void (*destroy)();

} A_Logger;

// 全局单例的日志对象
extern A_Logger *a_logger_singleton;

// 初始化单例日志对象
A_Logger *a_initLogger();

// 简化日志输出

#ifdef DISABLE_LOGGER
    #define A_LOG_MSG(level, format, ...) do {} while(0)
#else
    #define A_LOG_MSG(level, format, ...) \
        do { \
            if (a_logger_singleton != NULL) { \
                a_logger_singleton->loggerMessage(level, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
            } \
        } while (0)
#endif // DISABLE_LOGGER
#define A_LOG_MSG_TRACE(format, ...) A_LOG_MSG(A_TRACE, format, ##__VA_ARGS__)
#define A_LOG_MSG_DEBUG(format, ...) A_LOG_MSG(A_DEBUG, format, ##__VA_ARGS__)
#define A_LOG_MSG_INFO(format, ...) A_LOG_MSG(A_INFO, format, ##__VA_ARGS__)
#define A_LOG_MSG_WARN(format, ...) A_LOG_MSG(A_WARN, format, ##__VA_ARGS__)
#define A_LOG_MSG_ERROR(format, ...) A_LOG_MSG(A_ERROR, format, ##__VA_ARGS__)
#define A_LOG_MSG_FATAL(format, ...) A_LOG_MSG(A_FATAL, format, ##__VA_ARGS__)

#endif // !AZVS_BASE_H