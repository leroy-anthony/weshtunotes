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
#include <QPainter>
#include <QApplication>
#include <QRadialGradient>
#include <QHBoxLayout>

#include <Nepomuk/Resource>

#include "../scene/ToolBarScene.h"
#include "../config/Configuration.h"
#include "../tag/NoteTag.h"
#include "../tag/AddTag.h"
#include "../tag/NextTag.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"
#include "../data/DataManager.h"
#include "../handle/HandleItem.h"
#include "settings.h"

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

        m_plainTextEdit = new CustomTextEdit( this );

        m_addTag = new Tag::AddTag( this );
        m_nextTag = new Tag::NextTag( this );

        m_horizontalLayout->addWidget(m_addTag);
        m_horizontalLayout->addWidget(m_nextTag);
        m_horizontalLayout->addWidget(m_plainTextEdit);

        connect( m_plainTextEdit, SIGNAL(selectionChanged()),  this, SLOT(edit()));
    }

    NoteItem::~NoteItem()
    {
        delete m_addTag;
        delete m_nextTag;
        delete m_tag;
    }

    void NoteItem::adaptSize()
    {
        m_plainTextEdit->adaptSizeFromText();
    }

    void NoteItem::edit()
    {
        Scene::ToolBarScene * toolBar = Scene::ToolBarScene::toolBarScene();
        toolBar->currentItemChanged( this );
        toolBar->currentCharFormatChanged( m_plainTextEdit->textCursor().charFormat() );

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

    Qt::Alignment NoteItem::alignment()
    {
        return m_plainTextEdit->alignment();
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
        if ( weight > 0 )
        {
            m_plainTextEdit->setFontPointSize( weight );
        }
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
        Data::DataManager settings( fileName );

        settings.setValue(handleId,"data",GeneratorID::id());
        settings.setValue(handleId,"color",m_color.name());

        if ( m_tag != 0 )
        {
            settings.setValue(handleId,"tag",m_tag->name()+":" + m_tag->currentStateName());

            addNoteToNepomuk();
        }
        else
        {
            settings.removeValue(handleId,"tag");
        }

        Data::DataManager::saveNote( m_plainTextEdit->document()->toHtml(), GeneratorID::id() );
    }

    void NoteItem::load()
    {       
        m_plainTextEdit->blockSignals( true );

        m_plainTextEdit->setHtml( Data::DataManager::loadNote( GeneratorID::id() ) );
        m_plainTextEdit->adaptSizeFromText();

        m_plainTextEdit->blockSignals( false ); 
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
        removeNoteFromNepomuk();

        m_tag = new Tag::NoteTag( this, tagName );
        m_tag->setCurrentState( tagState );
        m_horizontalLayout->insertWidget(0,m_tag);
        m_tag->apply();
    }

    void NoteItem::tagApply( QAction * action )
    {
        if ( action->isChecked() )
        {
            removeNoteFromNepomuk();

            m_tag = new Tag::NoteTag( this, action->data().toString() );
            m_horizontalLayout->insertWidget(0,m_tag);
            m_plainTextEdit->selectAll();
            m_tag->apply();
            m_plainTextEdit->selectNone();

            addNoteToNepomuk();
        }
        else
        {
            // supprimer
            m_horizontalLayout->removeWidget(m_tag);
            removeNoteFromNepomuk();
        }
    }

    void NoteItem::tagApply()
    {
        m_plainTextEdit->selectAll();
        m_tag->apply();
        m_plainTextEdit->selectNone();
    }

    void NoteItem::insertData( const QMimeData * data )
    {
        m_plainTextEdit->addData( data );
    }

    void NoteItem::paintEvent( QPaintEvent * event )
    {
        QPainter painter(this);

        QLinearGradient gradient( 0, 0, 0, height() );
        gradient.setColorAt( 0, m_color.lighter(Config::VisualAspect::lighterIntensity) );
        gradient.setColorAt( 1, m_color );

        painter.setPen( Qt::NoPen );
        painter.setBrush( gradient );

        painter.drawRect( 0, 0, width(), height() );
    }

    void NoteItem::removeNoteFromNepomuk()
    {
        if ( m_tag != 0 )
        {
            delete m_tag;
            m_tag = 0;

	    if ( m_handle != 0 )
	    {
		Nepomuk::Resource file( Settings::basketsStorePath().toLocalFile()+QDir::separator()+m_handle->configFile() );
		file.remove();
	    }
        }
    }

    void NoteItem::addNoteToNepomuk()
    {
	if ( m_handle != 0 )
	{
	    Nepomuk::Resource file( Settings::basketsStorePath().toLocalFile()+QDir::separator()+m_handle->configFile() );
	    file.addTag( *m_tag->nepomukTag() );
	}
    }

}
