#ifndef PORTABLE_XMem_HEADER
#define PORTABLE_XMem_HEADER

#if defined(_WIN64) || defined(_WIN32)
#include "XVMem_windows.h"
#else
#include "XVMem_unix.h"
#endif 

extern UINT32 UnMap(void* address, size_t /*size*/);

#endif    //PORTABLE_XMem_HEADER