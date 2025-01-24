#include "../include/azvs_base.h"
#include <stdlib.h>

void *new_with_log(size_t size, const char *function, int line)
{
    void *result = malloc(size);
    if (NULL == result)
    {
        A_LOG_MSG_ERROR("内存分配失败！\n");
    }
    else
    {
        A_LOG_MSG_TRACE("地址 %p 分配 %u 字节的内存\n", result, size);
    }
    return result;
}

void *renew_with_log(void *ptr, size_t size, const char *function, int line)
{
    if (0 == size)
    {
        DELETE(ptr);
        return NULL;
    }
    void *result = realloc(ptr, size);
    if (NULL == result)
    {
        A_LOG_MSG_ERROR("内存分配失败！\n");
    }
    else
    {
        A_LOG_MSG_TRACE("地址 %p 重新分配 %u 字节的内存\n", result, size);
    }
    return result;
}

void delete_with_log(void *ptr, const char *function, int line)
{
    if (ptr)
    {
        free(ptr);
        A_LOG_MSG_TRACE("地址 %p 内存释放成功\n", ptr);
    }
    else
    {
        A_LOG_MSG_DEBUG("试图释放空指针\n");
    }
}