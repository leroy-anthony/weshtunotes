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
#include <KTreeWidgetSearchLine>
#include <KLocalizedString>
#include <KMessageBox>


#include "settings.h"
#include "../tag/TagFactory.h"
#include "../explorer/TreeExplorer.h"
#include "../basket/ItemTreeBasket.h"
#include "../basket/ClipperBasket.h"
#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../config/ConfigDialog.h"
#include "../scene/CustomGraphicsView.h"
#include "../scene/FreeScene.h"
#include "../scene/LayoutScene.h"
#include "../scene/ToolBarScene.h"
#include "../scene/PlasmaScene.h"
#include "../data/AssociationManager.h"
#include "../data/ExportDataToXml.h"
#include "../data/ImportDataFromXml.h"
#include "../animation/AnimationManager.h"

KStatusBar * MainWindow::m_statusBar = 0;

Scene::CustomGraphicsView * MainWindow::m_view = 0;

Basket::ItemTreeBasket * MainWindow::m_lastBasketLoad = 0;

MainWindow::MainWindow( QWidget * parent ) :
        KXmlGuiWindow( parent )
{
    setupUi(this);

    initToolBar();
    initView();
    initExplorer();

    m_tagFactory = Tag::TagFactory::newTagFactory();
    m_tagFactory->loadTags();


    setupActions();

    initStatusBar();

    initSystemTray();

    setupGUI( Default, "kweshtunotesui.rc" );
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

    KAction * a = new KAction( i18n("Configure Tag..."), 0);
    actionCollection()->addAction( "tag", a );
    connect( a, SIGNAL(triggered(bool)), m_tagFactory, SLOT(show()) );

    KStandardAction::preferences(this, SLOT(showSettings()), actionCollection());
}

void MainWindow::save()
{
    statusBar()->showMessage( i18n("Save"), 100 );

    if ( m_lastBasketLoad != 0 )
    {
        m_lastBasketLoad->basket()->save();

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
    m_controllerScene = new Scene::ToolBarScene( this );
}

void MainWindow::initView()
{
    QWidget * widget = new QWidget();

    m_view = new Scene::CustomGraphicsView();
    centralwidget->layout()->setMargin(0);
    centralwidget->layout()->addWidget( m_view );
    centralwidget->layout()->addWidget( widget );

    QLayout * layoutButtonView = new QHBoxLayout( widget );
    layoutButtonView->setMargin(0);
    layoutButtonView->setSpacing(2);

    layoutButtonView->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    QToolButton * q1 = new QToolButton();
    q1->setIcon(Config::ImageFactory::newInstance()->icon("zoom-original"));
    q1->setToolTip(i18n("Zoom 1:1"));
    connect( q1, SIGNAL(clicked()), m_view, SLOT(resetZoom()) );
    layoutButtonView->addWidget(q1);

    QToolButton * q2 = new QToolButton();
    q2->setIcon(Config::ImageFactory::newInstance()->icon("zoom-in"));
    q2->setToolTip(i18n("Zoom in"));
    connect( q2, SIGNAL(clicked()), m_view, SLOT(doubleZoom()) );
    layoutButtonView->addWidget(q2);

    QToolButton * q3 = new QToolButton();
    q3->setIcon(Config::ImageFactory::newInstance()->icon("zoom-out"));
    q3->setToolTip(i18n("Zoom out"));
    connect( q3, SIGNAL(clicked()), m_view, SLOT(halfZoom()) );
    layoutButtonView->addWidget(q3);

    QToolButton * q4 = new QToolButton();
    q4->setIcon(Config::ImageFactory::newInstance()->icon("zoom-fit-best"));
    q4->setToolTip(i18n("Zoom fit"));
    connect( q4, SIGNAL(clicked()), m_view, SLOT(fitInViewZoom()) );
    layoutButtonView->addWidget(q4);

    QToolButton * q5 = new QToolButton();
    q5->setIcon(Config::ImageFactory::newInstance()->icon("shapes"));
    q5->setToolTip(i18n("Zoom center"));
    connect( q5, SIGNAL(clicked()), m_view, SLOT(centerZoom()) );
    layoutButtonView->addWidget(q5);

    layoutButtonView->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    QToolButton * q6 = new QToolButton();
    q6->setIcon(Config::ImageFactory::newInstance()->icon("go-previous"));
    q6->setToolTip(i18n("Previous note"));
    connect( q6, SIGNAL(clicked()), m_view, SLOT(prevItem()) );
    layoutButtonView->addWidget(q6);

    QToolButton * q7 = new QToolButton();
    q7->setIcon(Config::ImageFactory::newInstance()->icon("go-next"));
    q7->setToolTip(i18n("Next note"));
    connect( q7, SIGNAL(clicked()), m_view, SLOT(nextItem()) );
    layoutButtonView->addWidget(q7);

    layoutButtonView->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Ignored));

    Animation::AnimationManager::initAnimation(m_view);
}

void MainWindow::initExplorer()
{
    m_treeExplorer = new Explorer::TreeExplorer(this);

    // creation du layout du dock
    QLayout * layoutDock = new QVBoxLayout( dockWidgetContents );
    layoutDock->setMargin(2);
    layoutDock->setSpacing(0);

    QWidget * widget = new QWidget();
    dockFichier->setTitleBarWidget( widget );
    dockFichier->titleBarWidget()->setContentsMargins(6,4,0,0);

    QLayout * layoutButtonExplorer = new QHBoxLayout( widget );
    layoutButtonExplorer->setMargin(0);
    layoutButtonExplorer->setSpacing(0);

    QToolButton * q1 = new QToolButton();
    q1->setIcon(Config::ImageFactory::newInstance()->icon("folder-new"));
    q1->setToolTip(i18n("New root basket"));
    connect( q1, SIGNAL(clicked()), m_treeExplorer, SLOT(addBasketToRoot()) );
    layoutButtonExplorer->addWidget(q1);

    QToolButton * q3 = new QToolButton();
    q3->setIcon(Config::ImageFactory::newInstance()->icon("edit-delete"));
    q3->setToolTip(i18n("Delete basket"));
    connect( q3, SIGNAL(clicked()), m_treeExplorer, SLOT(delCurrentBasket()) );
    layoutButtonExplorer->addWidget(q3);

    KTreeWidgetSearchLine * lineEdit = new KTreeWidgetSearchLine();
    lineEdit->addTreeWidget(m_treeExplorer);
    layoutButtonExplorer->addWidget(lineEdit);

    layoutDock->addWidget( m_treeExplorer );

    connect( m_treeExplorer, SIGNAL(itemClicked(QTreeWidgetItem*,int)),                   this, SLOT(loadScene(QTreeWidgetItem*,int)) );
    connect( m_treeExplorer, SIGNAL(delCurrentBasketRequest()),                           this, SLOT(delCurrentBasket()) );
    connect( m_treeExplorer, SIGNAL(addToCurrentBasketRequest(Basket::ItemTreeBasket *)), this, SLOT(addBasket(Basket::ItemTreeBasket *)) );
}

void MainWindow::addBasket( Basket::ItemTreeBasket * item )
{
    if ( item != 0 )
    {
        item->basket()->save();
        loadScene( item , 0 );
    }
}

void MainWindow::delCurrentBasket()
{
    m_lastBasketLoad = 0;
    QList<QTreeWidgetItem*> items = m_treeExplorer->selectedItems();
    if ( items.size() > 0 )
    {
        loadScene(items[0]);
    }
    else
    {
        m_view->setScene( new Scene::FreeScene() );
        m_view->setDisabled(true);
    }
}


void MainWindow::reloadView()
{
    if ( m_lastBasketLoad != 0 )
    {
        Scene::AbstractScene * scene = m_lastBasketLoad->basket()->scene();
        m_view->setScene( scene );
        scene->restoreView( m_view );
        m_view->adjustLayoutSize();

        showMessage( i18np("1 item","%1 items",scene->handles().size()), 0 );
    }
}


void MainWindow::loadScene( QTreeWidgetItem * item , int column )
{
    Q_UNUSED(column);

    Basket::ItemTreeBasket * i = static_cast<Basket::ItemTreeBasket*>(item);

    if ( m_lastBasketLoad != 0 )
    {
        m_lastBasketLoad->basket()->save();
    }

    m_lastBasketLoad = i;

    i->basket()->load();

    m_currentScene = i->basket()->scene();

    reloadView();

    Animation::AnimationManager::startLoad( m_currentScene->scene()->items() );

    m_view->update();
}

Scene::AbstractScene * MainWindow::currentScene()
{
    return m_currentScene;
}

Scene::CustomGraphicsView * MainWindow::currentView()
{
    return m_view;
}

void MainWindow::initSystemTray()
{
    m_trayIcon = new KSystemTrayIcon(this);
    m_trayIcon->setIcon(Config::ImageFactory::newInstance()->icon("basket.png"));
    m_trayIcon->show();
    setWindowIcon(Config::ImageFactory::newInstance()->icon("basket.png"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainWindow::showSettings()
{
    new Config::ConfigDialog( this );
}


