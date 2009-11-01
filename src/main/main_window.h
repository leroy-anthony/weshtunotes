/*
  Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include <KXmlGuiWindow>
#include <QtGui>
#include <QList>
#include <KToolBar>
#include <ksystemtrayicon.h>

#include "ui_main_window.h"

#include "../scene/ToolBarScene.h"
#include "../scene/AbstractScene.h"
#include "../scene/FreeScene.h"
#include "../scene/LayoutScene.h"
#include "../item/NoteItem.h"
#include "../explorer/TreeExplorer.h"
#include "../tag/TagFactory.h"
#include "../scene/CustomGraphicsView.h"

using namespace std;

class MainWindow : public KXmlGuiWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    Scene::AbstractScene * currentScene();
    Scene::CustomGraphicsView * currentView();

public slots:
    void loadScene( QTreeWidgetItem * item , int column = 0 );
    void showTagFactory();
    void save();
    
    void iconActivated( QSystemTrayIcon::ActivationReason reason );

    void layoutScene();
    void freeScene();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initView();
    void initToolBar();
    void initMedia();
    void initSystemTray();
    void setupActions();
    void loadData();

    Scene::AbstractScene * m_currentScene;
    Scene::ToolBarScene * m_controlerScene;

    Scene::CustomGraphicsView * m_view;

    Tag::TagFactory * m_tagFactory;

    Explorer::TreeExplorer * m_treeExplorer;
    Basket::ItemTreeBasket * m_lastBasketLoad;

    KSystemTrayIcon * m_trayIcon;

};

