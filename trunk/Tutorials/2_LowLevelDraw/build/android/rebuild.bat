set NDK_ROOT=C:\work\android-ndk-r9c\

xcopy ..\..\assets\* assets\*  /Y

call %NDK_ROOT%ndk-build.cmd clean

call %NDK_ROOT%ndk-build.cmd

pause

