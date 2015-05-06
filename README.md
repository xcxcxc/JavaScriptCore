# JavaScriptCore

The JavaScriptCore library is part of the [WebKit project](http://www.webkit.org/) and thus Open Source. However, in the sources you get from the [WebKit SVN](https://svn.webkit.org/repository/webkit/trunk), the XCode project files are curiously missing an iOS compile target. The sources you get from [opensource.apple.com](http://opensource.apple.com/release/ios-601/) are missing the project files altogether. You can't compile it at all. 

This repo aims to re-produce the missing iOS targets while staying on a somewhat up-to-date version.


## How to Compile for iOS

1. Run `python make.py` under Source.
2. Get coffee! Building this takes a while ;P

You can do `python make.py --help` for more options.

## How to Compile for Android
1. Install CMake 
2. Set ANDROID_NDK_ROOT variable to you android ndk path
	export ANDROID_NDK_ROOT=path/to/ndk
3. mkdir build_android && cd build_android
4. Run CMake Command (make sure you have perf, bison, python, ruby and perl installed)
	```
    cmake -DCMAKE_TOOLCHAIN_FILE="../android_toolchain/android.toolchain.cmake" \
          --platform=android-14 -DANDROID_ABI=armeabi -DCMAKE_BUILD_TYPE=MinSizeRel \
          -DANDROID=TRUE -DANDROID_STL=gnustl_static \
          -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
          -DANDROID_NATIVE_API_LEVEL=14 -DENABLE_WEBKIT=0 -DENABLE_WEBCORE=0 \
          -DENABLE_TOOLS=0 -DENABLE_WEBKIT=0 -DENABLE_WEBKIT2=0 \
          -DENABLE_API_TESTS=0 -DPORT=Android \
          -DCMAKE_FIND_ROOT_PATH="$(pwd)/../icu_build_android" \
          -DPYTHON_EXECUTABLE=$(which python) -DPERL_EXECUTABLE=$(which perl) \
          -DRUBY_EXECUTABLE=$(which ruby) -DBISON_EXECUTABLE=$(which bison) \
          -DGPERF_EXECUTABLE=$(which Gperf) \
          -DENABLE_LLINT=1 -DSHARED_CORE=0 -DENABLE_PROMISES=1 \
          -DENABLE_INSPECTOR=0 -DENABLE_JIT=0 -DEXPORT_ONLY_PUBLIC_SYMBOLS=1 \
          -DCMAKE_CXX_FLAGS="-D__STDC_LIMIT_MACROS" \
          -DCMAKE_INSTALL_PREFIX=../AndroidModulesRelease/JavaScriptCore \
          ..
    ```
    one liner:
    ```
    cmake -DCMAKE_TOOLCHAIN_FILE="../android_toolchain/android.toolchain.cmake" --platform=android-14 -DANDROID_ABI=armeabi -DCMAKE_BUILD_TYPE=MinSizeRel -DANDROID=TRUE -DANDROID_STL=gnustl_static -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 -DANDROID_NATIVE_API_LEVEL=14 -DENABLE_WEBKIT=0 -DENABLE_WEBCORE=0 -DENABLE_TOOLS=0 -DENABLE_WEBKIT=0 -DENABLE_WEBKIT2=0 -DENABLE_API_TESTS=0 -DPORT=Android -DCMAKE_FIND_ROOT_PATH="$(pwd)/../icu_build_android" -DPYTHON_EXECUTABLE=$(which python) -DPERL_EXECUTABLE=$(which perl) -DRUBY_EXECUTABLE=$(which ruby) -DBISON_EXECUTABLE=$(which bison) -DGPERF_EXECUTABLE=$(which Gperf) -DENABLE_LLINT=1 -DSHARED_CORE=0 -DENABLE_PROMISES=1 -DENABLE_INSPECTOR=0 -DENABLE_JIT=0 -DEXPORT_ONLY_PUBLIC_SYMBOLS=1 -DCMAKE_CXX_FLAGS="-D__STDC_LIMIT_MACROS" -DCMAKE_INSTALL_PREFIX=../AndroidModulesRelease/JavaScriptCore ..
    ```
