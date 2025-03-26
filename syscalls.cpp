/**
 * syscalls
 * Created on: 03/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include <sys/time.h>

#include "stdout.hpp"

extern "C" {

int _gettimeofday_r(struct _reent*, struct timeval* tp, void*)
{
    tp->tv_sec  = 0;
    tp->tv_usec = 0;
    return 0;
}

/**
 *****************************************************************************
 **
 **  File        : syscalls.c
 **
 **  Abstract    : System Workbench Minimal System calls file
 **
 ** 		          For more information about which c-functions
 **                need which of these lowlevel functions
 **                please consult the Newlib libc-manual
 **
 **  Environment : System Workbench for MCU
 **
 **  Distribution: The file is distributed �as is,� without any warranty
 **                of any kind.
 **
 **  (c)Copyright System Workbench for MCU.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. Distribution of this file (unmodified or modified) is not
 **  permitted. System Workbench for MCU permit registered System Workbench(R) users the
 **  rights to distribute the assembled, compiled & linked contents of this
 **  file as part of an application binary file, provided that it is built
 **  using the System Workbench for MCU toolchain.
 **
 *****************************************************************************
 */

/* Includes */
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

/* Variables */
// #undef errno
extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

char*  __env[1] = {0};
char** environ  = __env;

/* Functions */
void initialise_monitor_handles() { }

int _getpid(void) { return 1; }

int _kill(int, int)
{
    errno = EINVAL;
    return -1;
}

[[noreturn]] void _exit(int status)
{
    _kill(status, -1);
    while (true) { } /* Make sure we hang here */
}

int _wait(int*)
{
    errno = ECHILD;
    return -1;
}

int _times(tms*) { return -1; }

int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

int _execve(char*, char**, char**)
{
    errno = ENOMEM;
    return -1;
}
}