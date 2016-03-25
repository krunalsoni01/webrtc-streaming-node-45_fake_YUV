LOCAL_PATH := $(call my-dir)
#LOCAL_PATH1 := /home/infosys/ORTC/ortc-lib/libs
#ANDROIDNDK_PATH := $(LOCAL_PATH)/../boost/android-ndk-r8e
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS	:= -Wall \
-W \
-O2 \
-pipe \
-fPIC \
-fexceptions \
-D_ANDROID \
-DNDEBUG \

LOCAL_MODULE    := websocketpp

LOCAL_C_INCLUDES:=$(LOCAL_PATH) \
/home/infosys/ORTC/ortc-lib/libs/boost/projects/android/build/include/boost-1_53 \
#/home/infosys/ORTC/android-ndk-r8e)/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/include \
#/home/infosys/ORTC/android-ndk-r8e/sources/cxx-stl/gnu-libstdc++/4.4.3/include \

LOCAL_SRC_FILES := \
src/sha1/sha1.cpp \
src/base64/base64.cpp \
src/md5/md5.c \
src/processors/hybi_header.cpp \
src/processors/hybi_util.cpp \
src/messages/data.cpp \
src/uri.cpp \
src/network_utilities.cpp \

LOCAL_LDLIBS := -L/home/infosys/b022_fresh/oct30/lib-rtcweb-client/NativeClient/third_party/websocketpp/lib


LOCAL_LDLIBS    += \
		-llog \
		-lc \
		-lstdc++ \
		-lm \
		-lboost_atomic-gcc-mt-1_53 \
		-lboost_graph-gcc-mt-1_53 \
		-lboost_iostreams-gcc-mt-1_53 \
		-lboost_filesystem-gcc-mt-1_53 \
		-lboost_program_options-gcc-mt-1_53 \
		-lboost_signals-gcc-mt-1_53 \
		-lboost_regex-gcc-mt-1_53 \
		-lboost_system-gcc-mt-1_53 \
		-lboost_random-gcc-mt-1_53 \
		-lboost_thread-gcc-mt-1_53 \
		-lboost_date_time-gcc-mt-1_53 \
		
#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

