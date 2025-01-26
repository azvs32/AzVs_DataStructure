#include "../include/azvs_base.h"
#include "../include/azvs_linear.h"
#include <string.h>

void __destroyLinear(AP_Linear self);
void __setLinear(const AP_Linear self, const void * data, const size_t index);
void *__getLinear(const AP_Linear self, const size_t index);
void __delLinear(const AP_Linear self, const size_t index);

AP_Linear __a_initLinear(const size_t total, const size_t data_size, char *data_type)
{
    AP_Linear linear = NEW_TYPE(A_Linear);
    linear->total = total;
    linear->data_size = data_size;
    linear->data_type = data_type;
    linear->count = 0;
    linear->data = NEW_ARRAY(char, data_size*total);
    linear->flags = NEW_ARRAY(char, total);
    memset(linear->flags, 'N', total);

    linear->destroy = __destroyLinear;
    linear->set = __setLinear;
    linear->get = __getLinear;
    linear->del = __delLinear;

    return linear;
}

void __destroyLinear(AP_Linear self)
{
    DELETE(self->flags);
    DELETE(self->data);
    DELETE(self);
    self = NULL;
}

void __setLinear(const AP_Linear self, const void * data, const size_t index)
{
    GUARD_CHECK_ERROR(self==NULL, return, "linear set: 线性表为空");
    GUARD_CHECK_ERROR(index>=self->total && index<0, return, "linear set: 线性表下标越界");
    GUARD_CHECK_ERROR(data==NULL, return, "linear set: 数据元素不允许为NULL");
    if (self->flags[index]=='Y')
    {
        A_LOG_MSG_DEBUG("linear set: 覆盖了原有的数据");
        self->count-=1;
    }
    memcpy(self->data+index*self->data_size, data, self->data_size);
    self->flags[index] = 'Y';
    self->count += 1;
}

void *__getLinear(const AP_Linear self, const size_t index)
{
    GUARD_CHECK_ERROR(self==NULL, return NULL, "linear get: 线性表为空");
    GUARD_CHECK_ERROR(index>=self->total && index<0, return NULL, "linear get: 线性表下标越界");
    if (self->flags[index] == 'N')
    {
        A_LOG_MSG_DEBUG("linear get: 线性表[%d]没有数据", index);
    }
    return self->data+index*self->data_size;
}

void __delLinear(const AP_Linear self, const size_t index)
{
    GUARD_CHECK_ERROR(self==NULL, return, "linear del: 线性表为空");
    GUARD_CHECK_ERROR(index>=self->total && index<0, return, "linear del: 线性表下标越界");
    if (self->flags[index] == 'N')
    {
        A_LOG_MSG_DEBUG("linear del: 线性表[%d]没有数据", index);
        return;
    }
    memset(self->data+index*self->data_size, 0, self->data_size);
    self->flags[index] = 'N';
    self->count -= 1;
    A_LOG_MSG_DEBUG("linear del: 线性表[%d]数据已删除", index);
}