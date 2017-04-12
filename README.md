# net4
.NET Framework 4.0 RPM/WPM bindings for Love2D/LuaJIT. This was made specifically to read and write to application memory for windows for my own personal debugging purposes (I wanted Love2D for a GUI). This isn't a full library, I only have wrapped functions which I needed and exposed some light userdata. You need a decent understanding of the lua-c library to use it properly.

Compilation:
```cmake -G "Visual Studio 14" -H. -Bbuild -DCMAKE_GENERATOR_PLATFORM=x64
cmake --build build --target net4 --config Release
```

If it doesn't work then ¯\_(ツ)_/¯   _

Maybe you need to link the Love2D libraries with it on compilation (I really don't remember, I wrote this library a year ago and I am not used to compiling projects with so many dependencies). Anyway if you look at how libraries for Love2D are compiled on its mega source then that should help you.

To be honest, after I learnt about the FFI library for LuaJIT I really wish I wrote this library in lua rather than a cpp compiled linked library.
