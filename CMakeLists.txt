PROJECT(weshTuNotes)

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -W -Wall ")

FIND_PACKAGE(Qt4)

SET ( QT_USE_QTSVG TRUE )
SET ( QT_USE_QTOPENGL TRUE )
SET ( QT_USE_QTNETWORK TRUE )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_ADD_RESOURCES(weshnotes ./src/main/main.qrc )

set(weshnotes_UI  
./src/main/ui_main_window.ui 
./src/tag/tag_factory.ui)

QT_WRAP_UI(weshnotes weshnotes_UI_H weshnotes_UI_SRC ${weshnotes_UI})

set(weshnotes_SRC 
./src/main/main.cpp 
./src/main/main_window.cpp 
./src/widget/VerticalButton.cpp
./src/widget/qtcolorpicker.cpp
./src/widget/ColorCombo.cpp
./src/scene/AbstractScene.cpp 
./src/scene/FreeScene.cpp
./src/scene/ToolBarScene.cpp
./src/scene/CustomGraphicsView.cpp
./src/item/AbstractItem.cpp 
./src/item/NoteItem.cpp
./src/item/CustomTextEdit.cpp
./src/handle/HandleItem.cpp
./src/handle/SizeHorHandle.cpp
./src/handle/MoveHandle.cpp
./src/handle/DeleteHandle.cpp
./src/handle/GraphicHandleItem.cpp
./src/interface/IVisualItemEdition.cpp
./src/interface/ITextEdition.cpp
./src/explorer/AbstractExplorer.cpp
./src/explorer/TreeExplorer.cpp
./src/basket/ItemTreeBasket.cpp
./src/config/Configuration.cpp
./src/config/ImageFactory.cpp
./src/tag/AbstractTag.cpp
./src/tag/ChooseTag.cpp
./src/tag/NoteTag.cpp
./src/tag/State.cpp
./src/tag/TagFactory.cpp
${weshnotes_UI_H})

set(weshnotes_MOC 
./src/main/main_window.h
./src/widget/VerticalButton.h 
./src/widget/qtcolorpicker.h 
./src/scene/AbstractScene.h 
./src/scene/FreeScene.h
./src/scene/ToolBarScene.h
./src/scene/CustomGraphicsView.h
./src/item/AbstractItem.h 
./src/item/NoteItem.h
./src/item/CustomTextEdit.h
./src/handle/HandleItem.h
./src/handle/SizeHorHandle.h
./src/handle/MoveHandle.h
./src/handle/DeleteHandle.h
./src/handle/GraphicHandleItem.h
./src/explorer/TreeExplorer.h
./src/basket/ItemTreeBasket.h 
./src/tag/ChooseTag.h 
./src/tag/NoteTag.h
./src/tag/TagFactory.h)

QT_WRAP_CPP(weshnotes weshnotes_SRC ${weshnotes_MOC})

add_executable(weshTuNotes WIN32 MACOSX_BUNDLE ${weshnotes_SRC})
target_link_libraries(weshTuNotes ${QT_LIBRARIES})


install(TARGETS weshTuNotes DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

FILE(GLOB_RECURSE icon data/icon/*)

install(FILES ${icon} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/weshtunotes/data/icon)
