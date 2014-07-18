LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := bannerviewJNI
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := bannerjni.cpp \
                 Render.cpp \
                 tapCamera.cpp \
                 vecmath.cpp \
                 Banner.cpp \
                 GLProgram.cpp
LOCAL_LDLIBS    := -llog -lGLESv2 -lz

LOCAL_STATIC_LIBRARIES := jpeg
LOCAL_STATIC_LIBRARIES += png
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libjpeg
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libpng

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

include $(LOCAL_PATH)/libjpeg/Android.mk  $(LOCAL_PATH)/libpng/Android.mk
