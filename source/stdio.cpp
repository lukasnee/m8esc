/*
 * m8ec - Embedded Client for the Dirtywave M8 Headless device.
 * Copyright (C) 2023 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#if defined(STM32H750xx)
#include "m8ec/periph/Uart4.hpp"
#elif defined(STM32F411xE)
#include "m8ec/periph/Uart1.hpp"
#endif

#include "SEGGER_SYSVIEW.h"
#include <reent.h> // required for _write_r
struct _reent;

#include <sys/stat.h>
#include <sys/unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

/**
 * @brief Standard output redirection to the screen
 */
extern "C" int _write(int fd, char *ptr, int len) {
    if ((fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }
#ifdef SEGGER_SYSVIEW_ENABLED
    SEGGER_SYSVIEW_PrintData(ptr, len);
#else // #ifdef SEGGER_SYSVIEW_ENABLED
#if defined(STM32H750xx)
    using SerialDebug = m8ec::periph::Uart4;
#elif defined(STM32F411xE)
    using SerialDebug = m8ec::periph::Uart1;
#endif
    if (!SerialDebug::get_instance().write(reinterpret_cast<std::uint8_t *>(ptr), len)) {
        FONAS_PANIC();
    }
#endif // #ifdef SEGGER_SYSVIEW_ENABLED
    return len;
}

extern "C" int _close(int fd) {
    UNUSED(fd);
    return -1;
}

extern "C" int _lseek(int fd, int ptr, int dir) {
    UNUSED(fd);
    UNUSED(ptr);
    UNUSED(dir);
    return 0;
}

extern "C" int _read(int fd, char *ptr, int len) {
    UNUSED(fd);
    UNUSED(ptr);
    return len;
}

extern "C" int _fstat(int fd, struct stat *st) {
    UNUSED(fd);
    memset(st, 0, sizeof(*st));
    return 0;
}

extern "C" int _isatty(int fd) {
    UNUSED(fd);
    return 1;
}

extern "C" int _getpid(void) { return 1; }

extern "C" int _kill(int pid, int sig) {
    UNUSED(pid);
    UNUSED(sig);
    errno = EINVAL;
    return -1;
}
