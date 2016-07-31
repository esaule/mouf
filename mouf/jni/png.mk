LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -Ipng/

LOCAL_MODULE    := libpng
LOCAL_SRC_FILES :=\
	png/png.c \
	png/pngerror.c \
	png/pngget.c \
	png/pngmem.c \
	png/pngpread.c \
	png/pngread.c \
	png/pngrio.c \
	png/pngrtran.c \
	png/pngrutil.c \
	png/pngset.c \
	png/pngtrans.c \
	png/pngwio.c \
	png/pngwrite.c \
	png/pngwtran.c \
	png/pngwutil.c 
	
LOCAL_LDLIBS := -lz

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
