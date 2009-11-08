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

#include "ToolBarScene.h"

#include <QStyle>

#include <KAction>
#include <kactioncollection.h>
#include <kfontaction.h>
#include <kfontcombobox.h>

#include "../interface/ITextEdition.h"
#include "../interface/IVisualItemEdition.h"
#include "../main/main_window.h"
#include "../config/ImageFactory.h"
#include "../widget/ColorCombo.h"


namespace Scene
{

    ToolBarScene::ToolBarScene( MainWindow * mainWindow ):
            m_mainWindow(mainWindow)
    {
        KAction * a = 0;

        m_fontComboBox = new KFontComboBox();
        m_fontComboBox->setToolTip("Text Font");
        a = new KAction(this);
        a->setDefaultWidget(m_fontComboBox);
        connect(m_fontComboBox, SIGNAL(currentFontChanged(const QFont &)), SLOT(setFontFamily(const QFont &)));
        m_mainWindow->actionCollection()->addAction("fontfamily", a);

        m_actionFontSize = new KFontSizeAction(0);
        m_actionFontSize->setToolTip("Text Size");
        connect(m_actionFontSize, SIGNAL(fontSizeChanged(int)), SLOT(setFontPointSize(int)));
        m_mainWindow->actionCollection()->addAction("fontsize", m_actionFontSize);

        m_colorText = new KColorCombo();
        m_colorText->setFixedWidth(48);
        m_colorText->setToolTip("Text Color");
        a = new KAction(this);
        a->setDefaultWidget(m_colorText);
        connect(m_colorText, SIGNAL(activated(const QColor &)), SLOT(setTextColor(const QColor &)));
        m_mainWindow->actionCollection()->addAction("fontcolor", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textBold),tr("Bold"));
        a->setShortcutConfigurable(true);
        m_mainWindow->actionCollection()->addAction("bold", a);
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setBold(bool)));

        a = addAction(Config::ImageFactory::icon(Config::Image::textItalic),tr("Italic"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setItalic(bool)));
        m_mainWindow->actionCollection()->addAction("italic", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textUnderline),tr("Underline"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setFontUnderline(bool)));
        m_mainWindow->actionCollection()->addAction("underline", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textStrikeOut),tr("Strikeout"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setFontStrikeOut(bool)));
        m_mainWindow->actionCollection()->addAction("strikeout", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textJustifyLeft),tr("Align Left"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentLeft(bool)));
        m_mainWindow->actionCollection()->addAction("alignleft", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textJustifyCenter),tr("Align Center"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentCenter(bool)));
        m_mainWindow->actionCollection()->addAction("aligncenter", a);

        a = addAction(Config::ImageFactory::icon(Config::Image::textJustifyRight),tr("Align Right"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentRight(bool)));
        m_mainWindow->actionCollection()->addAction("alignright", a);

        m_colorItem = new KColorCombo();
        m_colorItem->setFixedWidth(64);
        m_colorItem->setToolTip("Note Color");
        a = new KAction(this);
        a->setDefaultWidget(m_colorItem);
        connect(m_colorItem, SIGNAL(activated(const QColor &)), SLOT(setColorItem(const QColor &)));
        m_mainWindow->actionCollection()->addAction("notecolor", a);

        KStandardAction::actualSize(mainWindow->currentView(),SLOT(resetZoom()),m_mainWindow->actionCollection());

        KStandardAction::zoomIn(mainWindow->currentView(),SLOT(doubleZoom()),m_mainWindow->actionCollection());

        KStandardAction::zoomOut(mainWindow->currentView(),SLOT(halfZoom()),m_mainWindow->actionCollection());

        a = KStandardAction::fitToPage(mainWindow->currentView(),SLOT(fitInViewZoom()),m_mainWindow->actionCollection());
        a->setIcon(Config::ImageFactory::icon(Config::Image::zoomFitBest));

        a = addAction("List Scene");
        connect(a, SIGNAL(triggered(bool)), mainWindow, SLOT(layoutScene()));
        m_mainWindow->actionCollection()->addAction(Scene::LayoutScene::type, a);

        a = addAction("Free Scene");
        connect(a, SIGNAL(triggered(bool)), mainWindow, SLOT(freeScene()));
        m_mainWindow->actionCollection()->addAction(Scene::FreeScene::type, a);

        m_toolBar = this;
    }

    KAction * ToolBarScene::addAction( const KIcon & icon, QString name )
    {
        KAction * a = new KAction( icon, name, this );
        a->setIcon(icon);
        a->setCheckable( true );

        return a;
    }

    KAction * ToolBarScene::addAction( QString name )
    {
        KAction * a = new KAction( name, this);
        a->setCheckable( true );

        return a;
    }

    Item::AbstractItem * ToolBarScene::currentAbstractItem()
    {
        if ( m_mainWindow->currentScene() != 0 )
        {
            return m_mainWindow->currentScene()->currentAbstractItem();
        }

        return 0;
    }

    void ToolBarScene::setBold( bool checked )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setBold(checked);
        }
    }

    void ToolBarScene::setItalic( bool checked )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setItalic(checked);
        }
    }

    void ToolBarScene::setFontStrikeOut( bool strikeOut )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setFontStrikeOut(strikeOut);
        }
    }

    void ToolBarScene::setAlignmentRight( bool checked )
    {
        Q_UNUSED( checked );

        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignRight);
        }
    }

    void ToolBarScene::setAlignmentCenter( bool checked )
    {
        Q_UNUSED( checked );

        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignCenter);
        }
    }

    void ToolBarScene::setAlignmentLeft( bool checked )
    {
        Q_UNUSED( checked );

        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignLeft);
        }
    }

    void ToolBarScene::setFontUnderline( bool checked )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setFontUnderline(checked);
        }
    }

    void ToolBarScene::setTextColor( const QColor & c )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setTextColor(c);
        }
    }

    void ToolBarScene::setFontFamily( const QFont & font )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setFontFamily(font);
        }
    }

    void ToolBarScene::setFontPointSize( int size )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setFontPointSize(size);
        }
    }

    void ToolBarScene::setColorItem( const QColor & c )
    {
        if ( currentAbstractItem() != 0 )
        {
            currentAbstractItem()->setItemColor(c);
        }
    }

    void ToolBarScene::currentItemChanged( Item::AbstractItem * item )
    {
        m_colorItem->setColor( item->color() );
    }

    void ToolBarScene::currentCharFormatChanged( const QTextCharFormat & f )
    {
        QAction * a = m_mainWindow->actionCollection()->action( "underline" );
        a->setChecked(f.fontUnderline());

        a = m_mainWindow->actionCollection()->action( "bold" );
        a->setChecked(f.fontWeight()>50);

        a = m_mainWindow->actionCollection()->action( "italic" );
        a->setChecked(f.fontItalic());

        m_actionFontSize->setFontSize( f.fontPointSize() );

        m_colorText->setColor(f.foreground().color());

        a = m_mainWindow->actionCollection()->action( "strikeout" );
        a->setChecked(f.fontStrikeOut());

        if ( currentAbstractItem() != 0 )
        {
            const QColor & c = currentAbstractItem()->itemColor();
            m_colorItem->setColor( c );
        }
    }

    ToolBarScene * ToolBarScene::m_toolBar = 0;

    ToolBarScene * ToolBarScene::toolBarScene()
    {
        return m_toolBar;
    }

}
