#include "helper.h"

Helper::Helper(QObject *parent) : QObject(parent)
{
    buffer = NULL;

}

bool Helper::allocate(){
    printf("size: %d\n",size);
    if(!buffer)
        buffer = (char *)malloc(size*1024);
    else
        buffer = (char *)realloc(buffer, size*1024);

   if(!buffer)
       return false;

   memset(buffer, 0, size*1024);
   size = size + 100;
   return true;
}

int Helper::get_size()
{
    return size;
}

bool Helper::free_buffer()
{
    if(buffer != NULL){
        free(buffer);
        buffer = NULL;
        size = 100;
        return true;
    }
    return false;
}
