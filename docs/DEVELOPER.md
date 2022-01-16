# Developer Guide

## Building

### Dependencies

- `cmake`
- `msgpack`
- `websocketpp`
- `curl`
- `openssl`
- `jsoncpp`
- `wxWidgets`

### Windows

Building has been tested with `Visual Studio 2017` and the following guide
is written for it.

Install `conan`. Open `PowerShell` and add Bincrafters remote for `conan`:

```
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
```

and enable versioning if you haven't already:

```
conan config set general.revisions_enabled=1
```

- Open project in Visual Studio via `File -> Open -> CMake...` by choosing the
  `CMakeListst.txt`.
- Edit CMake options via `CMake -> Change CMake settings -> CMakelists.txt`
  - Change `generator` value to `Visual Studio 15 2017 Win64`.
  - Remove `-v` from `buildCommandArgs` or specify valid verbosity level.
- CMake will produce error, note down the path from the error:
  > include could not find load file:
  >
  > C:/Users/Test/CMakeBuilds/d76aad98-e5ad-0738-bbe9-bc20c6d93f9c/build/x64-Debug/conanbuildinfo.cmake`
- Open PowerShell in the source directory and run:
  - `conan install . -s build_type=Debug -if='<PATH_FROM_PREVIOUS_STEP>'`
- After installation finished go back to Visual Studio click `Generate` in the
  notification.
- After CMake generation finished you're ready to build.

When you're ready to build a release just replace `Debug` with `Release` in:
- CMakeSettings.json (`CMake -> Change CMake settings -> CMakelists.txt`)
- Replace all `Debug` parts of the `conan` install command.
