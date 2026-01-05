@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64

echo Ensuring dependencies (including GTest) are installed...
msbuild ChessGame.sln /t:Build /p:Configuration=Debug /p:Platform=x64 /p:VcpkgEnableManifest=true
if %errorlevel% neq 0 (
    echo MSBuild failed!
    exit /b %errorlevel%
)

echo Running Tests...
powershell -ExecutionPolicy Bypass -File RunTests.ps1
