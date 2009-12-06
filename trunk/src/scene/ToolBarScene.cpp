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


#include <iostream>

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

        m_boldAction = addAction(Config::ImageFactory::newInstance()->icon("format-text-bold.png"),tr("Bold"));
        m_mainWindow->actionCollection()->addAction("bold", m_boldAction);
        connect(m_boldAction, SIGNAL(triggered(bool)), this, SLOT(setBold(bool)));

        m_italicAction = addAction(Config::ImageFactory::newInstance()->icon("format-text-italic.png"),tr("Italic"));
        connect(m_italicAction, SIGNAL(triggered(bool)), this, SLOT(setItalic(bool)));
        m_mainWindow->actionCollection()->addAction("italic", m_italicAction);

        m_underlineAction = addAction(Config::ImageFactory::newInstance()->icon("format-text-underline.png"),tr("Underline"));
        connect(m_underlineAction, SIGNAL(triggered(bool)), this, SLOT(setFontUnderline(bool)));
        m_mainWindow->actionCollection()->addAction("underline", m_underlineAction);

        m_strikeoutAction = addAction(Config::ImageFactory::newInstance()->icon("format-text-strikethrough.png"),tr("Strikeout"));
        connect(m_strikeoutAction, SIGNAL(triggered(bool)), this, SLOT(setFontStrikeOut(bool)));
        m_mainWindow->actionCollection()->addAction("strikeout", m_strikeoutAction);

        m_alignleftAction = addAction(Config::ImageFactory::newInstance()->icon("format-justify-left.png"),tr("Align Left"));
        connect(m_alignleftAction, SIGNAL(triggered(bool)), this, SLOT(setAlignmentLeft(bool)));
        m_mainWindow->actionCollection()->addAction("alignleft", m_alignleftAction);

        m_aligncenterAction = addAction(Config::ImageFactory::newInstance()->icon("format-justify-center.png"),tr("Align Center"));
        connect(m_aligncenterAction, SIGNAL(triggered(bool)), this, SLOT(setAlignmentCenter(bool)));
        m_mainWindow->actionCollection()->addAction("aligncenter", m_aligncenterAction);

        m_alignrightAction = addAction(Config::ImageFactory::newInstance()->icon("format-justify-right.png"),tr("Align Right"));
        connect(m_alignrightAction, SIGNAL(triggered(bool)), this, SLOT(setAlignmentRight(bool)));
        m_mainWindow->actionCollection()->addAction("alignright", m_alignrightAction);

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
        a->setIcon(Config::ImageFactory::newInstance()->icon("zoom-fit-best.png"));

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
            m_aligncenterAction->setChecked(false);
            m_alignleftAction->setChecked(false);

            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignRight);
        }
    }

    void ToolBarScene::setAlignmentCenter( bool checked )
    {
        Q_UNUSED( checked );

        if ( currentAbstractItem() != 0 )
        {
            m_alignrightAction->setChecked(false);
            m_alignleftAction->setChecked(false);

            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignCenter);
        }
    }

    void ToolBarScene::setAlignmentLeft( bool checked )
    {
        Q_UNUSED( checked );

        if ( currentAbstractItem() != 0 )
        {
            m_alignrightAction->setChecked(false);
            m_aligncenterAction->setChecked(false);

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

        m_alignrightAction->setChecked(false);
        m_aligncenterAction->setChecked(false);
        m_alignleftAction->setChecked(false);

        Qt::Alignment alignment = dynamic_cast<Item::ITextEdition*>(item)->alignment();

        if ( alignment == Qt::AlignRight )
        {
            m_alignrightAction->setChecked(true);
        }

        if ( alignment == Qt::AlignHCenter )
        {
            m_aligncenterAction->setChecked(true);
        }

        if ( alignment == Qt::AlignLeft )
        {
            m_alignleftAction->setChecked(true);
        }
    }

    void ToolBarScene::currentCharFormatChanged( const QTextCharFormat & f )
    {
        m_underlineAction->setChecked(f.fontUnderline());
        m_boldAction->setChecked(f.fontWeight()>50);
        m_italicAction->setChecked(f.fontItalic());
        m_actionFontSize->setFontSize( f.fontPointSize() );
        m_colorText->setColor(f.foreground().color());
        m_strikeoutAction->setChecked(f.fontStrikeOut());
    }

    ToolBarScene * ToolBarScene::m_toolBar = 0;

    ToolBarScene * ToolBarScene::toolBarScene()
    {
        return m_toolBar;
    }

}
