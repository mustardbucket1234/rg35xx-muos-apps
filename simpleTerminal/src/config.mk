#
#	Simple Terminal config for RG35xx MinUI
#		based on RG350 ver https://github.com/jamesofarrell/st-sdl
#

# st version
VERSION = 0.3

# Customize below to fit your system

# paths
PREFIX = /opt/trimuismart
MANPREFIX = ${PREFIX}/share/man
CROSS_COMPILE ?= ${PREFIX}/bin/arm-linux-gnueabihf-

# compiler and linker
CC = ${CROSS_COMPILE}gcc
SYSROOT = $(shell ${CC} --print-sysroot)

# includes and libs
INCS = -I. -I${SYSROOT}/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -DRG35XX
LIBS = -lc -L${SYSROOT}/usr/lib -lSDL -lpthread -Wl,-Bstatic,-lutil,-Bdynamic

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -fPIC
CFLAGS += -Os -Wall ${INCS} ${CPPFLAGS} -fPIC -std=gnu11 -marm -mtune=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -march=armv7-a+neon-fp16 -flto
LDFLAGS += ${CFLAGS} ${LIBS} -lSDL -s
