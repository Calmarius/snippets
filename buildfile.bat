@ECHO off

IF NOT DEFINED INCLUDE (
    ECHO You must run vcvarsall.bat in your Visual Studio build tools, before you can use this script.
    GOTO eof
)

IF [%1] == [] (
    ECHO You need to provide the file to compile.
    GOTO eof
)

SET FILENAME=%1


cl -I %CD% -W4 -WX -DUNIT_TEST -Zi -Fetestprogram.exe %FILENAME%

:eof
