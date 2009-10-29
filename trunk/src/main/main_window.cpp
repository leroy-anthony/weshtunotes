/*
 Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.
 
 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include "main_window.h"

#include <QSpacerItem>

#include <QCalendarWidget>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include "../explorer/TreeExplorer.h"
#include "../basket/ItemTreeBasket.h"
#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../config/ConfigDialog.h"

MainWindow::MainWindow(QWidget * parent, int argc, char *argv[]) :
        KXmlGuiWindow(parent),
        m_lastBasketLoad(0)
{
    setupUi(this);

    initView();

    initMedia();


    m_tagFactory = Tag::TagFactory::newTagFactory();
    m_tagFactory->loadTags();

    setupActions();
    initToolBar();
    initSystemTray();
    createGUI("data:main_ui.rc");

    loadData();
}

MainWindow::~MainWindow()
{
    save();

    delete m_tagFactory;
}

void MainWindow::loadData()
{
    m_treeExplorer->loadBaskets();
    m_treeExplorer->loadFromConfigCurrentBasket();

    QList<QTreeWidgetItem*> items = m_treeExplorer->selectedItems();
    if ( items.size() > 0 )
    {
        loadScene(items[0]);
    }

    if ( m_lastBasketLoad->scene()->type() == Scene::FreeScene::type )
    {
        actionCollection()->action( Scene::FreeScene::type )->setChecked(true);
        actionCollection()->action( Scene::FreeScene::type )->setDisabled(true);
        actionCollection()->action( Scene::LayoutScene::type )->setDisabled(false);
        actionCollection()->action( Scene::LayoutScene::type )->setChecked(false);
    }
    else
    {
        actionCollection()->action( Scene::LayoutScene::type )->setChecked(true);
        actionCollection()->action( Scene::FreeScene::type )->setChecked(false);
        actionCollection()->action( Scene::LayoutScene::type )->setDisabled(true);
        actionCollection()->action( Scene::FreeScene::type )->setDisabled(false);
    }
}

void MainWindow::setupActions()
{
    KStandardAction::save(this, SLOT(save()), actionCollection());
    KStandardAction::quit(qApp, SLOT(quit()), actionCollection());
    KStandardAction::paste(m_view, SLOT(paste()), actionCollection());
}

void MainWindow::save()
{
    if ( m_lastBasketLoad != 0 )
    {
        m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value()-53); // valeur magic !!
        m_view->verticalScrollBar()->setValue(m_view->verticalScrollBar()->value()+159); // valeur magic !!
        m_lastBasketLoad->scene()->storeView( m_view );
    }

    m_treeExplorer->saveBaskets();
    if ( m_lastBasketLoad != 0 )
    {
        Config::Configuration::saveLastBasket( m_lastBasketLoad->basketId() );
    }
}

void MainWindow::initToolBar()
{
    m_controlerScene = new Scene::ToolBarScene( this );
}

void MainWindow::initView()
{
    m_view = new Scene::CustomGraphicsView();
    centralwidget->layout()->addWidget( m_view );
    m_view->show();
}

void MainWindow::initMedia()
{
    m_treeExplorer = new Explorer::TreeExplorer(this);

    // creation du layout du dock
    QLayout * layoutDock = new QVBoxLayout( dockWidgetContents );
    layoutDock->setMargin(4);
    layoutDock->setSpacing(0);

    QWidget * widget = new QWidget();
    dockFichier->setTitleBarWidget(widget);

    QLayout * layoutButtonExplorer = new QHBoxLayout( widget );
    layoutButtonExplorer->setMargin(4);
    layoutButtonExplorer->setSpacing(2);

    QToolButton * qq = new QToolButton();
    qq->setIcon(Config::ImageFactory::icon(Config::Image::newBasket));
    connect( qq, SIGNAL(clicked()), m_treeExplorer, SLOT(addBasketToRoot()) );
    layoutButtonExplorer->addWidget(qq);

    QToolButton * q = new QToolButton();
    q->setIcon(Config::ImageFactory::icon(Config::Image::addBasket));
    connect( q, SIGNAL(clicked()), m_treeExplorer, SLOT(addToCurrentBasket()) );
    layoutButtonExplorer->addWidget(q);

    QToolButton * qqq = new QToolButton();
    qqq->setIcon(Config::ImageFactory::icon(Config::Image::deleteAction));
    connect( qqq, SIGNAL(clicked()), m_treeExplorer, SLOT(delCurrentBasket()) );
    layoutButtonExplorer->addWidget(qqq);

    layoutButtonExplorer->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    layoutDock->addWidget( m_treeExplorer );

    connect(m_treeExplorer,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(loadScene(QTreeWidgetItem*,int)));

}

void MainWindow::loadScene( QTreeWidgetItem * item , int column )
{
    Basket::ItemTreeBasket * i = dynamic_cast<Basket::ItemTreeBasket*>(item);

    if ( m_lastBasketLoad != 0 )
    {
        m_lastBasketLoad->scene()->storeView( m_view );
    }

    m_lastBasketLoad = i;
    Scene::AbstractScene * scene = i->scene();
    m_view->setScene( scene );
    i->scene()->restoreView( m_view );

    if ( m_lastBasketLoad->scene()->type() == Scene::FreeScene::type )
    {
        actionCollection()->action( Scene::FreeScene::type )->setChecked(true);
        actionCollection()->action( Scene::FreeScene::type )->setDisabled(true);
        actionCollection()->action( Scene::LayoutScene::type )->setDisabled(false);
        actionCollection()->action( Scene::LayoutScene::type )->setChecked(false);
    }
    else
    {
        actionCollection()->action( Scene::LayoutScene::type )->setChecked(true);
        actionCollection()->action( Scene::FreeScene::type )->setChecked(false);
        actionCollection()->action( Scene::LayoutScene::type )->setDisabled(true);
        actionCollection()->action( Scene::FreeScene::type )->setDisabled(false);
    }

}

Scene::AbstractScene * MainWindow::currentScene()
{
    return dynamic_cast<Scene::AbstractScene*>(m_view->scene());
}

Scene::CustomGraphicsView * MainWindow::currentView()
{
    return m_view;
}

void MainWindow::showTagFactory()
{
    m_tagFactory->show();
}

void MainWindow::initSystemTray()
{
    m_trayIcon = new KSystemTrayIcon(this);
    m_trayIcon->setIcon(Config::ImageFactory::icon(Config::Image::application));
    m_trayIcon->show();
    setWindowIcon(Config::ImageFactory::icon(Config::Image::application));
}

void MainWindow::layoutScene()
{
    m_lastBasketLoad->scene()->setType( Scene::LayoutScene::type );
    m_lastBasketLoad->save();
    m_lastBasketLoad->load();

    m_view->setScene( m_lastBasketLoad->scene() );

    actionCollection()->action( Scene::LayoutScene::type )->setDisabled(true);
    actionCollection()->action( Scene::LayoutScene::type )->setChecked(true);

    actionCollection()->action( Scene::FreeScene::type )->setChecked(false);
    actionCollection()->action( Scene::FreeScene::type )->setDisabled(false);
}

void MainWindow::freeScene()
{
    m_lastBasketLoad->scene()->setType( Scene::FreeScene::type );
    m_lastBasketLoad->save();
    m_lastBasketLoad->load();

    m_view->setScene( m_lastBasketLoad->scene() );
    m_view->fitInViewZoom();
    m_view->resetZoom();

    actionCollection()->action( Scene::FreeScene::type )->setDisabled(true);
    actionCollection()->action( Scene::FreeScene::type )->setChecked(true);

    actionCollection()->action( Scene::LayoutScene::type )->setChecked(false);
    actionCollection()->action( Scene::LayoutScene::type )->setDisabled(false);
}
