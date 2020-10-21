@echo off

set SRC=%~dp0
cd /d %SRC%

call :Fn_Format_All 1st\source
rem call :Fn_Format_All Test
goto :End


:Fn_Format_All
call :Fn_Format %~1 "*.hpp"
call :Fn_Format %~1 "*.cpp"
call :Fn_Format %~1 "*.h"
call :Fn_Format %~1 "*.c"
exit /b

:Fn_Format
set DirName=%~1
set ExtName=%~2
setlocal ENABLEDELAYEDEXPANSION
 for /F "usebackq" %%a in (`dir /a-d /s /b %SRC%%DirName%\%ExtName%`) do (
	call :Fn_Format_Inner "%%a" "%DirName%"
)
exit /b


:Fn_Format_Inner
set FileName=%~1
clang-format -style=file -i %FileName%
echo %FileName%
exit /b


:End
pause