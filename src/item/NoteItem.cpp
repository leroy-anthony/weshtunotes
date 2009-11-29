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
#include <QUrl>
#include <QCoreApplication>


#include "../scene/ToolBarScene.h"
#include "../config/Configuration.h"
#include "../tag/NoteTag.h"
#include "../config/ImageFactory.h"

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
        m_addTag = new Tag::AddTag( this );

        m_horizontalLayout->addWidget(m_addTag);
        m_horizontalLayout->addWidget(m_plainTextEdit);

        connect( m_plainTextEdit, SIGNAL(selectionChanged()),  this, SLOT(edit()));
    }

    NoteItem::~NoteItem()
    {
        delete m_addTag;

        for ( int i=0 ; i<m_tags.size() ; ++i )
        {
            delete m_tags[i];
        }
    }

    void NoteItem::adaptSize()
    {
        m_plainTextEdit->adaptSizeFromText();
    }

    void NoteItem::edit()
    {
        Scene::ToolBarScene * toolBar = Scene::ToolBarScene::toolBarScene();
        toolBar->currentItemChanged( this );
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

        settings.setValue(handleId,"data",m_nameId);
        settings.setValue(handleId,"color",m_color.name());

        QStringList namesTags;
        for ( int i=0 ; i<m_tags.size() ; ++i )
        {
            namesTags << m_tags[i]->name()+":" + m_tags[i]->currentStateName();
        }
        settings.setValue(handleId,"tags",namesTags);

        Config::Configuration::saveNote( settings.fileName(), m_plainTextEdit->document()->toHtml(), m_nameId );
    }

    void NoteItem::load( const QString & fileName )
    {       
        m_plainTextEdit->blockSignals( true );

        m_plainTextEdit->setHtml( Config::Configuration::loadNote( fileName, m_nameId ) );
        m_plainTextEdit->adaptSizeFromText();

        m_plainTextEdit->blockSignals( false );

        m_fileName = fileName; 
    }

    void NoteItem::load( const QMimeData * mimeData )
    {
        m_plainTextEdit->blockSignals( true );

        m_plainTextEdit->addData( mimeData );
        m_plainTextEdit->adaptSizeFromText();

        m_plainTextEdit->blockSignals( false );

        update();
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
        if ( action->isChecked() )
        {
            QString tagName = action->data().toString();

            Tag::NoteTag * tag = new Tag::NoteTag( this, tagName );
            m_tags << tag;
            m_horizontalLayout->insertWidget(0,tag);
            m_plainTextEdit->selectAll();
            tag->apply();
            m_plainTextEdit->selectNone();
        }
        else
        {
            // supprimer
            QString tagName = action->data().toString();
            for ( int i=0 ; i<m_tags.size() ; ++i )
            {
                if ( m_tags[i]->name() == tagName )
                {
                    Tag::NoteTag * tag = m_tags[i];
                    m_horizontalLayout->removeWidget(m_tags[i]);
                    m_tags.removeAt(i);
                    delete tag;
                    break;
                }
            }
            load( m_fileName );
            for ( int i=0 ; i<m_tags.size() ; ++i )
            {
                m_plainTextEdit->selectAll();
                m_tags[i]->apply();
                m_plainTextEdit->selectNone();
            }
        }
    }

    void NoteItem::tagApply()
    {
        for ( int i=0 ; i<m_tags.size() ; ++i )
        {
            m_plainTextEdit->selectAll();
            m_tags[i]->apply();
            m_plainTextEdit->selectNone();
        }
    }

    void NoteItem::keyPressEvent ( QKeyEvent * keyEvent )
    {
        QCoreApplication::sendEvent( m_plainTextEdit, keyEvent );
    }

    void NoteItem::insertData( const QMimeData * data )
    {
        m_plainTextEdit->addData( data );
    }

}
