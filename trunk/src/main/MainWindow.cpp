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

#include "MainWindow.h"

#include <QSpacerItem>
#include <QtDBus/QtDBus>
#include <QCalendarWidget>
#include <QScrollBar>

#include <KActionCollection>
#include <KStandardAction>
#include <KIconDialog>
#include <KConfigDialog>
#include <KStatusBar>
#include <KDirModel>
#include <KDirLister>
#include <KFileMetaInfo>
#include <KSystemTrayIcon>
#include <KAction>

#include "settings.h"
#include "../tag/TagFactory.h"
#include "../explorer/TreeExplorer.h"
#include "../basket/ItemTreeBasket.h"
#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../config/ConfigDialog.h"
#include "../config/GeneralPageDialog.h"
#include "../config/AppareancePageDialog.h"
#include "../scene/CustomGraphicsView.h"
#include "../scene/FreeScene.h"
#include "../scene/LayoutScene.h"
#include "../scene/ToolBarScene.h"

KStatusBar * MainWindow::m_statusBar = 0;

MainWindow::MainWindow( QWidget * parent ) :
        KXmlGuiWindow( parent ),
        m_lastBasketLoad(0)
{
    setupUi(this);

    initView();
    initExplorer();

    m_tagFactory = Tag::TagFactory::newTagFactory();
    m_tagFactory->loadTags();

    setupActions();

    initStatusBar();
    initToolBar();
    initSystemTray();

    setupGUI( Default, "kweshtunotesui.rc" );

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
    else
    {
        m_view->setScene( new Scene::FreeScene() );
    }
}

void MainWindow::setupActions()
{
    KStandardAction::save(this, SLOT(save()), actionCollection());
    KStandardAction::quit(qApp, SLOT(quit()), actionCollection());
    KStandardAction::paste(m_view, SLOT(paste()), actionCollection());
    KStandardAction::copy(m_view, SLOT(copy()), actionCollection());

    KAction * a = new KAction("Configure Tag...",0);
    actionCollection()->addAction( "tag", a );
    connect( a, SIGNAL(triggered(bool)), m_tagFactory, SLOT(show()) );

    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());
}

void MainWindow::save()
{
    statusBar()->showMessage( "save", 100 );

    if ( m_lastBasketLoad != 0 )
    {
        m_view->horizontalScrollBar()->setValue(m_view->horizontalScrollBar()->value());
        m_view->verticalScrollBar()->setValue(m_view->verticalScrollBar()->value());
        m_lastBasketLoad->basket()->scene()->storeView( m_view );
    }

    m_treeExplorer->saveBaskets();

    if ( m_lastBasketLoad != 0 )
    {
        Config::Configuration::saveLastBasket( m_lastBasketLoad->basket()->id() );
    }
}

void MainWindow::initStatusBar()
{
    m_statusBar = KMainWindow::statusBar();
}

void MainWindow::showMessage( const QString & message, int time )
{
    if ( m_statusBar != 0 )
    {
        m_statusBar->showMessage( message, time );
    }
}

KStatusBar * MainWindow::statusBar()
{
    return m_statusBar;
}

void MainWindow::initToolBar()
{
    m_controlerScene = new Scene::ToolBarScene( this );
}

void MainWindow::initView()
{
    QWidget * widget = new QWidget();

    m_view = new Scene::CustomGraphicsView();
    centralwidget->layout()->setMargin(0);
    centralwidget->layout()->addWidget( m_view );
    centralwidget->layout()->addWidget( widget );
    m_view->show();

    QLayout * layoutButtonView = new QHBoxLayout( widget );
    layoutButtonView->setMargin(0);
    layoutButtonView->setSpacing(2);

    layoutButtonView->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    QToolButton * q1 = new QToolButton();
    q1->setIcon(Config::ImageFactory::newInstance()->icon("zoom-original.png"));
    q1->setToolTip("Zoom 1:1");
    connect( q1, SIGNAL(clicked()), m_view, SLOT(resetZoom()) );
    layoutButtonView->addWidget(q1);

    QToolButton * q2 = new QToolButton();
    q2->setIcon(Config::ImageFactory::newInstance()->icon("zoom-in.png"));
    q2->setToolTip("Zoom in");
    connect( q2, SIGNAL(clicked()), m_view, SLOT(doubleZoom()) );
    layoutButtonView->addWidget(q2);

    QToolButton * q3 = new QToolButton();
    q3->setIcon(Config::ImageFactory::newInstance()->icon("zoom-out.png"));
    q3->setToolTip("Zoom out");
    connect( q3, SIGNAL(clicked()), m_view, SLOT(halfZoom()) );
    layoutButtonView->addWidget(q3);

    QToolButton * q4 = new QToolButton();
    q4->setIcon(Config::ImageFactory::newInstance()->icon("zoom-fit-best.png"));
    q4->setToolTip("Zoom fit");
    connect( q4, SIGNAL(clicked()), m_view, SLOT(fitInViewZoom()) );
    layoutButtonView->addWidget(q4);

    layoutButtonView->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));
}

void MainWindow::initExplorer()
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

    QToolButton * q1 = new QToolButton();
    q1->setIcon(Config::ImageFactory::newInstance()->icon("document-new.png"));
    q1->setToolTip("New root basket");
    connect( q1, SIGNAL(clicked()), this, SLOT(addBasketToRoot()) );
    layoutButtonExplorer->addWidget(q1);

    QToolButton * q2 = new QToolButton();
    q2->setIcon(Config::ImageFactory::newInstance()->icon("list-add.png"));
    q2->setToolTip("Add basket");
    connect( q2, SIGNAL(clicked()), this, SLOT(addToCurrentBasket()) );
    layoutButtonExplorer->addWidget(q2);

    QToolButton * q3 = new QToolButton();
    q3->setIcon(Config::ImageFactory::newInstance()->icon("edit-delete.png"));
    q3->setToolTip("Delete basket");
    connect( q3, SIGNAL(clicked()), this, SLOT(delCurrentBasket()) );
    layoutButtonExplorer->addWidget(q3);

    layoutButtonExplorer->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    layoutDock->addWidget( m_treeExplorer );

    connect(m_treeExplorer,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(loadScene(QTreeWidgetItem*,int)));
}

void MainWindow::addToCurrentBasket()
{
    Basket::ItemTreeBasket * item = m_treeExplorer->addToCurrentBasket();
    if ( item != 0 )
    {
        item->basket()->save();
        loadScene( item , 0 );
    }
}

void MainWindow::addBasketToRoot()
{
    Basket::ItemTreeBasket * item = m_treeExplorer->addBasketToRoot();
    if ( item != 0 )
    {
        item->basket()->save();
        loadScene( item , 0 );
    }
}

void MainWindow::delCurrentBasket()
{
    m_lastBasketLoad = 0;
    m_treeExplorer->delCurrentBasket();
    QList<QTreeWidgetItem*> items = m_treeExplorer->selectedItems();
    if ( items.size() > 0 )
    {
        loadScene(items[0]);
    }
}

void MainWindow::loadScene( QTreeWidgetItem * item , int column )
{
    Basket::ItemTreeBasket * i = dynamic_cast<Basket::ItemTreeBasket*>(item);

    if ( m_lastBasketLoad != 0 )
    {
        m_lastBasketLoad->basket()->save();
    }

    m_lastBasketLoad = i;

    i->basket()->load();

    Scene::AbstractScene * scene = i->basket()->scene();
    m_view->setScene( scene );
    i->basket()->scene()->restoreView( m_view );

    m_view->update();

    if ( m_lastBasketLoad->basket()->scene()->type() == Scene::FreeScene::type )
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

void MainWindow::layoutScene()
{
    m_lastBasketLoad->basket()->scene()->setType( Scene::LayoutScene::type );
    m_lastBasketLoad->basket()->save();
    m_lastBasketLoad->basket()->load();

    m_view->setScene( m_lastBasketLoad->basket()->scene() );

    actionCollection()->action( Scene::LayoutScene::type )->setDisabled(true);
    actionCollection()->action( Scene::LayoutScene::type )->setChecked(true);

    actionCollection()->action( Scene::FreeScene::type )->setChecked(false);
    actionCollection()->action( Scene::FreeScene::type )->setDisabled(false);
}

void MainWindow::freeScene()
{
    m_lastBasketLoad->basket()->scene()->setType( Scene::FreeScene::type );
    m_lastBasketLoad->basket()->save();
    m_lastBasketLoad->basket()->load();

    m_view->setScene( m_lastBasketLoad->basket()->scene() );
    m_view->fitInViewZoom();
    m_view->resetZoom();

    actionCollection()->action( Scene::FreeScene::type )->setDisabled(true);
    actionCollection()->action( Scene::FreeScene::type )->setChecked(true);

    actionCollection()->action( Scene::LayoutScene::type )->setChecked(false);
    actionCollection()->action( Scene::LayoutScene::type )->setDisabled(false);
}

void MainWindow::initSystemTray()
{
    m_trayIcon = new KSystemTrayIcon(this);
    m_trayIcon->setIcon(Config::ImageFactory::newInstance()->icon("basket.png"));
    m_trayIcon->show();
    setWindowIcon(Config::ImageFactory::newInstance()->icon("basket.png"));

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
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

void MainWindow::showSettings()
{
    if(KConfigDialog::showDialog("settings"))
        return;

    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    dialog->resize(650,400);
    dialog->setFaceType(KPageDialog::List);
    dialog->addPage(new Config::GeneralPageDialog(0),"General" );
    dialog->addPage(new Config::AppareancePageDialog(0),"Appareance note" );
    dialog->show();

    connect( dialog, SIGNAL(settingsChanged()), this, SLOT(updateConfiguration()) );
}

void MainWindow::updateConfiguration()
{
    Settings::self()->writeConfig();
}
