PROJECT(weshTuNotes)

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE ON)

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

FIND_PACKAGE(Qt4)

SET ( QT_USE_QTSVG TRUE )

INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS( ${QT_DEFINITIONS} )

set(CMAKE_CXX_FLAGS "-g -Wall")

add_subdirectory(src)

install(FILES data/icon/add.svg
 data/icon/application-exit.png
 data/icon/basket.png
 data/icon/cr16-action-tag_checkbox.png
 data/icon/document-close.png
 data/icon/document-new.png
 data/icon/edit-delete.png
 data/icon/emblem-favorite.png
 data/icon/emblem-important.png
 data/icon/format-justify-center.png
 data/icon/format-justify-fill.png
 data/icon/format-justify-left.png
 data/icon/format-justify-right.png
 data/icon/format-text-bold.png
 data/icon/format-text-italic.png
 data/icon/format-text-strikethrough.png
 data/icon/format-text-underline.png
 data/icon/im-status-message-edit.png
 data/icon/list-add.png
 data/icon/rect.svg
 data/icon/triangle.svg
 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/weshtunotes/data/icon)