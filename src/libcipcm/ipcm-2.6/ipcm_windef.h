#ifndef IPCM_WINDEF_H
#define IPCM_WINDEF_H

#if (defined(__MINGW32__) || defined(__MINGW64__)) || defined(_MSC_VER)
#define OK_WIN_BUILD

#ifdef USE___UUIDOF
#undef USE___UUIDOF
#endif
#define USE___UUIDOF 0

#endif

#endif
