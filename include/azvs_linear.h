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

    /**
     * @brief 释放线性表
     * @param self 线性表本身
     */
    void (*destroy)(struct azvs_linear * self);

    /**
     * @brief 在下标index位置存入data数据
     * @param self 线性表本身
     * @param data 需要插入的数据
     * @param index 指定的下标
     */
    void (*set)(const struct azvs_linear * self, const void *data, const size_t index);

    /**
     * @brief 返回指定下标元素的数据
     * @param self 线性表本身
     * @param index 指定的下标
     */
    void*(*get)(const struct azvs_linear * self, const size_t index);

    /**
     * @brief 删除指定下标元素的数据
     * @param self 线性表本身
     * @param index 指定的下标
     */
    void (*del)(const struct azvs_linear * self, const size_t index);

} A_Linear, *AP_Linear;

/**
 * @brief 初始化线性表
 * @param total 线性表存储元素的最大数量
 * @param data_size 每个元素占用的大小
 * @param data_type 线性表中元素的类型
 */
AP_Linear __a_initLinear(const size_t total, const size_t data_size, char *data_type);
#define a_initLinear(total, data_type) __a_initLinear(total, sizeof(data_type), #data_type)

#endif // AZVS_LINEAR_H