# XVMem
This project is ispired by 'CSharedStruct':<br> > https://www.codeproject.com/Articles/1362/A-Simple-Wrapper-for-Sharing-Data-Structures-Betwe <br>
My initial fork of that project was eventually called 'XSharedStruct'. Here's an early version of my cross-platform port: <br> > http://trustedsound.com/code/CSharedStruct.txt <br>

Whereas CSharedStruct was windows-only, XSharedStruct was a cross-platform port (thus the X-prefix).  Still, XSharedStruct was a quick-and-dirty port to Linux and Mac. XVMem is a cleaner cross-platform implementation.  It still exposes a strong Windows bias; so it feels like you're using the WIN32/WIN64 API for shared/virtual memory and file IO.  Header files and cpp files map that API onto *nix (Linux and MacOS) shared/virtual memory and file IO.

This project is currently used by the AVXLib (in my public github account), with example usages in AVXTest (also in my public github account).  XVMem is primarilly intended as a virtual memory backing store for large files.  By design, it supports read/write, albeit without semaphores (I say, "by design", because my current usage of this library is read-only). With no semephores, avoiding race conditions is left up to the consumer of the lib when it is not used in read-only mode. XVMem also supports mallocing the ram instead of utilizing shared memory, for use-cases where memory usage is only in-process.

There's probably more feature-rich libaries that serve similiar purposes, but this one is lean, familiar to me, and introduces zero additional dependencies.  I'm sharing it in case anyone has similiar requirements.  Licence is MIT.
