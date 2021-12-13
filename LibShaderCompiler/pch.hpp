#pragma once

#ifdef LIBSHADERCOMPILER_IMPL
#define SC_API __declspec(dllexport) 
#else
#define SC_API __declspec(dllimport)
#endif // LIBSHADERCOMPILER_IMPL
