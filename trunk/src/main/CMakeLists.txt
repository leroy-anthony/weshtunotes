INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


set(weshnotes_UI  ui_main_window.ui ../tag/tag_factory.ui)
set(weshnotes_SRC main.cpp main_window.cpp ui_main_window.h ../tag/tag_factory.h )
set(weshnotes_MOC main_window.h )


set(weshnotes_SRC 
../widget/VerticalButton.cpp
../widget/qtcolorpicker.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../widget/VerticalButton.h 
../widget/qtcolorpicker.h
${weshnotes_MOC})

set(weshnotes_SRC 
../scene/AbstractScene.cpp 
../scene/FreeScene.cpp
../scene/ToolBarScene.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../scene/AbstractScene.h 
../scene/FreeScene.h
../scene/ToolBarScene.h
${weshnotes_MOC})


set(weshnotes_SRC 
../item/AbstractItem.cpp 
../item/NoteItem.cpp
../item/CustomTextEdit.cpp
../handle/HandleItem.cpp
../handle/SizeHorHandle.cpp
../handle/MoveHandle.cpp
../handle/DeleteHandle.cpp
../handle/GraphicHandleItem.cpp
${weshnotes_SRC})


set(weshnotes_SRC 
../interface/IVisualItemEdition.cpp
../interface/ITextEdition.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../item/AbstractItem.h 
../item/NoteItem.h
../item/CustomTextEdit.h
../handle/HandleItem.h
../handle/SizeHorHandle.h
../handle/MoveHandle.h
../handle/DeleteHandle.h
../handle/GraphicHandleItem.h
${weshnotes_MOC})


set(weshnotes_SRC 
../explorer/AbstractExplorer.cpp
../explorer/TreeExplorer.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../explorer/TreeExplorer.h
${weshnotes_MOC})


set(weshnotes_SRC 
../basket/ItemTreeBasket.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../basket/ItemTreeBasket.h 
${weshnotes_MOC})

set(weshnotes_SRC 
../config/Configuration.cpp
../config/ImageFactory.cpp
${weshnotes_SRC})


set(weshnotes_MOC 
../tag/ChooseTag.h 
../tag/NoteTag.h
../tag/TagFactory.h
${weshnotes_MOC})

set(weshnotes_SRC 
../tag/AbstractTag.cpp
../tag/ChooseTag.cpp
../tag/NoteTag.cpp
../tag/State.cpp
../tag/TagFactory.cpp
${weshnotes_SRC})


QT4_ADD_RESOURCES(weshnotes main.qrc )

QT_WRAP_CPP(weshnotes weshnotes_SRC ${weshnotes_MOC})
QT_WRAP_UI(weshnotes weshnotes_UI_H weshnotes_UI_SRC ${weshnotes_UI})


add_executable(weshnotes WIN32 MACOSX_BUNDLE ${weshnotes_SRC})
target_link_libraries(weshnotes ${QT_LIBRARIES})


install(TARGETS weshnotes DESTINATION ${weshnotes_DESTINATION})
