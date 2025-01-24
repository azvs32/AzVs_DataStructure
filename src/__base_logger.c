#include "../include/azvs_base.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void __setLoggerLevel(const int level);
void __setLoggerOutputType(const int type);
void __setFilePath(const char *path);
void __loggerMessage(const A_LoggerLevel level, const char *function, const int line, const char *format, ...);
void __destroy();

char *getLoggerLevelString(const A_LoggerLevel level);

// 全局单例的日志对象
A_Logger *a_logger_singleton;

// 初始化日志对象
A_Logger *a_initLogger()
{
    if (NULL == a_logger_singleton)
    {
        a_logger_singleton = (A_Logger *)malloc(sizeof(A_Logger));
        if (!a_logger_singleton)
        {
            fprintf(stderr, "init -> 初始化日志对象时内存分配失败！\n");
            return NULL;
        }

        a_logger_singleton->__currentLoggerLevel = A_INFO | A_WARN | A_ERROR;
        a_logger_singleton->__currentLoggerOutputType = A_CONSOLE;
        a_logger_singleton->__filePath = strdup("./logs/default.log");
        a_logger_singleton->__outputFile = NULL;

        a_logger_singleton->setLoggerLevel = __setLoggerLevel;
        a_logger_singleton->setLoggerOutputType = __setLoggerOutputType;
        a_logger_singleton->setFilePath = __setFilePath;
        a_logger_singleton->loggerMessage = __loggerMessage;
        a_logger_singleton->destroy = __destroy;
    }
    return a_logger_singleton;
}

void __setLoggerLevel(const int level)
{
    a_logger_singleton->__currentLoggerLevel = level;
}

void __setLoggerOutputType(const int type)
{
    /*
    如果 原(Y) AND 新(N)
        关闭 FILE 文件
    如果 原(N) AND 新(Y)
        如果 PATH 存在
            打开 FILE 文件
        如果 PATH 不存在
            exit(1)
    */

    if ((a_logger_singleton->__currentLoggerOutputType & A_FILE) != 0 && (type & A_FILE) == 0)
    {
        if (fclose(a_logger_singleton->__outputFile) != 0)
        {
            A_LOG_MSG_ERROR("日志文件 %s 关闭失败!\n", a_logger_singleton->__filePath);
        }
        else
        {
            a_logger_singleton->__outputFile = NULL;
        }
    }
    if ((a_logger_singleton->__currentLoggerOutputType & A_FILE) == 0 && (type & A_FILE) != 0)
    {
        if (a_logger_singleton->__filePath != NULL)
        {
            a_logger_singleton->__outputFile = fopen(a_logger_singleton->__filePath, "a+");
            if (a_logger_singleton->__outputFile == NULL)
            {
                A_LOG_MSG_FATAL("日志文件 %s 打开失败！请确保路径存在！\n", a_logger_singleton->__filePath);
                exit(1);
            }
        }
        else
        {
            A_LOG_MSG_FATAL("未正确设置日志文件路径！\n");
            exit(1);
        }
    }
    a_logger_singleton->__currentLoggerOutputType = type;
}

void __setFilePath(const char *path)
{
    /*
    如果 未开启日志文件模式
        直接修改 path
    如果 开启日志文件模式
        关闭之前的日志文件
        修改 path
        打开新的日志文件
    */

    if ((a_logger_singleton->__currentLoggerOutputType & A_FILE) == 0)
    {
        free(a_logger_singleton->__filePath);
        a_logger_singleton->__filePath = strdup(path);
    }
    if ((a_logger_singleton->__currentLoggerOutputType & A_FILE) != 0)
    {
        if (fclose(a_logger_singleton->__outputFile) != 0)
        {
            A_LOG_MSG_ERROR("日志文件 %s 关闭失败！\n", a_logger_singleton->__filePath);
        }
        else
        {
            a_logger_singleton->__outputFile = NULL;
        }
        free(a_logger_singleton->__filePath);
        a_logger_singleton->__filePath = strdup(path);
        a_logger_singleton->__outputFile = fopen(a_logger_singleton->__filePath, "a+");
        if (a_logger_singleton->__outputFile == NULL)
        {
            A_LOG_MSG_FATAL("日志文件 %s 打开失败！请确保路径存在！\n", a_logger_singleton->__filePath);
            exit(1);
        }
    }
}

void __loggerMessage(const A_LoggerLevel level, const char *function, const int line, const char *format, ...)
{
    if (0 == (level & a_logger_singleton->__currentLoggerLevel))
    {
        return;
    }

    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    time_t nowtime;
    struct tm *timeinfo;
    time(&nowtime);
    timeinfo = localtime(&nowtime);

    // 控制台输出逻辑
    if (A_CONSOLE & a_logger_singleton->__currentLoggerOutputType)
    {
        printf("[%04d-%02d-%02d %02d:%02d:%02d][%s][%s:%d] -> %s",
               timeinfo->tm_year + 1900,
               timeinfo->tm_mon + 1,
               timeinfo->tm_mday,
               timeinfo->tm_hour,
               timeinfo->tm_min,
               timeinfo->tm_sec,
               getLoggerLevelString(level),
               function, line, buffer);
    }
    // 文件输出逻辑
    if (A_FILE & a_logger_singleton->__currentLoggerOutputType)
    {
        fprintf(a_logger_singleton->__outputFile,
                "[%04d-%02d-%02d %02d:%02d:%02d][%s][%s:%d] -> %s",
                timeinfo->tm_year + 1900,
                timeinfo->tm_mon + 1,
                timeinfo->tm_mday,
                timeinfo->tm_hour,
                timeinfo->tm_min,
                timeinfo->tm_sec,
                getLoggerLevelString(level),
                function, line, buffer);
        fflush(a_logger_singleton->__outputFile); // 确保内容写入文件
    }
}

void __destroy()
{
    if (a_logger_singleton->__outputFile)
    {
        if (fclose(a_logger_singleton->__outputFile) != 0)
        {
            A_LOG_MSG_ERROR("日志文件 %s 关闭失败！\n", a_logger_singleton->__filePath);
        }
        else
        {
            a_logger_singleton->__outputFile = NULL;
        }
        free(a_logger_singleton->__filePath);
        a_logger_singleton->__filePath = NULL;
    }
    free(a_logger_singleton);
    a_logger_singleton = NULL;
}

char *getLoggerLevelString(const A_LoggerLevel level)
{
    switch (level)
    {
    case A_FATAL:
        return "FATAL";
    case A_ERROR:
        return "ERROR";
    case A_WARN:
        return "WARN";
    case A_INFO:
        return "INFO";
    case A_DEBUG:
        return "DEBUG";
    case A_TRACE:
        return "TRACE";
    default:
        return "UNKNOWN";
    }
}