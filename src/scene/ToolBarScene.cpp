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

#include "../item/ITextEdition.h"
#include "../item/IVisualItemEdition.h"
#include "../main/main_window.h"

namespace Scene
{

    ToolBarScene * ToolBarScene::m_toolBarScene = 0;

    ToolBarScene * ToolBarScene::toolBarScene()
    {
        return m_toolBarScene;
    }

    ToolBarScene::ToolBarScene( MainWindow * mainWindow ):
            m_mainWindow(mainWindow)
    {
        m_toolBar = m_mainWindow->addToolBar(tr("Text"));
        m_toolBar->setIconSize(QSize(24,24));

        QAction * a = 0;

        m_comboFont = new QFontComboBox();
        m_toolBar->addWidget(m_comboFont);
        connect(m_comboFont, SIGNAL(currentFontChanged(const QFont &)), SLOT(setFontFamily(const QFont &)));

        m_comboFontSize = new QComboBox();
        m_comboFontSize->addItem( "6" );
        m_comboFontSize->addItem( "7" );
        m_comboFontSize->addItem( "8" );
        m_comboFontSize->addItem( "9" );
        m_comboFontSize->addItem( "10" );
        m_comboFontSize->addItem( "11" );
        m_comboFontSize->addItem( "12" );
        m_comboFontSize->addItem( "14" );
        m_comboFontSize->addItem( "16" );
        m_comboFontSize->addItem( "18" );
        m_comboFontSize->addItem( "20" );
        m_comboFontSize->addItem( "22" );
        m_comboFontSize->addItem( "24" );
        m_comboFontSize->addItem( "26" );
        m_comboFontSize->addItem( "28" );
        m_comboFontSize->addItem( "36" );
        m_comboFontSize->addItem( "48" );
        m_toolBar->addWidget(m_comboFontSize);
        connect(m_comboFontSize, SIGNAL(currentIndexChanged(const QString &)), SLOT(setFontPointSize(const QString &)));

        m_colorText = new QtColorPicker();
        m_colorText->setStandardColors();
        m_toolBar->addWidget(m_colorText);
        connect(m_colorText, SIGNAL(colorChanged(const QColor &)), SLOT(setTextColor(const QColor &)));

        //TODO classe pour récupérer une seule instance d'icone.

        a = addAction(QIcon("icon:format-text-bold.png"),tr("bold"));
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setBold(bool)));
        a = addAction(QIcon("icon:format-text-italic.png"),"italic");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setItalic(bool)));
        a = addAction(QIcon("icon:format-text-underline.png"),"underLine");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setFontUnderline(bool)));
        a = addAction(QIcon("icon:format-text-strikethrough.png"),"strikeout");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setFontStrikeOut(bool)));

        a = addAction(QIcon("icon:format-justify-left.png"),"alignLeft");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentLeft(bool)));
        a = addAction(QIcon("icon:format-justify-center.png"),"alignCenter");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentCenter(bool)));
        a = addAction(QIcon("icon:format-justify-right.png"),"alignRight");
        connect(a, SIGNAL(triggered(bool)), this, SLOT(setAlignmentRight(bool)));

        m_toolBar->addSeparator();

        m_colorItem = new QtColorPicker();
        m_colorItem->setStandardColors();
        m_toolBar->addWidget(m_colorItem);
        connect(m_colorItem, SIGNAL(colorChanged(const QColor &)), SLOT(setColorItem(const QColor &)));

        m_toolBarScene = this;
    }

    QAction * ToolBarScene::addAction( const QIcon & icon, QString name )
    {
        QAction * a = new QAction( icon, name, this );
        a->setIcon(icon);
        a->setCheckable( true );

        m_toolBar->addAction(a);
        m_actions.insert(name,a);

        return a;
    }

    QAction * ToolBarScene::addAction( QString name )
    {
        QAction * a = new QAction( name, this);
        a->setCheckable( true );
        m_toolBar->addAction(a);
        m_actions.insert(name,a);

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
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignRight);
        }
    }

    void ToolBarScene::setAlignmentCenter( bool checked )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setAlignment(Qt::AlignCenter);
        }
    }

    void ToolBarScene::setAlignmentLeft( bool checked )
    {
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

    void ToolBarScene::setFontPointSize( const QString & size )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::ITextEdition*>(currentAbstractItem())->setFontPointSize(size.toInt());
        }
    }

    void ToolBarScene::setColorItem( const QColor & c )
    {
        if ( currentAbstractItem() != 0 )
        {
            dynamic_cast<Item::IVisualItemEdition*>(currentAbstractItem())->setItemColor(c);
        }
    }

    void ToolBarScene::currentCharFormatChanged( const QTextCharFormat & f )
    {
        QAction * a = m_actions[ "underLine" ];
        a->setChecked(f.fontUnderline());

        a = m_actions[ "bold" ];
        a->setChecked(f.fontWeight()>50);

        a = m_actions[ "italic" ];
        a->setChecked(f.fontItalic());

        m_comboFontSize->setCurrentIndex( m_comboFontSize->findText( QString("%1").arg(f.fontPointSize()) ) );

        m_comboFont->setCurrentIndex( m_comboFont->findText(f.font().family()) );

        m_colorText->setCurrentColor(f.foreground().color());

        if ( currentAbstractItem() != 0 )
        {
            const QColor & c = dynamic_cast<Item::IVisualItemEdition*>(currentAbstractItem())->itemColor();
            m_colorItem->setCurrentColor( c );
        }
    }

}
