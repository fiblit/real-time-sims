Name: Dalton Hildreth
ID: ***REMOVED***
email: hildr039@umn.edu

----------
Resources:
I used [Glitter](http://polytonic.github.io/Glitter/) to get the boilerplate of OpenGL/GLFW working.
I didn't use Bullet, but I used every other library from it.

I used http://learnopengl.com to, well, learn OpenGL, so some of his code is in mine to do rendering.

----------
To Init:
```
git --recursive c
```

To Update & Build
```
git submodule update --init
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