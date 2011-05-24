/*
    Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QSystemTrayIcon>

#include <KXmlGuiWindow>

#include "ui_main_window.h"
#include "../handle/GraphicHandleItem.h"

class QTreeWidgetItem;
class QNetworkReply;

class KSystemTrayIcon;

namespace Basket
{
    class ItemTreeBasket;
}

namespace Handle
{
    class GraphicHandleItem;
}

namespace Scene
{
    class CustomGraphicsView;
    class ToolBarScene;
    class AbstractScene;
}

namespace Explorer
{
    class TreeExplorer;
}

namespace Tag
{
    class TagFactory;
}



class MainWindow : public KXmlGuiWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget * parent = 0 );
    ~MainWindow();

    Scene::AbstractScene * currentScene();
    Scene::CustomGraphicsView * currentView();

    static void reloadView();
    void loadData();

public slots:
    void loadScene( QTreeWidgetItem * item , int column = 0 );
    void save();
    
    void delCurrentBasket();
    void addBasket( Basket::ItemTreeBasket * item );

    void showSettings();

    static void showMessage( const QString & message, int time );
    static KStatusBar * statusBar();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void loadScene();

private:
    void initView();
    void initStatusBar();
    void initToolBar();
    void initExplorer();
    void initSystemTray();
    void setupActions();


    Scene::AbstractScene * m_currentScene;
    Scene::ToolBarScene * m_controllerScene;
    static KStatusBar * m_statusBar;

    static Scene::CustomGraphicsView * m_view;

    Tag::TagFactory * m_tagFactory;

    Explorer::TreeExplorer * m_treeExplorer;
    static Basket::ItemTreeBasket * m_lastBasketLoad;

    KSystemTrayIcon * m_trayIcon;

    Handle::GraphicHandleItem * m_copyHandleItem;

};

