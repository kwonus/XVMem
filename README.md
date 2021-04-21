# XVMem
This project is ispired by 'CSharedStruct'<br> > https://www.codeproject.com/Articles/1362/A-Simple-Wrapper-for-Sharing-Data-Structures-Betwe <br>
My initial fork of that project was called 'XSharedStruct' <br> > http://trustedsound.com/code/CSharedStruct.txt <br>

Whereas CSharedStruct was windows-only, XSharedStruct was my initial cross-platform port (thus the X-prefix).  Yet, XSharedStruct was a quick-and-dirty port to Linux and Mac. XVMem is a cleaner cross-platform implementation.  It still exposes a strong Windows bias; so it feels like you're using the WIN32/WIN64 API for shared/virtual memory and file IO.  Header files and cpp files map that API onto *nix (Linux and MacOS) shared/virtual memory and file IO.

This project is currently used by the AVX cpp library (forthcoming to my public github account soon).  XVMem is primarilly intended as a virtual memory backing store for large files.  By design, it supports read/write, albeit without semaphores to avoid race conditions (I say, "by design", because my current usage of this library is read-only. The libary not only supports read-only memory, but enforces it).  XVMem also supports mallocing the ram instead of utilizing shared memory (in case memory usage only needs to be in-process).

There's probably more robust libaries out there that serve similiar purposes, but this one is familiar to me and introduces zero additional dependencies.  I'm sharing it in case anyone has similiar needs.  Licence is MIT.
