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

MainWindow::MainWindow(QWidget * parent, int argc, char *argv[]) :
        QMainWindow(parent)
{
    setupUi(this);

    initView();
    initToolBar();
    initMedia();

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

    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

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
    qq->setIcon(QIcon("icon:document-new.png"));
    connect( qq, SIGNAL(clicked()), m_treeExplorer, SLOT(addBasketToRoot()) );
    layoutButtonExplorer->addWidget(qq);

    QToolButton * q = new QToolButton();
    q->setIcon(QIcon("icon:list-add.png"));
    connect( q, SIGNAL(clicked()), m_treeExplorer, SLOT(addToCurrentBasket()) );
    layoutButtonExplorer->addWidget(q);

    QToolButton * qqq = new QToolButton();
    qqq->setIcon(QIcon("icon:edit-delete.png"));
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
