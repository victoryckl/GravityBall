APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1
#for android-ndk-r9
APP_CPPFLAGS += -Wno-error=format-security