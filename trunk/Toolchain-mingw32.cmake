# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER i486-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i486-mingw32-g++)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH /usr/i486-mingw32/ /usr/i486-mingw32/lib/ /usr/local/qt4-win32/ /usr/local/qt4-win32/lib /usr/local/qt4-win32/include )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET ( QT_MOC_EXECUTABLE "/usr/bin/moc" )
SET ( QT_UIC_EXECUTABLE "/usr/bin/uic" )

set(QT_BINARY_DIR   /usr/local/qt4-win32/bin)
set(QT_LIBRARY_DIR  /usr/i486-mingw32/lib)
set(QT_LIBRARY_DIR  /usr/local/qt4-win32/lib)
set(QT_QTCORE_LIBRARY   /usr/local/qt4-win32/lib/libQtCore4.a)
set(QT_QTGUI_LIBRARY   /usr/local/qt4-win32/lib/libQtGui4.a)
set(QT_QTSVG_LIBRARY   /usr/local/qt4-win32/lib/libQtSvg4.a)
set(QT_QTOPENGL_LIBRARY /usr/local/qt4-win32/lib/libQtOpenGL4.a)
set(QT_QTNETWORK_LIBRARY /usr/local/qt4-win32/include/libQtNetwork.a)
set(QT_QTCORE_INCLUDE_DIR /usr/local/qt4-win32/include/QtCore)
set(QT_QTSVG_INCLUDE_DIR /usr/local/qt4-win32/include/QtSvg)
set(QT_QTGUI_INCLUDE_DIR /usr/local/qt4-win32/include/QtGui)
set(QT_QTOPENGL_INCLUDE_DIR /usr/local/qt4-win32/include/QtOpenGL)
set(QT_QTNETWORK_INCLUDE_DIR /usr/local/qt4-win32/include/QtNetwork)
set(QT_MKSPECS_DIR  /usr/local/qt4-win32/mkspecs)