// Copyright 2020 Matt Borland
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CACHE_SIZE_
#define CACHE_SIZE_

#include <cstdint>

// https://www.unix.com/man-page/freebsd/3/sysctlbyname/
// https://stackoverflow.com/questions/5446134/determine-values-of-several-system-variables-in-the-terminal-in-a-mac
#if defined(__APPLE__) || defined(_SYSTYPE_BSD)

#include <sys/sysctl.h>
#include <string>

namespace detail
{
inline std::size_t size(const std::string& name)
{
    std::size_t size {};
    std::size_t size_of_size {sizeof(size)};
    sysctlbyname(name.c_str(), &size, &size_of_size, 0, 0);
    return size;
}
}

inline std::size_t cache_line_size()
{
    return detail::size("hw.cachelinesize");
}

inline std::size_t L1I_cache_size()
{
    return detail::size("hw.l1icachesize");
}

inline std::size_t L1D_cache_size()
{
    return detail::size("hw.l1dcachesize");
}

inline std::size_t L2_cache_size()
{
    return detail::size("hw.l2cachesize");
}

inline std::size_t L3_cache_size()
{
    return detail::size("hw.l3cachesize");
}

// https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-system_logical_processor_information
// https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-cache_descriptor
#elif defined(_WIN32)

#include <stdlib.h>
#include <windows.h>
std::size_t cache_line_size()
{
    std::size_t line_size {};
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = nullptr;

    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) 
    {
	    if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
        {
	        line_size = buffer[i].Cache.LineSize;
	        break;
	    }
    }

    free(buffer);
    return line_size;
}

namespace detail
{
std::size_t cache_size(const int level)
{
    std::size_t size {};
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = nullptr;

    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) 
    {
        if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == level)
        {
 	        size = buffer[i].Cache.Size;
	        break;
	    }
    }

    free(buffer);
    return line_size;
}
}

// Windows does not offer a method of differentiating L1I and L1D
inline std::size_t L1I_cache_size()
{
    return detail::cache_size(1);
}

inline std::size_t L1D_cache_size()
{
    return detail::cache_size(1);
}

inline std::size_t L2_cache_size()
{
    return detail::cache_size(2);
}

inline std::size_t L3_cache_size()
{
    return detail::cache_size(3);
}

// https://www.man7.org/linux/man-pages/man3/sysconf.3.html
#elif defined(__linux__)

#include <unistd.h>

inline std::size_t cache_line_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL1_DCACHE_LINESIZE));
}

inline std::size_t L1I_cache_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL1_ICACHE_SIZE));
}

inline std::size_t L1D_cache_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL1_DCACHE_SIZE));
}

inline std::size_t L2_cache_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL2_CACHE_SIZE));
}

inline std::size_t L3_cache_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL3_CACHE_SIZE));
}

// Used for POWER arch
inline std::size_t L4_cache_size()
{
    return static_cast<std::size_t>(sysconf(_SC_LEVEL4_CACHE_SIZE));
}

#else
#error Unrecognized platform
#endif
#endif // CACHE_SIZE_
