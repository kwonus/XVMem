///////////////////////////////////////////////////////////////// 
//    Linux and MAC API for SHARED MEMORY (MIMIC A SUBSET OF WIN32) 
///////////////////////////////////////////////////////////////// 
#if !(defined(_WIN64) || defined(_WIN32))

#include <sys/types.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>
#include "XVMem.h"

typedef void* LPSECURITY_ATTRIBUTES;

void CloseHandle(HANDLE& handle)
{
    if (handle != NULL)
    {
        int* fd = handle;
        close(*fd);

        handle = NULL;
    }
}

HANDLE CreateFile(const char lpFileName[],
    UINT32 dwDesiredAccess,
    UINT32 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    UINT32 dwCreationDisposition,
    UINT32 dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    static int files[32];  // maximum of 32 mem-mapped files 
    static int memMaps = 32;

    // NOTE:  GENERIC_READ  == 0 
    // AND:   GENERIC_WRITE == O_RDWR 
    // THEREFORE:  GENERIC_READ|GENERIC_WRITE == O_RDWR  [this logic does not permit write-only] 

    int fd = open(lpFileName, dwDesiredAccess | dwCreationDisposition);

    if (fd < 0)
    {
        return NULL;
    }
    if (--memMaps < 0)
    {
        close(fd);
        return NULL;
    }
    return &(files[memMaps] = fd);
}

bool WriteFile(HANDLE handle,
    void* buf,
    UINT32 nNumberOfBytesToWrite,
    UINT32* lpNumberOfBytesWritten,
    void* ignore)
{
    *lpNumberOfBytesWritten = write(*handle, buf, nNumberOfBytesToWrite);

    bool written = (*lpNumberOfBytesWritten > 0);

    if (!written)
        *lpNumberOfBytesWritten = 0;

    return written;
}

bool ReadFile(HANDLE handle,
    void* buf,
    UINT32 nNumberOfBytesToRead,
    UINT32* lpNumberOfBytesRead,
    void* ignore)
{
    *lpNumberOfBytesRead = read(*handle, buf, nNumberOfBytesToRead);

    bool some_read = (*lpNumberOfBytesRead > 0);

    if (!some_read)
        *lpNumberOfBytesRead = 0;

    return some_read;
}

int  GetLastError()
{
    return errno;
}

HANDLE CreateFileMapping(HANDLE inputHandle, UINT32, UINT32, UINT32, UINT32, UINT32)
{
    return inputHandle;
}

UINT32 UnMap(void* address, size_t size)
{
    return (-1 != munmap(address, size));
}

void* MapViewOfFile(HANDLE hFileMappingObject,
    UINT32 dwDesiredAccess,
    UINT32 /*dwFileOffsetHigh*/, // ignored 
    UINT32 dwFileOffsetLow,
    UINT32 dwNumberOfBytesToMap)
{
    int protection = dwDesiredAccess;

    void* map = mmap(0,
        dwNumberOfBytesToMap,
        protection,
        MAP_SHARED,
        *hFileMappingObject,
        dwFileOffsetLow);

    if (map == MAP_FAILED)
    {
        map = NULL;
    }
    return map;
}

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 

UINT32 GetFileSize(HANDLE handle, UINT32)
{
    UINT32 size = 0;

    struct stat sb;

    if (fstat(*handle, &sb) == 0)
    {
        size = sb.st_size;
    }
    return size;
}

#endif

