#ifndef WINDOWS_XVMem_HEADER
#define WINDOWS_XVMem_HEADER

#define WIN32_LEAN_AND_MEAN 
#include <windows.h> 
#include <malloc.h> 
#include <stdio.h> 

#define    ImportLib    __declspec( dllimport ) 
#define    ExportLib    __declspec( dllexport )

inline char* Strncpy(char* destination, const char* name, int len) {
#pragma warning(suppress : 4996)
    return strncpy(destination, name, len);
}
inline size_t Strnlen(const char* str, int maxlen) {
#pragma warning(suppress : 4996)
    return strnlen(str, maxlen);
}
inline size_t Strnicmp(const char* str1, const char* str2, int maxlen) {
#pragma warning(suppress : 4996)
    return strnicmp(str1, str2, maxlen);
}
inline size_t Strncmp(const char* str1, const char* str2, int maxlen) {
#pragma warning(suppress : 4996)
    return strncmp(str1, str2, maxlen);
}

#endif    //WINDOWS_XVMem_HEADER