#include "object.h"

void init_object(Object* object, size_t object_size)
{
    object->data.device_id = NULL;
    object->data.last_seen = NULL;
    object->data.status = NULL;
    object->data.uptime = 0;
    object->size = object_size;
}

size_t getObjectSize(Object* object)
{
    return object->size;
}
