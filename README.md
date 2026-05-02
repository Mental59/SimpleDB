# SimpleDB

Minimal C project configured with CMake and Clang.

## Requirements

- CMake 3.20+
- Clang
- Ninja

## Build

Debug:

```powershell
cmake --preset debug
cmake --build --preset build-debug
```

Release:

```powershell
cmake --preset release
cmake --build --preset build-release
```

All build artifacts are generated under the `build/` folder.

## Run

Debug executable:

```powershell
.\build\debug\simpledb.exe
```

Release executable:

```powershell
.\build\release\simpledb.exe
```
