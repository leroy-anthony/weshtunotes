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

#include "../explorer/TreeExplorer.h"
#include "../basket/ItemTreeBasket.h"
#include "../config/Configuration.h"
#include "../config/ImageFactory.h"

MainWindow::MainWindow(QWidget * parent, int argc, char *argv[]) :
        QMainWindow(parent)
{
    setupUi(this);

    initView();
    initToolBar();
    initMedia();
    initSystemTray();

    m_treeExplorer->loadBaskets();

    QList<QTreeWidgetItem*> items = m_treeExplorer->selectedItems();
    if ( items.size() > 0 )
    {
        loadScene(items[0]);
    }

    m_tagFactory = Tag::TagFactory::newTagFactory();
    m_tagFactory->loadTags();

    connect(actionCustomize,SIGNAL(activated()), this, SLOT(showTagFactory()));
}

MainWindow::~MainWindow()
{
    m_treeExplorer->saveBaskets();
}

void MainWindow::initToolBar()
{
    m_controlerScene = new Scene::ToolBarScene( this );
}

void MainWindow::initView()
{
    m_view = new QGraphicsView();

    centralwidget->layout()->addWidget( m_view );

    m_view->show();
}


void MainWindow::initMedia()
{
    dockFichier->setWindowTitle(tr("Paniers"));

    m_treeExplorer = new Explorer::TreeExplorer(this);

    // creation du layout du dock
    QLayout * layoutDock = new QVBoxLayout( dockWidgetContents );
    layoutDock->setMargin(0);
    layoutDock->setSpacing(0);

    QWidget * widget = new QWidget();
    QLayout * layoutButtonExplorer = new QHBoxLayout( widget );
    layoutButtonExplorer->setMargin(0);
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

    layoutDock->addWidget( widget );



    layoutDock->addWidget( m_treeExplorer );

    m_treeExplorer->setHeaderHidden( true );

    connect(m_treeExplorer,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(loadScene(QTreeWidgetItem*,int)));

}

void MainWindow::loadScene( QTreeWidgetItem * item , int column )
{
    Basket::ItemTreeBasket * i = dynamic_cast<Basket::ItemTreeBasket*>(item);
    m_view->setScene(  i->scene() );
}

Scene::AbstractScene * MainWindow::currentScene()
{
    return dynamic_cast<Scene::AbstractScene*>(m_view->scene());
}

void MainWindow::showTagFactory()
{
    m_tagFactory->show();
}

void MainWindow::initSystemTray()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(Config::ImageFactory::icon(Config::Image::application));
    m_trayIcon->show();
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    setWindowIcon(Config::ImageFactory::icon(Config::Image::application));

    QAction * minimizeAction = new QAction(tr("Mi&nimize"), this);
    minimizeAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    QAction * maximizeAction = new QAction(tr("Ma&ximize"), this);
    maximizeAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    QAction * restoreAction = new QAction(tr("&Restore"), this);
    restoreAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction * quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu * trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        setVisible(!isVisible());
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}
