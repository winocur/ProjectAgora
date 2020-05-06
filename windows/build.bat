g++ -m64 -w ../src/sdl_agora.cpp -o ../bin/agora_windows ^
-I../libs/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2 ^
-I../libs/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2 ^
-I../libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include/SDL2 ^
-L../libs/SDL2-2.0.12/x86_64-w64-mingw32/lib ^
-L../libs/SDL2_image-2.0.5/x86_64-w64-mingw32/lib ^
-L../libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib ^
-g -lopengl32 -lGLU32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -fno-exceptions
