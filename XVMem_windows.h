#ifndef WINDOWS_XVMem_HEADER
#define WINDOWS_XVMem_HEADER

#define WIN32_LEAN_AND_MEAN 
#include <windows.h> 
#include <tchar.h> 
#include <malloc.h> 
#include <stdio.h> 

#define    ImportLib    __declspec( dllimport ) 
#define    ExportLib    __declspec( dllexport )

inline int Strncpy(TCHAR* destination, const TCHAR* name, int len) {
#pragma warning(suppress : 4996)
    _tcsncpy(destination, name, len);
}
inline int Strnlen(const char* str, int maxlen) {
#pragma warning(suppress : 4996)
    strnlen(str, maxlen);
}

#endif    //WINDOWS_XVMem_HEADER