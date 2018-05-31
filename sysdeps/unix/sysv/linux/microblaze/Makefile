ifeq ($(subdir),resource)
sysdep_routines += backtrace_linux
endif

ifeq ($(subdir),misc)
sysdep_routines += y2038-dummy
endif

ifeq ($(subdir),nptl)
# pull in __syscall_error routine
libpthread-routines += sysdep
libpthread-shared-only-routines += sysdep
endif
