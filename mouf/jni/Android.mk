LOCAL_PATH := $(call my-dir)

include jni/png.mk
include jni/pixman.mk
include jni/cairo.mk
include jni/freetype.mk

# include $(CLEAR_VARS)
# LOCAL_MODULE := cairo
# LOCAL_SRC_FILES := include/libcairo.so
# include $(PREBUILT_SHARED_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := pixman
# LOCAL_SRC_FILES := include/libpixman.so
# include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

#CAIRO_ANDROID=$(HOME)/dev/android-cairo/
CAIRO_ANDROID=./

CAIRO_COMPILE=-I$(CAIRO_ANDROID)/jni/pixman/pixman -I$(CAIRO_ANDROID)/jni/cairo/src -I$(CAIRO_ANDROID)/jni/cairo-extra -I$(CAIRO_ANDROID)/jni/pixman-extra -Wno-missing-field-initializers -D CAIRO_HAS_PNG_FUNCTIONS=1 -Ijni/freetype/include

CAIRO_LINK=-L$(CAIRO_ANDROID)/libs/$(TARGET_ARCH_ABI) -lpixman -lcairo

VERSION_NUMBER=\"`git describe --tags``if git diff --exit-code > /dev/null ; then echo -n ; else echo \-dirty; fi`\"

LOCAL_MODULE    := mouf
LOCAL_CFLAGS    := -O2 -I. $(CAIRO_COMPILE) -DANDROID=1  -DVERSION_NUMBER=$(VERSION_NUMBER) -DCAIRO_HAS_FT_FONT=1
LOCAL_CPPFLAGS    := -O2 --std=c++11 -I. $(CAIRO_COMPILE) -DVERSION_NUMBER=$(VERSION_NUMBER) -DNOEXCEPTION
LOCAL_LDLIBS    := -lm -llog -landroid -lz
#LOCAL_SRC_FILES := shmup.cpp IA.cpp GameState.cpp GameState_serialize.cpp GameState_hash.cpp IA_brute.cpp SpriteCollection_cairo.cpp log.cpp
#LOCAL_SRC_FILES := shmup.cpp log.cpp  SpriteCollection_cairo.cpp Game.cpp Game-engine.cpp Game-render.cpp ShootingPattern.cpp ComplexObject.cpp type.cpp Game-IO.cpp graphics.cpp debug.cpp Trajectory.cpp Entity-IO.cpp Entity.cpp MovingPattern.cpp Condition.cpp Animation.cpp Player.cpp asset_ifstream.cpp input_android.cpp StringFinder.cpp config.cpp
LOCAL_SRC_FILES := shmup.cpp log.cpp  SpriteCollection_cairo.cpp type.cpp graphics.cpp debug.cpp  asset_ifstream.cpp input_android.cpp StringFinder.cpp config.cpp dictionary.cpp fontmanager.cpp
LOCAL_STATIC_LIBRARIES := android_native_app_glue # libcairo libpixman
LOCAL_SHARED_LIBRARIES := cairo pixman png ft2

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
