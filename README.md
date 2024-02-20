# lp
A simple bash script to interface with an ESC/POS serial thermal receipt printer.
Supports printing text, contents of files, cutting, printing newlines, and printing bitmaps.

Edit `port` to change what serial port the script references.
Edit the filepath for the `bitmap` binary on line 56 to correspond to your compiled `bitmap` binary from the provided C++. The command I used is:

```g++ -g -Wall -W -D_REENTRANT -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4 -I. -I. -o bitmap bitmap.cpp $(pkg-config --cflags --libs QtCore QtGui)```

I use it on Cygwin.
