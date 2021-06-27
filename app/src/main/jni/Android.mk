LOCAL_PATH := $(call my-dir)
$(info $(LOCAL_PATH))

include $(CLEAR_VARS)

LOCAL_MODULE    := sfml-example

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/sources
LOCAL_C_INCLUDES += $(LOCAL_PATH)/myLibs/rapidjson-master/include

# Function to traverse directories and subdirectories
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALLFILES = $(call walk, $(LOCAL_PATH)/sources)

$(info $(ALLFILES))

LOCAL_SRC_FILES := $(filter %.cpp, $(ALLFILES))

LOCAL_SHARED_LIBRARIES := sfml-system-d
LOCAL_SHARED_LIBRARIES += sfml-window-d
LOCAL_SHARED_LIBRARIES += sfml-graphics-d
LOCAL_SHARED_LIBRARIES += sfml-audio-d
LOCAL_SHARED_LIBRARIES += sfml-network-d
LOCAL_SHARED_LIBRARIES += sfml-activity-d
LOCAL_SHARED_LIBRARIES += openal
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main-d

include $(BUILD_SHARED_LIBRARY)

$(call import-module,third_party/sfml)
