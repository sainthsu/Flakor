LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := flakor_static
LOCAL_MODULE_FILENAME := libflakor

LOCAL_CFLAGS    := -Werror

BASE_PATH = $(LOCAL_PATH)/../

LOCAL_SRC_FILES := \
Flakor.cpp \
platform/Game.cpp \
platform/android/Engine.cpp \
platform/android/Application.cpp \
platform/android/ActivityCallback.cpp \
include/common.cpp \
base/lang/Object.cpp \
base/lang/Array.cpp \
base/lang/AutoreleasePool.cpp \
base/lang/DataVisitor.cpp \
base/lang/Dictionary.cpp \
base/lang/Set.cpp \
base/lang/Str.cpp \
base/lang/Zone.cpp \
base/element/Color.cpp \
base/element/Element.cpp \
base/element/Helper.cpp \
base/element/Blendfunc.cpp \
math/Camera.cpp \
math/CArray.cpp \
math/Matrices.cpp \
math/MatrixStack.cpp \
math/GLMatrix.cpp \
core/resource/BitData.cpp \
core/resource/Uri.cpp \
core/resource/LoaderThread.cpp \
core/resource/Resource.cpp \
core/resource/Image.cpp \
core/resource/ImageLoader.cpp \
core/resource/ResourceManager.cpp \
core/input/Touch.cpp \
core/opengl/GPUInfo.cpp \
core/opengl/GLProgram.cpp \
core/opengl/vbo/VBO.cpp \
core/opengl/shader/Shaders.cpp \
core/opengl/texture/atitc.cpp \
core/opengl/texture/etc1.cpp \
core/opengl/texture/pvr.cpp \
core/opengl/texture/s3tc.cpp \
core/opengl/texture/TGAlib.cpp \
core/opengl/texture/Texture2D.cpp \
tool/utility/TexUtils.cpp \
2d/Entity.cpp \
2d/Scene.cpp \
2d/Sprite.cpp \

LOCAL_EXPORT_LDLIBS := -lGLESv1_CM \
                       -lGLESv2 \
                       -lEGL \
                       -llog \
                       -lz \
                       -landroid

LOCAL_LDLIBS    := -lGLESv1_CM \
                   -lGLESv2 \
                   -lEGL \
                   -llog \
                   -lz \
                   -landroid


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/. \
                           $(LOCAL_PATH)/.. \
                           $(LOCAL_PATH)/base/element \
                           $(LOCAL_PATH)/platform \
                           $(LOCAL_PATH)/base/lang \
                           $(LOCAL_PATH)/base/interface \
                           $(LOCAL_PATH)/include \
					$(LOCAL_PATH)/core/opengl/ \
					$(LOCAL_PATH)/core/opengl/vbo \
					$(LOCAL_PATH)/core/opengl/shader \
					$(LOCAL_PATH)/core/texture \
					$(LOCAL_PATH)/math \
					$(LOCAL_PATH)/support/libjpeg \
					$(LOCAL_PATH)/support/libpng

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/. \
                    $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/base/element \
					$(LOCAL_PATH)/base/entity \
					$(LOCAL_PATH)/base/lang \
					$(LOCAL_PATH)/base/interface \
					$(LOCAL_PATH)/include \
					$(LOCAL_PATH)/core/opengl/ \
					$(LOCAL_PATH)/core/opengl/vbo \
					$(LOCAL_PATH)/core/opengl/shader \
					$(LOCAL_PATH)/core/texture \
					$(LOCAL_PATH)/math \
					$(LOCAL_PATH)/support/libjpeg \
					$(LOCAL_PATH)/support/libpng

LOCAL_STATIC_LIBRARIES := jpeg
LOCAL_STATIC_LIBRARIES += png
LOCAL_CPP_EXTENSION := .cxx .cpp .cc
# define the macro to compile through support/zip_support/ioapi.c
#LOCAL_CFLAGS   :=  -DUSE_FILE32API
#LOCAL_CPPFLAGS := -Wno-deprecated-declarations -Wno-extern-c-compat
#LOCAL_EXPORT_CFLAGS   := -DUSE_FILE32API
#LOCAL_EXPORT_CPPFLAGS := -Wno-deprecated-declarations -Wno-extern-c-compat

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_NEON  := true
endif

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

include $(LOCAL_PATH)/support/libjpeg/Android.mk  $(LOCAL_PATH)/support/libpng/Android.mk
