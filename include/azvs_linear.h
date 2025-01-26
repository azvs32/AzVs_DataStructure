#ifndef AZVS_LINEAR_H
#define AZVS_LINEAR_H

#include <stdlib.h>

typedef struct azvs_linear
{
    void *data;               // 数据指针
    char *flags;              // 数据标记：flags[i]=='Y'时表示data[i]中存放了数据
    size_t data_size;         // 线性表中元素的大小
    char *data_type;          // 线性表中元素的类型
    size_t count;             // 线性表实际数据数量
    size_t total;             // 线性表最大存储容量

    // 销毁线性表
    void (*destroy)();
} A_Linear, *AP_Linear;

// 初始化线性表对象
AP_Linear __a_initLinear(const size_t total, const size_t data_size, char *data_type);
#define a_initLinear(total, data_type) __a_initLinear(total, sizeof(data_type), #data_type)

#endif // AZVS_LINEAR_H