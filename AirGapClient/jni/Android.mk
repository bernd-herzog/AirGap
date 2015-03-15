LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -std=c99

SRC_PATH := ../../../../../Users/j39f3fs/Entwicklung/airgap/src/

LOCAL_MODULE    := AirGap
LOCAL_SRC_FILES := $(SRC_PATH)AirGapMain.cpp $(SRC_PATH)Depacketizer.c $(SRC_PATH)MemorySink.c 
LOCAL_SRC_FILES += $(SRC_PATH)BinarySlicer.c $(SRC_PATH)ClockRecovery.c $(SRC_PATH)QuadraturDemodulator.c 
LOCAL_SRC_FILES += $(SRC_PATH)Multiply.c $(SRC_PATH)FirFilter.c $(SRC_PATH)agmath.c $(SRC_PATH)ReedSolomon.c

LOCAL_EXPORT_CFLAGS := -DANDROID=1

include $(BUILD_SHARED_LIBRARY)
