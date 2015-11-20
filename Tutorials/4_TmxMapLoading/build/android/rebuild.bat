
xcopy ..\..\assets\* assets\*  /Y

call %NDK_ROOT%ndk-build.cmd clean

call %NDK_ROOT%ndk-build.cmd

pause

