# tetrisgl
A OpenGL-based Tetris game controlled by Nintendo's Wiimote + Nunchuck.

## Required Libraries
Following libraries are required for the building process and their are available in Ubuntu repositories:
- CWiid
- SDL
- SDL_ttf
- SDL_mixer
- GL
- GLU
- GLEW
- Magick++ (NOT GraphicsMagick++!) 

### Linux Mint apt-get Calls and Build
- $ sudo apt-get install libmagick++-dev
- $ sudo apt-get install libglew1.6-dev
- $ sudo apt-get install libcwiid-dev libcwiimote-dev
- $ sudo mkdir /usr/local/share/tetrisgl
- $ cd src/data/
- $ cp -r * /usr/local/share/tetrisgl/
- $ g++ -O3 \`Magick++-config --cflags --cppflags\` \`sdl-config --cflags\` -o tetrisgl *.cpp \`Magick++-config --ldflags --libs\` \`sdl-config --libs\` -lSDL_ttf -lSDL_mixer -fpermissive -lGL -lGLEW -lGLU -lcwiid
