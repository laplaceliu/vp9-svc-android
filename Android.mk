LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include libvpx/build/make/Android.mk

include $(CLEAR_VARS)

LOCAL_MODULE    := vp9_svc_encoder

LOCAL_C_INCLUDES := $(LOCAL_PATH)/libvpx

LOCAL_SRC_FILES := vp9_svc_encoder.cpp

LOCAL_SHARED_LIBRARIES := vpx

include $(BUILD_SHARED_LIBRARY)
