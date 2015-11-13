# Bablib

## Installation
- make sure Qt4 is installed (on windows, choose the installation with mingw)
    make also sure that the qmake command points to Qt4's one (in case several versions of Qt are installed)

- run qprog/setup to configure Qt's environment variables

### For Windows
- copy the files from dos_console to the root directory and edit qt_console.bat to adjust Qt's install path each time you need to compile or run a qprog project, you should start from this shell also all dlls of used libs (e.g. qlgviewer) should be placed in qprog/bin directory

- create a 'build' directory (or symbolic link) at the path given in qprog/config/paths.pri  
(this path can be reconfigured as explained in qprog/README.txt)

- compile the libraries: cd to each subfolder of qprog/libs and run qmake; make  
(only for folders containing a .pro file)  

#### **QGLViewer**
- cd to qprog/libs/qglviewer/libQGLViewer***/QGLViewer
- run qmake;make

#### on Windows
move the created ./release/QGLViewer2.dll to qprog/bin


# Par rapport au *submodule* de GIT

* [Git submodules: Specify a branch/tag](http://stackoverflow.com/questions/1777854/git-submodules-specify-a-branch-tag)
* [6.6 Utilitaires Git - Sous-modules](https://git-scm.com/book/fr/v1/Utilitaires-Git-Sous-modules)
* [Adding git submodule that contains another submodule?](http://stackoverflow.com/questions/4600835/adding-git-submodule-that-contains-another-submodule)

# Par rapport au *subtree* de GIT
* [Comprendre et maÃ®triser les subtrees Git](http://www.git-attitude.fr/2015/01/30/git-subtrees/)

### Sous Mint (Portable LENOVO)

#### Packages installÃ©s
```bash
$ history | grep apt | grep install
  35  sudo apt-get install build-essential
  36  sudo apt-get install cmake cmake-qt-gui cmake-curses-gui
 426  sudo apt-get install qt4-qmake
 456  sudo apt-get install qt4-dev-tools
 457  sudo apt-get install qt4-default
 472  sudo apt-get install mesa-common-dev
 473  sudo apt-get install freeglut3-dev
 636  sudo apt-get install libpng12-dev
 763  sudo apt-get install libqt4-dev
 764  sudo apt-get install libqt4-opengl-dev
 850  sudo apt-get install libgsl0-dev
```
