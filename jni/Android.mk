LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := card2digit
LOCAL_SRC_FILES := card2digit.cpp svm.cpp svm-predict.c
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)
