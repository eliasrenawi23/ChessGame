@echo off
echo Building ChessGame (Build Only)...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64
msbuild ChessGame.sln /t:Build /p:Configuration=Debug /p:Platform=x64 /p:VcpkgEnableManifest=true
if %errorlevel% neq 0 (
    echo Build Failed!
    exit /b %errorlevel%
)
echo Build Success!
