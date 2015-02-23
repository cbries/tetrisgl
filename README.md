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

## Basic Installation
- $ tar xfvz tetris-x.x.x.tar.gz
- $ cd tetrisgl
- $ ./configure
- $ make
- $ sudo make install
- $ sudo mkdir /usr/local/share/tetrisgl
- $ sudo cp -r --parents src/data/* /usr/local/share/tetrisgl
