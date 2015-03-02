LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -std=c99

LOCAL_MODULE    := AirGap
LOCAL_SRC_FILES := AirGap.cpp Depacketizer.c AudioSource_Alsa.c FileSink.c BinarySlicer.c ClockRecovery.c QuadraturDemodulator.c Multiply.c FirFilter.c agmath.c ReedSolomon.c

include $(BUILD_SHARED_LIBRARY)
