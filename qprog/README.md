# Bablib et Libs (externes)

* static lib / recompiled lib :
    Local libs are linked as static libs by default. Adding a lib's name to the SOURCE_LIBS variable will force recompilation of its sources with the current project, instead of using the precompiled binary lib file.

* debug and release modes :
    By default, local (static) libs are built in release mode. It does not prevent them to be linked with code compliled in debug mode.
    To use a local lib in debug mode, it should be recompiled with the current project (by adding it to the SOURCE_LIBS variable). If the current project is in debug mode, the recompiled libs will be too.

* installation dependent parameters modification :
    1. paths : for a given install of QPROG, some paths can be customised. They are contained in the config/paths.pri file. To override these paths, copy <config/paths.pri> into <config/paths_local.pri> and modify the content as desired (do not modify paths.pri, as it is on the SVN).
    2. libs configuration : on specific platforms, local libs possibly need to be configured differently. Local libs are configured in QT features files (.prf), located in the <qtfeatures> directory. To override some of these feature files, create a custom directory containing the desired modified feature files and append this directory's path to the QMAKEFEATURES variable of QT's environment (qmake -set QMAKEFEATURES <custom directory>:<default directory>)


* il faut lancer le script setup.linux via:
```bash
$ source setup.linux
```
pour setter la variable d'env. *LD_LIBRARY_PATH* qui permet d'avoir un accÃ¨s au build dynamique de *QGLViewer*

# [QGLViewer](http://libqglviewer.com/)
[![logo](http://libqglviewer.com/images/qglviewer.png)](http://libqglviewer.com/)

[GitHub](https://github.com/GillesDebunne/libQGLViewer) de la lib  
Utilisation de la fonctionnalitÃ© submodule de GIT pour avoir une notion de dÃ©pendance de la BabLib Ã  QGLViewer
* [6.6 Utilitaires Git - Sous-modules](https://git-scm.com/book/fr/v1/Utilitaires-Git-Sous-modules)
* [How to remove a directory in my GitHub repository?](http://stackoverflow.com/questions/6313126/how-to-remove-a-directory-in-my-github-repository)

J'ai copiÃ© les resultats de build dans le rÃ©pertoire qprob/bin
```bash
/qprog$ ls -a bin/ | grep libQGL
libQGLViewer.so
libQGLViewer.so.2
libQGLViewer.so.2.6
libQGLViewer.so.2.6.3
```
*ldd*, *objdump*, *readef*:
* [How to check what libraries are used by a program or process on Linux](http://ask.xmodulo.com/check-library-dependency-program-process-linux.html)
* [How to list library dependencies of a non-native binary?](http://stackoverflow.com/questions/10052041/how-to-list-library-dependencies-of-a-non-native-binary)
* [How do I find the direct shared object dependencies of a Linux (ELF) binary?](http://stackoverflow.com/questions/6242761/how-do-i-find-the-direct-shared-object-dependencies-of-a-linux-elf-binary)


# QGLViewer 2 le retour :p

Du coup, j'ai laissÃ© tomber les submodule pour passer en subtree et j'ai rÃ©cupÃ©rÃ© un dÃ©pot [libQGLViewer](https://github.com/yoyonel/libqglviewer) dans mon github (qui doit Ãªtre un fork de la lib de Gilles sur GITHUB)
