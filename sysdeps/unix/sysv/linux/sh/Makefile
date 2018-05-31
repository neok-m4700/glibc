ifeq ($(subdir),io)
sysdep_routines += pipe
endif

ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif

ifeq ($(subdir),misc)
gen-as-const-headers += sigaltstack-offsets.sym
sysdep_routines += y2038-dummy
endif

ifeq ($(subdir),math)
# The libm.so link can't find __fpscr_values
libm.so-no-z-defs = yes
endif
