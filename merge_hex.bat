@echo off
rem PATH_SCMD=C:\SiliconLabs\SimplicityStudio\v5\developer\adapter_packs\commander
rem pwd

set BOOTLOADER=..\bootloader\bootloader-apploader.hex
set APP=reacher-pb.s37

@echo on
%PATH_SCMD%\commander.exe convert %BOOTLOADER% %APP% -o reacher-tboard-v.hex

