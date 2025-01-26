#include "../include/azvs_base.h"
#include "../include/azvs_linear.h"
#include <string.h>

void __destroyLinear();

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

    return linear;
}

void __destroyLinear(AP_Linear self)
{
    DELETE(self->flags);
    DELETE(self->data);
    DELETE(self);
    self = NULL;
}