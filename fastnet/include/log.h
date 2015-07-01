#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>  
#include <stdarg.h>  
#include <ctype.h>  
#pragma warning(push)  
#pragma warning(disable:4996)  
namespace base
{
 
void __cdecl log_out_a(const char *format, ...)  
{  
    va_list vlArgs;  
    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);  
    va_start(vlArgs, format);  
    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);  
    va_end(vlArgs);  
    strcat(strBuffer, "\n");  
    OutputDebugStringA(strBuffer);  
    GlobalFree(strBuffer);  
    return;  
} 
}
#endif 