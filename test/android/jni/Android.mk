LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := flakor_test

LOCAL_MODULE_FILENAME := libflakor_test
#还可以在android.mk中设置NDK_MODULE_PATH在import语句之前加入，将一个新的路径加入NDK_MODULE_PATH变量。
$(call import-add-path,$(LOCAL_PATH)/../../../flakor)
    

LOCAL_SRC_FILES := classes/MainGame.cpp \
                   classes/TestScene.cpp \
                   main.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/classes \
                    $(LOCAL_PATH)/../../../flakor \
				    $(LOCAL_PATH)/../../../flakor/..

LOCAL_STATIC_LIBRARIES := flakor_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
