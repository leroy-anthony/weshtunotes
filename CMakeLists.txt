PROJECT(kweshtunotes)

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE ON)

#### VERSION ####

EXECUTE_PROCESS(
        COMMAND cat version.txt
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_VARIABLE VERSION_INFO
        ERROR_VARIABLE VERSION_INFO
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

CONFIGURE_FILE ( "${PROJECT_SOURCE_DIR}/src/main/version.h.in" "${PROJECT_BINARY_DIR}/version.h"  )

INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")

#### VERSION ####

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra")

SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTSQL TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

find_package(Nepomuk REQUIRED)
find_package(Soprano REQUIRED)

include_directories(${NEPOMUK_INCLUDES})
include_directories(${SOPRANO_INCLUDE_DIRS})

set(weshnotes_UI  
./src/main/ui_main_window.ui 
./src/tag/tag_factory.ui
./src/config/GeneralSettings.ui
./src/config/AnimationSettings.ui
./src/config/AppareanceSettings.ui
./src/config/SynchroSettings.ui
./src/config/ClipperSettings.ui
./src/basket/NewBasketForm.ui
./src/scene/NewAppletForm.ui)

QT_WRAP_UI(weshnotes weshnotes_UI_H weshnotes_UI_SRC ${weshnotes_UI})

set(weshnotes_SRC 
./src/main/main.cpp 
./src/main/MainWindow.cpp 
./src/main/KWeshTuNotesApp.cpp
./src/scene/AbstractScene.cpp 
./src/scene/FreeScene.cpp
./src/scene/LayoutScene.cpp
./src/scene/SceneFactory.cpp
./src/scene/ToolBarScene.cpp
./src/scene/CustomGraphicsView.cpp
./src/scene/PlasmaScene.cpp
./src/scene/NewAppletDialog.cpp
./src/item/ItemFactory.cpp
./src/item/AbstractItem.cpp 
./src/item/NoteItem.cpp
./src/item/CustomTextEdit.cpp
./src/handle/HandleItem.cpp
./src/handle/SizeHorHandle.cpp
./src/handle/MoveHandle.cpp
./src/handle/DeleteHandle.cpp
./src/handle/PinHandle.cpp
./src/handle/GraphicHandleItem.cpp
./src/interface/IVisualItemEdition.cpp
./src/interface/ITextEdition.cpp
./src/explorer/AbstractExplorer.cpp
./src/explorer/TreeExplorer.cpp
./src/explorer/ItemTreeBasket.cpp
./src/basket/AbstractBasket.cpp
./src/basket/Basket.cpp
./src/basket/TagBasket.cpp
./src/basket/PlasmaBasket.cpp
./src/basket/ClipperBasket.cpp
./src/basket/BasketFactory.cpp
./src/basket/NewBasketDialog.cpp
./src/basket/PropertiesBasketDialog.cpp
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
./src/data/AssociationManager.cpp
./src/data/ExportDataToXml.cpp
./src/data/ImportDataFromXml.cpp
./src/data/DataManager.cpp
./src/synchro/DigitalBucketConnection.cpp
./src/synchro/GoogleDocsConnection.cpp
./src/synchro/AbstractConnection.cpp
./src/synchro/SynchroManager.cpp
./src/synchro/NetworkAccessManagerProxy.cpp
./src/synchro/ConnectionFactory.cpp
./src/config/GeneralPageDialog.cpp
./src/config/AppareancePageDialog.cpp
./src/config/AnimationPageDialog.cpp
./src/config/SynchroPageDialog.cpp
./src/config/ClipperPageDialog.cpp
./src/animation/AnimationManager.cpp
${weshnotes_UI_H})

set(weshnotes_MOC 
./src/main/main_window.h
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
./src/synchro/SynchroManager.h
./src/synchro/AbstractBasket.h
./src/tag/ChooseTag.h 
./src/tag/NoteTag.h
./src/tag/TagFactory.h
./src/tag/AddTag.h)

kde4_add_kcfg_files(weshnotes_SRC ./src/config/settings.kcfgc)
kde4_add_executable(kweshtunotes ${weshnotes_SRC})
target_link_libraries(kweshtunotes ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KPARTS_LIBS} ${QT_LIBRARIES} ${SOPRANO_LIBRARIES} ${NEPOMUK_QUERY_LIBRARIES} ${NEPOMUK_LIBRARIES} )

install(TARGETS kweshtunotes DESTINATION ${BIN_INSTALL_DIR})
install(FILES ./src/main/kweshtunotesui.rc DESTINATION  ${DATA_INSTALL_DIR}/kweshtunotes)
install(FILES ./data/tags DESTINATION  ${DATA_INSTALL_DIR}/kweshtunotes)
install(FILES kweshtunotes.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})
