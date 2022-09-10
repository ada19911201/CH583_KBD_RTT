#include "xprintf.h"
#include "stdio.h"
#include <stdarg.h>
#include "usbuser.h"

int xprintf(const char *format, ...)
{
    int printed;
    va_list args;
    va_start(args, format);
    char dat_buf[62];
    printed = vsprintf((char *)dat_buf, format, args);
    hid_send_data(printed,(char*)dat_buf);
    va_end(args);
    return printed;
}



