$VCPKG_ROOT = ".\vcpkg_installed\x64-windows\x64-windows"
$INCLUDE_PATH = "$VCPKG_ROOT\include"
$LIB_PATH = "$VCPKG_ROOT\lib" 
# Use lib for release or debug\lib for debug. Since we use /MDd (debug runtime), we should use debug libs ideally.
$LIB_PATH_DEBUG = "$VCPKG_ROOT\debug\lib"

# Gather source files (exclude main.cpp)
$sources = Get-ChildItem -Path "ChessGame\*.cpp" -Exclude "main.cpp" | ForEach-Object { $_.FullName }
$testSources = Get-ChildItem -Path "tests\*.cpp" | ForEach-Object { $_.FullName }

# Output exe
$exePath = "tests\run_tests.exe"

# Add DLLs to PATH
$env:PATH = "$PWD\$VCPKG_ROOT\debug\bin;" + $env:PATH
# Also add x64\Debug where game DLLs might be
$env:PATH = "$PWD\x64\Debug;" + $env:PATH

# Note: *.cpp includes TestFEN.cpp automatically.
Write-Host "Compiling and Linking Tests..."
# Note: Linking against debug libs
$cmd = "cl.exe /std:c++17 /EHsc /MDd /I`"$INCLUDE_PATH`" /I`"ChessGame`" $sources $testSources /Fe`"$exePath`" /link /LIBPATH:`"$LIB_PATH_DEBUG`" /LIBPATH:`"$LIB_PATH_DEBUG\manual-link`" /LIBPATH:`"$LIB_PATH`" gtest.lib gtest_main.lib sdl3.lib sdl3_image.lib shell32.lib"

cmd /c $cmd

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build Successful. Running Tests..."
    & $exePath
}
else {
    Write-Host "Build Failed. Exit Code: $LASTEXITCODE"
}
