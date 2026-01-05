# Chess Game (C++ / SDL3)

A classic Chess Game implemented in C++ using the SDL3 library.

## Features
- Full Chess Logic (Movement, Captures, Check/Checkmate, En Passant, Castling, Promotion)
- Graphical User Interface using SDL3
- Two-player local mode

## Prerequisites

To build and run this project, you need:

1.  **Visual Studio 2022** (Community, Professional, or Enterprise)
2.  **Desktop development with C++** workload installed.
3.  **Git** (for cloning and vcpkg).

*Note: Dependencies (SDL3, SDL3_image) are automatically managed by `vcpkg` during the build process.*

## Quick Start

The easiest way to run the game is using the provided script:

1.  Open a terminal in the project folder.
2.  Run the script:
    ```cmd
    .\Play.bat
    ```
    This will automatically build the project (downloading dependencies if needed) and launch the game.

## Manual Build

If you prefer to build manually from the command line:

```powershell
msbuild ChessGame.sln /t:Build /p:Configuration=Debug /p:Platform=x64
```

Then run the executable:
```powershell
cd x64\Debug
Start ChessGame.exe
```

## Project Structure

- `ChessGame/`: Source code (.cpp, .h) and Assets (`texture/`)
- `vcpkg.json`: Dependency manifest
- `Play.bat`: One-click build and run script
