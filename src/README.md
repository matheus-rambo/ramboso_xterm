# Table of Contents
- [createdir](#createdir)    
- [currentdir](#currentdir)    
- [showdir](#showdir)

## Createdir
Creates a new Directory.    
It uses the [mkdir syscall](https://man7.org/linux/man-pages/man2/mkdir.2.html).  

[Source](https://github.com/matheus-rambo/ramboso_xterm/blob/main/src/createdir.c)

## Currentdir
Gets the user current directory.    
It uses the [getcwd syscall](https://man7.org/linux/man-pages/man2/getcwd.2.html).

[Source](https://github.com/matheus-rambo/ramboso_xterm/blob/main/src/currentdir.c)

## Showdir
Shows the directory entries.   
Files, Directories.     
It uses the the [opendir syscall](https://man7.org/linux/man-pages/man3/opendir.3.html) to open a directory stream, then, uses the [readdir syscall](https://man7.org/linux/man-pages/man3/readdir.3.html) to read the directory entries, then, after all, uses the [closedir syscall](https://man7.org/linux/man-pages/man3/closedir.3.html) to close the directory stream.

[Source](https://github.com/matheus-rambo/ramboso_xterm/blob/main/src/showdir.c)
