installation :
    - make sure Qt4 is installed (on windows, choose the installation with mingw)
        >  make also sure that the qmake command points to Qt4's one (in case several versions of Qt are installed)
    - run qprog/setup to configure Qt's environment variables
    - for windows : copy the files from dos_console to the root directory and edit qt_console.bat to adjust Qt's install path
        > each time you need to compile or run a qprog project, you should start from this shell
        > also all dlls of used libs (e.g. qlgviewer) should be placed in qprog/bin directory
    - create a 'build' directory (or symbolic link) at the path given in qprog/config/paths.pri
        (this path can be reconfigured as explained in qprog/README.txt)
    - compile the libraries: cd to each subfolder of qprog/libs and run qmake; make
        (only for folders containing a .pro file)
        > for qglviewer cd to qprog/libs/qglviewer/libQGLViewer***/QGLViewer and run qmake;make
            > on windows, move the created ./release/QGLViewer2.dll to qprog/bin

