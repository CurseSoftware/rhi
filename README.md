# C++ Template Project built using CMake
This is a cross-platform template C++ project using the CMake build system.

## Building
There are included `go.sh` and `go.ps` scripts in the project root directory.

Run the command `./go.sh Build-All` to run the build commands on Linux or MacOS.

Run the command `./go.ps Build-All` to run the build commands on Windows.

Alternatively, you can just run the build commands yourself:
```
cmake -B build .
cmake --build build
```
These commands place all build utilities CMake generates in the `build` directory which is preferable to cluttering the root dir.

## Using compile_commands.json
If you are using the clangd language server, CMake can generate a `compile_commands.json` file that works with clangd in order to allow the LSP to work.

Because we are placing CMake build outputs in our `build` directory, this places the json file there as well. 

On Linux you can run `ln -s build/compile_commands.json .` to create a symlink. This also means it will change as the source json file changes since it is a link.

On Windows I am gonna do that later lol.

## Tayloring to your preferences
This project is using default values for some names.

We use the name `assembly` as the name of the assembly we are building. If you wish to rename this, change this directories name to what you want.
You will also need to change this in the `CMakeLists.txt` file as well in the `project(...)` call at the beginning.

## Compiling the assembly as a library
The default target of compilation is currently an executable.

If you wish to compile to a library you should comment/remove the lines compiling as an executable, and uncomment the ones for the library.

**I am making a separate branch for library and executable builds soon!**

The default is a dynamic library, but you can change to STATIC library if you wish.
