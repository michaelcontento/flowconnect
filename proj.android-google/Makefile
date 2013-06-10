run: clean-src debug logcat

deepclean: clean
	ant clean
	rm -rf obj/local/armeabi/*
	rm -rf libs/armeabi/libcocos2dcpp.so

clean: clean-src clean-libs

clean-src:
	rm -rf src/com/avalon
	rm -rf src/com/wenbin

clean-libs:
	rm -rf libs/*.jar

copy:
	# General
	cp -r ../Vendors/avalon/avalon/platform/android/ads/provider/java/* .
	cp -r ../Vendors/avalon/avalon/platform/android/ui/java/* .
	cp -r ../Vendors/avalon/avalon/platform/android/utils/java/* .
	# Google
	cp -r ../Vendors/avalon/avalon/platform/android-google/gamecenter/java/* .
	cp -r ../Vendors/avalon/avalon/platform/android-google/payment/java/* .
	cp -r ../Vendors/avalon/avalon/platform/android-google/ads/provider/java/* .

logcat:
	~/android-sdk-macosx/platform-tools/adb logcat

native:
	export NDK_ROOT=/Users/michaelcontento/Downloads/android-ndk-r8e-darwin-x86_64/android-ndk-r8e
	./build_native.sh

debug: copy native
	ant debug && ant installd

release: copy native
	ant release && ant installr
