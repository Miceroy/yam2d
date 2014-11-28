set NDK_ROOT=C:\work\android-ndk-r10c\


xcopy ..\..\assets\* assets\*  /Y

call %NDK_ROOT%ndk-build.cmd


pause

