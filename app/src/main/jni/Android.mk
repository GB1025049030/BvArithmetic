LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_MULTILIB := 32
LOCAL_MODULE    := libSmoothBlur
LOCAL_SRC_FILES := SmoothBlur.c
LOCAL_LDLIBS    := -lm -llog -ljnigraphics -ldl
#LOCAL_LDFLAGS := Wl,-hash-style=sysv
#LOCAL_LDFLAGS += -Wl,--hash-style=sysv
#libsysv-hash-table-library_ldflags := Wl,-hash-style=sysv

include $(BUILD_SHARED_LIBRARY)
