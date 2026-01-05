@echo off
echo Building ChessGame...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
msbuild ChessGame.sln /t:Build /p:Configuration=Debug /p:Platform=x64 /p:VcpkgEnableManifest=true
if %errorlevel% neq 0 (
    echo Build Failed!
    pause
    exit /b %errorlevel%
)
echo Copying assets...
xcopy /Y /I /E "ChessGame\texture" "x64\Debug\texture"

echo Build Success! Launching game...
cd x64\Debug
start ChessGame.exe
