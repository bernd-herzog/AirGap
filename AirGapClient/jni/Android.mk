LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -std=c99


LOCAL_MODULE    := AirGap
LOCAL_SRC_FILES := src/AirGapMain.cpp src/Depacketizer.c src/MemorySink.c src/BinarySlicer.c src/ClockRecovery.c src/QuadraturDemodulator.c src/Multiply.c src/FirFilter.c src/agmath.c src/ReedSolomon.c

include $(BUILD_SHARED_LIBRARY)
