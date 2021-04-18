#ifndef MACOS_OR_LINUX_XVMem_HEADER
#define MACOS_OR_LINUX_XVMem_HEADER

#include <stdint.h>
#include <stdio.h>

#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#define MAX_PATH 512

typedef unsigned char BYTE;
typedef int32_t DWORD;
typedef uint32_t UINT32;
typedef uint16_t UINT16;
typedef int* HANDLE;

// TCHAR processing 
typedef char TCHAR;
typedef wchar_t WCHAR; // Not fully used but still needed for some function prototypes 
#define wcsncpy strncpy

#define    ExportLib 
#define    ImportLib

#define FILE_ATTRIBUTE_READONLY O_RDONLY        // dwDesiredAccess
#define GENERIC_READ            O_RDONLY        // dwDesiredAccess [O_RDONLY == 0]
#define GENERIC_WRITE           O_RDWR          // dwDesiredAccess [O_RDWR == (GENERIC_READ|GENERIC_WRITE)]
#define FILE_SHARE_READ         GENERIC_READ    // dwShareMode (ignored)
#define FILE_SHARE_WRITE        GENERIC_WRITE   // dwShareMode (ignored)
#define CREATE_ALWAYS           O_CREAT|O_TRUNC // dwCreationDisposition
#define OPEN_EXISTING           0               // dwCreationDisposition
#define FILE_ATTRIBUTE_NORMAL   1               // dwFlagsAndAttributes
#define FILE_FLAG_SEQUENTIAL_SCAN 0             // dwFlagsAndAttributes

#define INVALID_HANDLE_VALUE NULL

#define FILE_MAP_READ           PROT_READ
#define FILE_MAP_WRITE          PROT_WRITE

#define PAGE_READWRITE    0     // These are ignored (handled in *nix API using PROT_WRITE parameter)
#define PAGE_READONLY     0     // These are ignored (handled by *nix API using PROT_READ parameter)

inline char * Strncpy(char* destination, const char* source, int len) {
    return strncpy(destination, source, len);
}
inline int Strnlen(const char* str, int maxlen) {
    return strnlen(str, maxlen);
}
typedef void* LPSECURITY_ATTRIBUTES;
void CloseHandle(HANDLE& handle);
HANDLE CreateFile(const char lpFileName[],
                  UINT32 dwDesiredAccess,
                  UINT32 dwShareMode,
                  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                  UINT32 dwCreationDisposition,
                  UINT32 dwFlagsAndAttributes,
                  HANDLE hTemplateFile);
bool WriteFile(HANDLE handle,
               void* buf,
               UINT32 nNumberOfBytesToWrite,
               UINT32* lpNumberOfBytesWritten,
               void* ignore);
bool ReadFile(HANDLE handle,
              void* buf,
              UINT32 nNumberOfBytesToRead,
              UINT32* lpNumberOfBytesRead,
              void* ignore);
int  GetLastError();
HANDLE CreateFileMapping(HANDLE inputHandle, UINT32, UINT32, UINT32, UINT32, UINT32);
void* MapViewOfFile(HANDLE hFileMappingObject,
                    UINT32 dwDesiredAccess,
                    UINT32 /*dwFileOffsetHigh*/, // ignored
                    UINT32 dwFileOffsetLow,
                    UINT32 dwNumberOfBytesToMap);
UINT32 GetFileSize(HANDLE handle, UINT32);

#endif    //MACOS_OR_LINUX_XVMem_HEADER