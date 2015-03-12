LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -std=c99


LOCAL_MODULE    := AirGap
LOCAL_SRC_FILES := AirGapMain.cpp ../jni-src/Depacketizer.c ../jni-src/MemorySink.c BinarySlicer.c ClockRecovery.c QuadraturDemodulator.c Multiply.c FirFilter.c agmath.c ReedSolomon.c

include $(BUILD_SHARED_LIBRARY)
