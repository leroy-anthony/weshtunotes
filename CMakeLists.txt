PROJECT(kweshtunotes)

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE ON)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

find_package(Nepomuk REQUIRED)
include_directories(${NEPOMUK_INCLUDES})

include_directories(${Soprano_INCLUDE_DIRS})


SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTOPENGL TRUE)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

set(weshnotes_UI  
./src/main/ui_main_window.ui 
./src/tag/tag_factory.ui
./src/config/GeneralSettings.ui
./src/config/AppareanceSettings.ui)

QT_WRAP_UI(weshnotes weshnotes_UI_H weshnotes_UI_SRC ${weshnotes_UI})

set(weshnotes_SRC 
./src/main/main.cpp 
./src/main/MainWindow.cpp 
./src/main/KWeshTuNotesApp.cpp
./src/widget/VerticalButton.cpp
./src/widget/qtcolorpicker.cpp
./src/widget/ColorCombo.cpp
./src/scene/AbstractScene.cpp 
./src/scene/FreeScene.cpp
./src/scene/LayoutScene.cpp
./src/scene/SceneFactory.cpp
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
./src/basket/AbstractBasket.cpp
./src/basket/Basket.cpp
./src/basket/TagBasket.cpp
./src/basket/BasketFactory.cpp
./src/basket/NewBasketDialog.cpp
./src/config/Configuration.cpp
./src/config/ImageFactory.cpp
./src/config/ConfigDialog.cpp
./src/config/VisualAspect.cpp
./src/tag/AbstractTag.cpp
./src/tag/ChooseTag.cpp
./src/tag/NoteTag.cpp
./src/tag/State.cpp
./src/tag/TagFactory.cpp
./src/tag/AddTag.cpp
./src/tag/NextTag.cpp
./src/technic/GeneratorID.cpp
./src/config/GeneralPageDialog.cpp
./src/config/AppareancePageDialog.cpp
${weshnotes_UI_H})

set(weshnotes_MOC 
./src/main/main_window.h
./src/widget/VerticalButton.h 
./src/widget/qtcolorpicker.h 
./src/scene/AbstractScene.h 
./src/scene/FreeScene.h
./src/scene/LayoutScene.h
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
./src/config/ConfigDialog.h
./src/explorer/TreeExplorer.h
./src/basket/ItemTreeBasket.h 
./src/tag/ChooseTag.h 
./src/tag/NoteTag.h
./src/tag/TagFactory.h
./src/tag/AddTag.h)

kde4_add_kcfg_files(weshnotes_SRC ./src/config/settings.kcfgc)
kde4_add_executable(kweshtunotes ${weshnotes_SRC})
target_link_libraries(kweshtunotes ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${QT_LIBRARIES} ${Soprano_LIBRARIES} ${NEPOMUK_QUERY_LIBRARIES} ${NEPOMUK_LIBRARIES} )

install(TARGETS kweshtunotes DESTINATION ${BIN_INSTALL_DIR})
install(FILES ./src/main/kweshtunotesui.rc DESTINATION  ${DATA_INSTALL_DIR}/kweshtunotes)
install(FILES kweshtunotes.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})
