
VERSION = 0.3


# paths
OUTPUT_DIR = output/arm7
ASSETS_DIR = assets
SRC_DIR = src
OBJ_DIR = $(OUTPUT_DIR)/obj


PREFIX = /opt/rg35xx
MANPREFIX = ${PREFIX}/share/man
CROSS_COMPILE ?= ${PREFIX}/bin/arm-linux-

# compiler and linker
CC = ${CROSS_COMPILE}gcc
SYSROOT = $(shell ${CC} --print-sysroot)

# includes and libs
INCS = -I. -I${SYSROOT}/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -DRG35XX
LIBS = -lc -L${SYSROOT}/usr/lib -lSDL -lSDL_ttf -lpthread -Wl,-Bstatic,-lutil,-Bdynamic

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -fPIC
CFLAGS += -Os -Wall ${INCS} ${CPPFLAGS} -fPIC -std=gnu11 -marm -mtune=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -march=armv7-a+neon-fp16 -flto
LDFLAGS += ${CFLAGS} ${LIBS} -lSDL -s
