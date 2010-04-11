# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER i486-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i486-mingw32-g++)

set(KDE_PREFIX    "/home/kahal/.wine/drive_c/Program\ Files/KDE")
set(QT_INSTALL_DIR ${KDE_PREFIX})
set(KDE4_INSTALL_DIR ${KDE_PREFIX})

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH /usr/i486-mingw32/ /usr/i486-mingw32/lib/ ${KDE_PREFIX} )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(QT_BINARY_DIR   ${KDE_PREFIX}/bin)
SET(QT_MOC_EXECUTABLE "/usr/bin/moc" )
SET(QT_UIC_EXECUTABLE "/usr/bin/uic" )
SET(QT_QMAKE_EXECUTABLE "/usr/bin/qmake" )
SET(QT_RCC_EXECUTABLE "/usr/bin/rcc" )

SET(QT_QT_INCLUDE_DIR ${KDE_PREFIX}/include/)
SET(QT_QTGUI_INCLUDE_DIR ${KDE_PREFIX}/include/QtGui)
SET(QT_QTOPENGL_INCLUDE_DIR ${KDE_PREFIX}/include/QtOpenGL)
SET(QT_QTSVG_INCLUDE_DIR ${KDE_PREFIX}/include/QtSvg)
SET(QT_QTSQL_INCLUDE_DIR ${KDE_PREFIX}/include/QtSql)

set(QT_LIBRARY_DIR  ${KDE_PREFIX}/lib)
set(QT_QTCORE_LIBRARY   ${KDE_PREFIX}/lib/libQtCore4.a)
set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
set(QT_QTCORE_INCLUDE_DIR ${KDE_PREFIX}/include/QtCore)
set(QT_MKSPECS_DIR  ${KDE_PREFIX}/mkspecs)
set(PHONON_INCLUDE_DIR ${KDE_PREFIX}/include/)
set(PHONON_LIBRARY   ${KDE_PREFIX}/lib/phonon.a)

set(QT_QTSVG_LIBRARY   ${KDE_PREFIX}/lib/libQtSvg4.a)

# this one is used by FindKDE4.cmake to load FindKDE4Internal.cmake:
set(KDE4_BIN_DIR    ${KDE_PREFIX}/bin)
set(KDE4_DATA_DIR    ${KDE_PREFIX}/share/apps CACHE PATH "points to the apps directory of installed kdelibs")

# disable some things:
set(WITH_AVAHI OFF   CACHE BOOL "Disabled")
set(WITH_DNSSD OFF   CACHE BOOL "Disabled")
set(WITH_ENCHANT OFF CACHE BOOL "Disabled")
set(WITH_FAM OFF     CACHE BOOL "Disabled")
set(WITH_GSSAPI OFF  CACHE BOOL "Disabled")
set(WITH_HSPELL OFF  CACHE BOOL "Disabled")
set(WITH_OpenEXR OFF CACHE BOOL "Disabled")

# use the binaries from native KDE4
set(KDE4_KDECONFIG_EXECUTABLE ${KDE4_BIN_DIR}/kde4-config CACHE PATH "")
set(KDE4_KCFGC_EXECUTABLE ${KDE4_BIN_DIR}/kconfig_compiler CACHE PATH "")
set(AUTOMOC4_EXECUTABLE /usr/bin/automoc4 CACHE PATH "")
set(KDE4_MEINPROC_EXECUTABLE ${KDE4_BIN_DIR}/meinproc4 CACHE PATH "")


