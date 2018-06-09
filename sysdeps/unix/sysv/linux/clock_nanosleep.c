/* Copyright (C) 2003-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <time.h>
#include <errno.h>

#include <sysdep-cancel.h>
#include "kernel-posix-cpu-timers.h"

/* We can simply use the syscall.  The CPU clocks are not supported
   with this function.  */
int
__clock_nanosleep (clockid_t clock_id, int flags, const struct timespec *req,
		   struct timespec *rem)
{
  INTERNAL_SYSCALL_DECL (err);
  int r;

  if (clock_id == CLOCK_THREAD_CPUTIME_ID)
    return EINVAL;
  if (clock_id == CLOCK_PROCESS_CPUTIME_ID)
    clock_id = MAKE_PROCESS_CPUCLOCK (0, CPUCLOCK_SCHED);

  if (SINGLE_THREAD_P)
    r = INTERNAL_SYSCALL (clock_nanosleep, err, 4, clock_id, flags, req, rem);
  else
    {
      int oldstate = LIBC_CANCEL_ASYNC ();

      r = INTERNAL_SYSCALL (clock_nanosleep, err, 4, clock_id, flags, req,
			    rem);

      LIBC_CANCEL_RESET (oldstate);
    }

  return (INTERNAL_SYSCALL_ERROR_P (r, err)
	  ? INTERNAL_SYSCALL_ERRNO (r, err) : 0);
}
weak_alias (__clock_nanosleep, clock_nanosleep)

/* 64-bit time version */

extern int __y2038_linux_support;

int
__clock_nanosleep64 (clockid_t clock_id, int flags,
		   const struct __timespec64 *req,
                   struct __timespec64 *rem)
{
  /* Until Linux provides a 64-bit syscall, use the 32-bit one */
  INTERNAL_SYSCALL_DECL (err);
  int r;
  struct timespec req32, rem32, *rem32p = NULL;

  if (req == NULL)
    {
      __set_errno(EFAULT);
      return -1;
    }

  if (req->tv_sec > INT32_MAX || req->tv_sec < INT32_MIN)
    {
      __set_errno(EOVERFLOW);
      return -1;
    }

  req32.tv_sec = req->tv_sec;
  req32.tv_nsec = req->tv_nsec;
  if (rem !=NULL) rem32p = &rem32;

  r = clock_nanosleep(clock_id, flags, &req32, rem32p);

  if (r == 0 && rem != NULL)
    {
      rem->tv_sec = rem32.tv_sec;
      rem->tv_nsec = rem32.tv_nsec;
    }

  return r;
}
