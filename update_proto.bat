echo off
rem: The intent of this batch is to regenerate the reach_proto/ansic/build directory
rem:  when modifying the proto file(s).  This isn't usually required, and 
rem:  it may take some setup to get protbufs to build from a windows batch.
rem: 
rem:  I run this from cygwin where the path is shortened to:
rem: $ echo $PATH
rem:  /usr/local/bin:/usr/bin:/cygdrive/c/Program Files/Git/cmd:/cygdrive/c/Program Files/CMake/bin:/cygdrive/c/Users/chuck.peplinski/AppData/Local/Programs/Python/Python310/Scripts:/cygdrive/c/Users/chuck.peplinski/AppData/Local/Programs/Python/Python310:/cygdrive/c/Users/chuck.peplinski/install2/protoc-24.2-win64/bin

set curdir=%cd%
set src=%cd%\reach_proto

cd %src%
cd proto 
echo Run 'python preprocess_options.py' to generate options file.
python preprocess_options.py

cd %src%\ansic\build

rem: With cygwin
echo Use cygin make to rebuild .pb files
make
rem:  echo With Microsoft
rem:  cmake --build .

echo copy from build to built
cp reach.pb.c ../built
cp reach.pb.h ../built

cd %curdir%
