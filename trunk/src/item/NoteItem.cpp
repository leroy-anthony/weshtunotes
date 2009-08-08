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

#include "NoteItem.h"

#include <QDebug>
#include <QEvent>
#include <QPlainTextEdit>
#include <QPlainTextDocumentLayout>
#include <QScrollBar>
#include <QTextCursor>
#include <QFile>
#include <QDir>

#include "../scene/ToolBarScene.h"
#include "../config/Configuration.h"
#include "../tag/NoteTag.h"

namespace Item
{

    NoteItem::NoteItem(QWidget * parent) :
            AbstractItem(parent)
    {
        setContentsMargins(0,0,0,0);

        m_horizontalLayout = new QHBoxLayout(this);
        m_horizontalLayout->setMargin(0);
        m_horizontalLayout->setContentsMargins(0,0,0,0);
        m_horizontalLayout->setSpacing(0);
        m_horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);

        m_plainTextEdit = new CustomTextEdit();
        m_tag = new Tag::NoteTag( this, "default" );

        m_horizontalLayout->addWidget(m_tag);
        m_horizontalLayout->addWidget(m_plainTextEdit);

        setItemColor( QColor("#F7F7C8") ); //Fixme : à mettre dans préférence (du panier ?)

        connect( m_plainTextEdit, SIGNAL(selectionChanged()),  this, SLOT(edit()));
    }

    NoteItem::~NoteItem()
    {
    }

    void NoteItem::edit()
    {
        emit AbstractItem::editItem((AbstractItem*)this);
    }

    void NoteItem::setBold( bool checked )
    {
        if ( checked )
        {
            m_plainTextEdit->setFontWeight( QFont::Black );
        }
        else
        {
            m_plainTextEdit->setFontWeight( QFont::Normal );
        }
    }

    void NoteItem::setAlignment( Qt::Alignment a )
    {
        m_plainTextEdit->setAlignment( a );
    }

    void NoteItem::setItalic( bool italic )
    {
        m_plainTextEdit->setFontItalic( italic );
    }

    void NoteItem::setFontFamily( const QFont & font )
    {
        m_plainTextEdit->setFontFamily(font.family());
    }

    void NoteItem::setFontUnderline( bool underline )
    {
        m_plainTextEdit->setFontUnderline( underline );
    }

    void NoteItem::setFontStrikeOut( bool strikeOut )
    {
        QFont f(m_plainTextEdit->font());
        f.setStrikeOut( strikeOut );
        m_plainTextEdit->setFont(f);
    }

    void NoteItem::setFontPointSize( int weight )
    {
        m_plainTextEdit->setFontPointSize( weight );
    }

    void NoteItem::setTextBackgroundColor ( const QColor & c )
    {
        // IMPLEMENT ME
    }

    void NoteItem::setTextColor( const QColor & c )
    {
        m_plainTextEdit->setTextColor(c);
    }

    void NoteItem::save( const QString & fileName, const QString & handleId )
    {
        Config::Configuration settings( fileName );

        settings.beginGroup( handleId );
        settings.setValue("data",m_itemId);
        settings.setValue("color",m_color);

        QStringList namesTags;
        for ( int i=0 ; i<m_tags.size() ; ++i )
        {
            namesTags << m_tags[i]->name()+":" + m_tags[i]->currentStateName();
        }
        settings.setValue("tags",namesTags);


        settings.endGroup();
        settings.sync();

        QFileInfo fileInfo(settings.fileName());
        QFile f( fileInfo.absolutePath() + QDir::separator() + m_itemId + ".html" );
        f.open(QFile::WriteOnly | QFile::Text);
        QTextStream stream(&f);
        stream << m_plainTextEdit->document()->toHtml();
        f.close();
    }

    void NoteItem::load( const QString & fileName )
    {
        m_plainTextEdit->blockSignals( true );

        Config::Configuration settings( fileName );

        QFileInfo fileInfo(settings.fileName());
        QFile f( fileInfo.absolutePath() + QDir::separator() + m_itemId + ".html" );
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&f);
        m_plainTextEdit->setHtml(stream.readAll());
        f.close();

        m_plainTextEdit->adaptSizeFromText();

        m_plainTextEdit->blockSignals( false );
    }

    void NoteItem::setItemColor( const QColor & color )
    {
        setStyleSheet("background: "+color.name()+";");
        m_plainTextEdit->setStyleSheet(styleSheet());
        m_color = QColor(color);
        emit colorChange();
    }

    const QColor & NoteItem::itemColor()
    {
        return m_color;
    }

    void NoteItem::isSelected()
    {
        Scene::ToolBarScene * toolBar = Scene::ToolBarScene::toolBarScene();
        toolBar->currentCharFormatChanged( m_plainTextEdit->currentCharFormat() );
    }

    void NoteItem::addTag( const QString & tagName, const QString & tagState )
    {
        Tag::NoteTag * tag = new Tag::NoteTag( this, tagName );
        tag->setCurrentState( tagState );
        m_tags << tag;
        m_horizontalLayout->insertWidget(0,tag);
        tag->apply();
    }


    void NoteItem::tagApply( QAction * action )
    {
        QString tagName = action->text();
        Tag::NoteTag * tag = new Tag::NoteTag( this, tagName );
        m_tags << tag;
        m_horizontalLayout->insertWidget(0,tag);
        m_plainTextEdit->selectAll();
        tag->apply();
        m_plainTextEdit->undo();
    }

}
