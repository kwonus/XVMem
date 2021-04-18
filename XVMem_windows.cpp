#if defined(_WIN64) || defined(_WIN32)
#include "XVMem.h"

UINT32 UnMap(void* address, size_t /*size*/)
{
    bool okay = true;
    try
    {
        UnmapViewOfFile(address);
    }
    catch (...)
    {
        okay = false;
    }
    return okay;
}
#endif