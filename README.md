# lp
A simple bash script to interface with an ESC/POS serial thermal receipt printer.
Supports printing text, contents of files, cutting, printing newlines, and printing pngs.
The script converts them to a compatible bitmap automatically, just give it a png. Note that the images can't be more than 250ish pixels wide or it doesn't work (either won't print anything or will print the raw bytes as ASCII instead of an image).

Edit `port` to change what serial port the script references.
Edit the filepath for the `bitmap` binary on line 56 to correspond to your compiled `bitmap` binary from the provided C++. The command I used is:

```g++ -g -Wall -W -D_REENTRANT -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4 -I. -I. -o bitmap bitmap.cpp $(pkg-config --cflags --libs QtCore QtGui)```

Usage:
```
--print: prints text provided
--file: prints content of file passed (--file gaster.txt prints contents of gaster.txt
--cut: tells printer to cut
--newline: prints one newline
--newlines: prints N newlines (--newlines 2 prints 2 newlines)
--newcut: prints 2 newlines and then cuts (on my printer it cuts off what was just printed otherwise)
--image: prints any png provided (max 250px width)
--help: prints help
```
It can also be used as a pipe destination - running `neofetch --stdout | lp` will redirect the output to `port`.

I use it on Cygwin.
