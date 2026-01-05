# How to Run the Chess Game

This guide provides simple instructions on how to build and play the Chess Game on Windows.

## Option 1: The "One-Click" Script (Recommended)

We have created a helper script called `Play.bat` that handles everything for you.

1.  **Open your Terminal** (Command Prompt, PowerShell, or the IDE Terminal).
2.  **Navigate** to the `ChessGame` folder.
3.  **Type** the following command and press **Enter**:

    ```cmd
    .\Play.bat
    ```

**What this script does:**
- It sets up the visual studio build environment.
- It downloads and compiles SDL3 (if not already done).
- It compiles the game code.
- It copies the necessary `texture` assets to the correct folder.
- It launches the game.

---

## Option 2: Visual Studio Code / Antigravity IDE

If you are using an IDE:

1.  Open the Terminal (Ctrl+` or View -> Terminal).
2.  Type `.\Play.bat` and hit Enter.

---

## Option 3: Manual Command Line

If you want to do it step-by-step:

1.  **Build**:
    ```cmd
    msbuild ChessGame.sln /t:Build /p:Configuration=Debug /p:Platform=x64 /p:VcpkgEnableManifest=true
    ```

2.  **Copy Assets** (Important!):
    Make sure the `texture` folder inside `ChessGame` is copied to `x64\Debug`.
    ```cmd
    xcopy /Y /I /E "ChessGame\texture" "x64\Debug\texture"
    ```

3.  **Run**:
    ```cmd
    cd x64\Debug
    ChessGame.exe
    ```

## Troubleshooting

- **"Texture not initialized!"**: This means the game can't find the images. Make sure you use `Play.bat` or manually copy the `texture` folder to the same folder as `ChessGame.exe`.
- **"SDL3/SDL.h not found"**: This generally happens if `vcpkg` didn't run. Re-run the build command, and it should trigger vcpkg.
