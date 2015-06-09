* static lib / recompiled lib :
    Local libs are linked as static libs by default. Adding a lib's name to the SOURCE_LIBS variable will force recompilation of its sources with the current project, instead of using the precompiled binary lib file.

* debug and release modes :
    By default, local (static) libs are built in release mode. It does not prevent them to be linked with code compliled in debug mode.
    To use a local lib in debug mode, it should be recompiled with the current project (by adding it to the SOURCE_LIBS variable). If the current project is in debug mode, the recompiled libs will be too.

* installation dependent parameters modification :
    1) paths : for a given install of QPROG, some paths can be customised. They are contained in the config/paths.pri file. To override these paths, copy <config/paths.pri> into <config/paths_local.pri> and modify the content as desired (do not modify paths.pri, as it is on the SVN).
    2) libs configuration : on specific platforms, local libs possibly need to be configured differently. Local libs are configured in QT features files (.prf), located in the <qtfeatures> directory. To override some of these feature files, create a custom directory containing the desired modified feature files and append this directory's path to the QMAKEFEATURES variable of QT's environment (qmake -set QMAKEFEATURES <custom directory>:<default directory>)
    
