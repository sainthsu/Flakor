LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := flakor
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := \
main.cpp \
Flakor.cpp \
Engine.cpp \
Application.cpp \
base/lang/Array.cpp \
base/lang/AutoreleasePool.cpp \
base/lang/Bool.cpp \
base/lang/DataVisitor.cpp \
base/lang/Dictionary.cpp \
base/lang/Object.cpp \
base/lang/Set.cpp \
base/lang/String.cpp \
base/lang/Zone.cpp \
base/element/Color.cpp \
base/element/Point.cpp \
base/element/Rect.cpp \
base/element/Size.cpp \
base/entity/Entity.cpp \
base/entity/Scene.cpp \
base/entity/Sprite.cpp \
math/Camera.cpp \
math/CArray.cpp \
math/Matrices.cpp \
math/MatrixStack.cpp \
core/opengl/GLProgram.cpp \
core/vbo/VBO.cpp \
core/shader/Shader.cpp \
core/texture2D.cpp \

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -lz \
                       -landroid

LOCAL_LDLIBS    := -llog -lGLESv2 -lz


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/. \
                    $(LOCAL_PATH)/.. \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/support/libjpeg 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/support/libpng

LOCAL_STATIC_LIBRARIES := jpeg
LOCAL_STATIC_LIBRARIES += png
LOCAL_CPP_EXTENSION := .cxx .cpp .cc
# define the macro to compile through support/zip_support/ioapi.c
LOCAL_CFLAGS   :=  -DUSE_FILE32API
LOCAL_CPPFLAGS := -Wno-deprecated-declarations -Wno-extern-c-compat
LOCAL_EXPORT_CFLAGS   := -DUSE_FILE32API
LOCAL_EXPORT_CPPFLAGS := -Wno-deprecated-declarations -Wno-extern-c-compat

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_NEON  := true
endif

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

include $(LOCAL_PATH)/libjpeg/Android.mk  $(LOCAL_PATH)/libpng/Android.mk
