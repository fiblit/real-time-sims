# Author
Dalton Hildreth

# Build

directory structure:
/
  Headers/
  Libraries/
  Resources/
  Shaders/
  Sources/
  CMakeLists.txt
  README.md

To Init:
```
git --recursive clone https://github.com/fiblit/gg-engine.git
```

To Build:
```
cd Build
# UNIX Makefile
# Although, this might work anywhere.
cmake ..

# Mac OSX
# cmake -G "Xcode" ..

# Microsoft Windows
# cmake -G "Visual Studio 14" ..
# cmake -G "Visual Studio 14 Win64" ..
```
