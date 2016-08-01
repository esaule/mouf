# this is now the default FreeType build for Android
#
ifndef USE_FREETYPE
USE_FREETYPE := 2.6.2
endif

#ifeq ($(USE_FREETYPE),2.6.2)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# compile in ARM mode, since the glyph loader/renderer is a hotspot
# when loading complex pages in the browser
#
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES:= \
    freetype/src/base/ftbbox.c \
    freetype/src/base/ftbitmap.c \
    freetype/src/base/ftfntfmt.c \
    freetype/src/base/ftfstype.c \
    freetype/src/base/ftglyph.c \
    freetype/src/base/ftlcdfil.c \
    freetype/src/base/ftstroke.c \
    freetype/src/base/fttype1.c \
    freetype/src/base/ftbase.c \
    freetype/src/base/ftsystem.c \
    freetype/src/base/ftinit.c \
    freetype/src/base/ftgasp.c \
    freetype/src/base/ftmm.c \
    freetype/src/gzip/ftgzip.c \
    freetype/src/raster/raster.c \
    freetype/src/sfnt/sfnt.c \
    freetype/src/smooth/smooth.c \
    freetype/src/autofit/autofit.c \
    freetype/src/truetype/truetype.c \
    freetype/src/cff/cff.c \
    freetype/src/psnames/psnames.c \
    freetype/src/pshinter/pshinter.c

LOCAL_C_INCLUDES += \
    jni/freetype/include \
    jni/png

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

LOCAL_SHARED_LIBRARIES += libpng libz

# the following is for testing only, and should not be used in final builds
# of the product
#LOCAL_CFLAGS += "-DTT_CONFIG_OPTION_BYTECODE_INTERPRETER"

LOCAL_CFLAGS += -O2

LOCAL_MODULE:= libft2

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
#endif
