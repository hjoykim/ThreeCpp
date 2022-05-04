@REM This is a generated file.
@echo off
setlocal
SET srcdir="E:\OpenGLWorkSpace\pcre-8.44"
SET pcretest="E:\OpenGLWorkSpace\pcre-8.44\build\DEBUG\pcretest.exe"
if not [%CMAKE_CONFIG_TYPE%]==[] SET pcretest="E:\OpenGLWorkSpace\pcre-8.44\build\%CMAKE_CONFIG_TYPE%\pcretest.exe"
call %srcdir%\RunTest.Bat
if errorlevel 1 exit /b 1
echo RunTest.bat tests successfully completed
