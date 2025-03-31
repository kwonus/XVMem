//////////////////////////////////////////////////////////////////////////////////////////////// 
//    Cross-Platform (Windows, Linux, OSX) Shared memory library adapted from baseline project: 
//    http://www.codeproject.com/Articles/1362/A-Simple-Wrapper-for-Sharing-Data-Structures-Betwe 
// 
#ifndef XVMem_HEADER
#define XVMem_HEADER

#include <XVMem_platform.h>

extern char  g_hSharedHome[MAX_PATH];

template <typename StructType> class XVMem
{
private:
    HANDLE m_hFileHandle;
    HANDLE m_hFileMapping;
    char  m_hSharedName[MAX_PATH];
    UINT32  m_dwMaxDataSize;
    StructType* m_pvData;

    bool m_bCreated;
    bool m_bWritable;

public:
    XVMem();
    ~XVMem();

    UINT32 GetSize();
    UINT32 GetCnt();

    bool IsShared();
    void Release();
    bool Release(bool shared);
    StructType* Acquire(char* name, bool writable, bool shared, UINT32 forceCnt = 0, const StructType* bodyVal = NULL, const StructType* firstVal = NULL, const StructType* lastVal = NULL);

    StructType* GetData();
};

template <typename StructType> UINT32 XVMem<StructType>::GetSize()
{
    return(m_dwMaxDataSize);
}

template <typename StructType> UINT32 XVMem<StructType>::GetCnt()
{
    return(m_dwMaxDataSize / sizeof(StructType));
}

template <typename StructType> bool XVMem<StructType>::IsShared()
{
    return (m_pvData && IsShared());
}

template <typename StructType> StructType* XVMem<StructType>::GetData()
{
    return m_pvData;
}

template <typename StructType> XVMem<StructType>::~XVMem()
{
    Release();
}

template <typename StructType> XVMem<StructType>::XVMem()
    : m_pvData(NULL)
    , m_hFileMapping(NULL)
    , m_hFileHandle(NULL)
    , m_dwMaxDataSize(0)
{
    ;
}

template <typename StructType> bool XVMem<StructType>::Release(bool shared)
{
    bool released = (IsShared() == shared);

    if (released)
    {
        Release();
    }
    return released;
}

template <typename StructType> void XVMem<StructType>::Release()
{
    if (m_hFileMapping || m_hFileHandle)
    {
        if (m_pvData)
        {
            UnMap(m_pvData, GetSize());
            m_pvData = NULL;
        }

        if (m_hFileMapping)
        {
            CloseHandle(m_hFileMapping);
            CloseHandle(m_hFileHandle);

            m_hFileMapping = NULL;
            m_hFileHandle = NULL;
        }
    }
    else    // This may have been a malloc 
    {
        if (m_pvData)
        {
            free(m_pvData);

            m_pvData = NULL;
        }
    }
}

template <typename StructType> StructType* XVMem<StructType>::Acquire(char* name, bool writable, bool shared, UINT32 forceCnt, const StructType* bodyVal, const StructType* firstVal, const StructType* lastVal)
{
    if (m_pvData)
        return m_pvData;

    m_bWritable = writable;

    if (g_hSharedHome[0] == (char)0 || name == NULL || name[0] == (char) 0)
        return NULL;

    char path[MAX_PATH];
    size_t len = Strnlen(g_hSharedHome, MAX_PATH-1);
    Strncpy(path, g_hSharedHome, MAX_PATH);
    Strncpy(path + len, name, int(MAX_PATH - len));

    if (writable && shared)
    {
        m_hFileHandle = CreateFileA(path
            , GENERIC_READ | GENERIC_WRITE
            , FILE_SHARE_READ | FILE_SHARE_WRITE
            , NULL
            , (forceCnt > 0) ? CREATE_ALWAYS : OPEN_EXISTING
            , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN
            , NULL);

        if (m_hFileHandle == INVALID_HANDLE_VALUE)
        {
            m_hFileHandle = CreateFileA(path
                , GENERIC_READ | GENERIC_WRITE
                , FILE_SHARE_READ | FILE_SHARE_WRITE
                , NULL
                , OPEN_EXISTING
                , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN
                , NULL);
        }
        if (m_hFileHandle == INVALID_HANDLE_VALUE)
            return NULL;

        if (forceCnt > 0)
        {
            DWORD writtenCnt;
            if (firstVal)
                WriteFile(m_hFileHandle, (void*)firstVal, sizeof(StructType), &writtenCnt, NULL);
            else
                WriteFile(m_hFileHandle, (void*)bodyVal, sizeof(StructType), &writtenCnt, NULL);
            if (sizeof(StructType) != writtenCnt)
            {
                ;    // error 
            }
            int cnt = forceCnt - 1;
            for (int i = 1; i < cnt; i++)
            {
                WriteFile(m_hFileHandle, (void*)bodyVal, sizeof(StructType), &writtenCnt, NULL);
                if (sizeof(StructType) != writtenCnt)
                {
                    ;    // error 
                }
            }
            if (forceCnt > 1)
            {
                if (lastVal)
                    WriteFile(m_hFileHandle, (void*)lastVal, sizeof(StructType), &writtenCnt, NULL);
                else
                    WriteFile(m_hFileHandle, (void*)bodyVal, sizeof(StructType), &writtenCnt, NULL);
                if (sizeof(StructType) != writtenCnt)
                {
                    ;    // error 
                }
            }
        }
        if (m_hFileHandle != INVALID_HANDLE_VALUE)
        {
            m_dwMaxDataSize = GetFileSize(m_hFileHandle, NULL);
            m_hFileMapping = CreateFileMapping(m_hFileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
            m_pvData = (StructType*)MapViewOfFile(m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        }
    }
    else if (name != NULL)
    {
        m_hFileHandle = CreateFileA(path
            , GENERIC_READ
            , FILE_SHARE_READ
            , NULL
            , OPEN_EXISTING
            , FILE_ATTRIBUTE_READONLY | FILE_FLAG_SEQUENTIAL_SCAN
            , NULL);

        if (m_hFileHandle != INVALID_HANDLE_VALUE)
        {
            m_dwMaxDataSize = GetFileSize(m_hFileHandle, NULL);

            if (!shared)
            {
                DWORD sizeRead;
                m_pvData = (StructType*)malloc(m_dwMaxDataSize);
                ReadFile(m_hFileHandle, m_pvData, m_dwMaxDataSize, &sizeRead, NULL);

                if (sizeRead != m_dwMaxDataSize)
                {
                    free(m_pvData);
                    m_pvData = NULL;
                }
            }
            else
            {
                m_hFileMapping = CreateFileMapping(m_hFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
                m_pvData = (StructType*)MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, m_dwMaxDataSize);
            }
        }
    }
    if ((m_hFileMapping == INVALID_HANDLE_VALUE) && shared)
    {
        int err = GetLastError();
        return NULL;
    }
    if (name != NULL)
    {
        Strncpy(m_hSharedName, name, MAX_PATH - 1);
    }
    else
    {
        m_hSharedName[0] = '\0';
    }
    if ((m_pvData == NULL) || !shared)
    {
        if (m_hFileMapping && m_hFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_hFileMapping);
            m_hFileMapping = NULL;
        }
        if (shared)
            return NULL;

        UINT32 cnt = (forceCnt > 0) ? forceCnt : GetCnt();

        if (cnt <= GetCnt())
            return m_pvData;

        if (cnt == 0)
            return NULL;

        if (m_pvData)
            free(m_pvData);

        m_pvData = (StructType*)calloc(cnt, sizeof(StructType));

        if (!m_pvData)
            return NULL;
    }
    return m_pvData;
}

#endif //XVMem_HEADER