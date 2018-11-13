# test-locale project

It seems the use of `locale` in C code varies between MS Windows and UNIX, and these are some experiments to explore that...

## The Apps

There are presently four (4) console apps - enum-locale, show-std-locale, test-locale, test-locale2, although enum-locale is only coded for WIN32...

As stated is really only about testing and experimenting to see what is possible...

### enum-locale

This is presently for WIN32 only, since it uses the API `EnumSystemLocalesEx(LOCALE_ENUMPROCEX,DWORD,LPARAM,LPVOID)` to enumerate, and display **all** the value sent to the callback...

UNIX has not yet been coded, but understand maybe `$ locale -a` does enumerate installed languages... 

### show-std-locale

This displays the string name returned from `std::locale().name();` and `std::locale("").name();...

### test-locale

This uses `setlocal` to set various languages, and then output a smaple number, and date...

### test-locale2

Similar to above, uses `setlocal` in vaious ways, and shows sample output...

## The Build

Git clone, or download and unzip zip source.

This is a [cmake](https://cmake.org/install/) project. CMake can generate a considerable number of "Build Systems". Use `cmake --help` to see the list.

Change into the `build` directory. There is a `build-me.bat` and a `build-me.sh` that may be suitable with minimal modifications, if they suit your system, else manually as follows...

### Build LINUX

If the `Build System` is `Unix Makefiles`...

```
$ cd build
$ cmake .. [-DCMAKE_INSTALL_PREFIX=/usr] [-DCMAKE_BUILD_TYPE=Release] [-G "Build System"]...
$ make
$ make install (if desired)
```

### Build Windows

If the `Build System` is a `Visual Studio %VCVERS% [Win64]`...

```
cd build
cmake .. [-DCMAKE_INSTALL_PREFIX=/dst/path] [-DCMAKE_BUILD_TYPE=Release] [-G "Build System"]...
cmake --build . --config Release
cmake --build . --config Release --target INSTALL (if desired)
```

## The License

GNU GPL v2 - see LICENSE.txt

20181112.
