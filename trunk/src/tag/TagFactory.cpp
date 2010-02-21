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

#include "TagFactory.h"

#include <QInputDialog>

#include <kmessagebox.h>

#include "../main/general.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"
#include "../tag/NoteTag.h"

namespace Tag
{

    TagFactory * TagFactory::m_instance = 0;

    TagFactory * TagFactory::newTagFactory( QWidget * parent )
    {
        if ( m_instance == 0 )
        {
            m_instance = new TagFactory( parent );
        }

        return m_instance;
    }

    TagFactory::TagFactory( QWidget * parent ):
            KDialog(parent, Qt::Dialog),
            m_currentState(0),
            m_currentTag(0),
            m_currentItemState(0),
            m_currentItemTag(0)
    {
        setCaption( "Create Tag" );
        setModal(true);

        QWidget * main = new QWidget( this );
        setupUi( main );
        setMainWidget( main );

        m_boldText->setIcon(Config::ImageFactory::newInstance()->icon("format-text-bold.png"));
        m_italicText->setIcon(Config::ImageFactory::newInstance()->icon("format-text-italic.png"));
        m_underlineText->setIcon(Config::ImageFactory::newInstance()->icon("format-text-underline.png"));
        m_strikeText->setIcon(Config::ImageFactory::newInstance()->icon("format-text-strikethrough.png"));
        m_delTagButton->setIcon(Config::ImageFactory::newInstance()->icon("edit-delete.png"));

        connect(m_delTagButton, SIGNAL(clicked()), this, SLOT(del()));
        connect(m_newTagButton, SIGNAL(clicked()), this, SLOT(newTag()));
        connect(m_newStateButton, SIGNAL(clicked()), this, SLOT(newState()));
        connect(m_tagsTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(loadTagOrState(QTreeWidgetItem*, int)));

        connect(m_nameTagOrState, SIGNAL(editingFinished()), this, SLOT(changeNameTagOrState()));

        connect(m_withColorBackground, SIGNAL(stateChanged(int)), this, SLOT(withColorBackground(int)));
        connect(m_colorItem, SIGNAL(activated(const QColor &)), SLOT(changeItemColor(const QColor &)));

        connect(m_boldText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_italicText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_underlineText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_strikeText, SIGNAL(released()), this, SLOT(changeState()));

        connect(m_fontText, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(changeFontState(const QFont&)));
        connect(m_colorText, SIGNAL(activated(const QColor &)), SLOT(changeTextColor(const QColor &)));
        connect(m_sizeText, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSizeFontState(int)));

        connect(m_withIcon, SIGNAL(stateChanged(int)), this, SLOT(withIcon(int)));

        connect( this, SIGNAL( okClicked() ), this, SLOT( ok() ) );

        connect(m_iconButton, SIGNAL(iconChanged(const QString&)), this, SLOT(selectIcon(const QString&)));
    }

    void TagFactory::del()
    {
        QTreeWidgetItem * item = m_tagsTree->currentItem();

        if ( m_itemToState.contains( item ) )
        {
            State * state = m_itemToState[ item ];
            NoteTag * tag = m_itemToTag[ item->parent() ];
            tag->removeState( state );

            m_itemToState.remove( item );
        }
        else if ( m_itemToTag.contains( item ) )
        {
            int childSize = item->childCount();
            for ( int i=0 ; i<childSize ; ++i )
            {
                m_itemToState.remove( item->child(i) );
            }

            m_itemToTag.remove( item );
        }

        delete item;
    }

    void TagFactory::selectIcon( const QString & icon )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setSymbol( icon );
        }
    }

    void TagFactory::loadTags()
    {
        Config::Configuration settings( "tags" );

        QStringList tags = settings.values( "General", "tags" );
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            NoteTag * tag = new NoteTag( 0, tags[i] );

            QTreeWidgetItem * itemTag = new QTreeWidgetItem( m_tagsTree, QStringList(tags[i]) );
            m_itemToTag[ itemTag ] = tag;

            const QList<State*> & states = tag->states();
            for ( int j=0 ; j<states.size() ; ++j )
            {
                State * state = states[j];

                QTreeWidgetItem * itemState = new QTreeWidgetItem( itemTag, QStringList(state->name()) );
                m_itemToState[ itemState ] = state;
            }
        }
    }

    void TagFactory::newTag()
    {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Tag name"),
                                             tr("Tag name:"), QLineEdit::Normal,
                                             "New tag", &ok);

        QList<QTreeWidgetItem*> items = m_tagsTree->findItems( name, Qt::MatchCaseSensitive | Qt::MatchRecursive );

        if ( ok && items.isEmpty() )
        {
            Tag::NoteTag * noteTag = NoteTag::newTag( name );

            QTreeWidgetItem * itemTag = new QTreeWidgetItem( m_tagsTree, QStringList(noteTag->name()) );
            m_itemToTag[ itemTag ] = noteTag;

            const QList<State*> & states = noteTag->states();
            for ( int i=0 ; i<states.size() ; ++i )
            {
                QTreeWidgetItem * itemState = new QTreeWidgetItem( itemTag, QStringList(states[i]->name()) );
                m_itemToState[ itemState ] = states[i];
            }
        }
    }

    void TagFactory::newState()
    {
        if ( m_currentItemTag != 0 && m_currentTag != 0 )
        {
            State * state = State::newState( m_currentTag );
            m_currentTag->addState( state );

            QTreeWidgetItem * itemState = new QTreeWidgetItem( m_currentItemTag, QStringList(state->name()) );
            m_itemToState[ itemState ] = state;
        }
    }

    void TagFactory::changeNameTagOrState()
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setName(m_nameTagOrState->text());
            m_currentItemState->setText( 0, m_nameTagOrState->text() );
        }
        else if ( m_currentTag != 0 )
        {
            m_currentTag->setName(m_nameTagOrState->text());
            m_currentItemTag->setText( 0, m_nameTagOrState->text() );
        }
    }

    void TagFactory::changeSizeFontState( int index )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setFontPointSize( m_sizeText->itemText( index ).toInt() );
        }
    }

    void TagFactory::changeTextColor( const QColor & color )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setTextColor( color );
        }
    }

    void TagFactory::withColorBackground( int state )
    {
        if ( state == 0 )
        {
            m_colorItem->setDisabled(true);
            m_colorItem->setColor(QColor());
        }
        else
        {
            m_colorItem->setDisabled(false);
        }
    }

    void TagFactory::withIcon( int state )
    {
        if ( state == 0 )
        {
            m_iconButton->setDisabled(true);
            m_iconButton->setIcon(0);
        }
        else
        {
            m_iconButton->setDisabled(false);
        }
    }

    void TagFactory::changeItemColor( const QColor & color )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setItemColor( color );
        }
    }

    void TagFactory::changeFontState( const QFont & font )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setFontFamily( font );
        }
    }

    void TagFactory::changeState()
    {
        if ( m_currentState != 0 )
        {
            m_currentState->setBold( m_boldText->isChecked() );
            m_currentState->setItalic( m_italicText->isChecked() );
            m_currentState->setFontUnderline( m_underlineText->isChecked() );
            m_currentState->setFontStrikeOut( m_strikeText->isChecked() );
        }
    }

    void TagFactory::loadTagOrState( QTreeWidgetItem * item , int column )
    {
        Q_UNUSED( column );

        if ( m_itemToState.contains( item ) )
        {
            m_currentItemState = item;
            loadState( m_itemToState[item] );

            m_appareanceGroupBox->setEnabled( true );
            m_fontGroupBox->setEnabled( true );
        }
        else if ( m_itemToTag.contains( item ) )
        {
            m_currentItemTag = item;
            loadTag( m_itemToTag[item] );

            m_appareanceGroupBox->setEnabled( false );
            m_fontGroupBox->setEnabled( false );
        }
    }

    void TagFactory::loadTag( NoteTag * noteTag )
    {
        m_currentState = 0;
        m_currentTag = noteTag;
        m_nameTagOrState->setText( noteTag->name() );
    }

    void TagFactory::loadState( State * state )
    {
        m_currentState = state;
        m_nameTagOrState->setText( state->name() );
        m_colorItem->setColor( QColor(state->itemColor()) );
        m_boldText->setChecked( state->bold() );
        m_italicText->setChecked( state->italic() );
        m_fontText->setCurrentFont( state->fontFamily() );
        m_colorText->setColor( QColor(state->textColor()) );
        m_sizeText->setCurrentIndex( m_sizeText->findText( QString("%1").arg(state->fontPointSize()) ) );
        m_strikeText->setChecked( state->fontStrikeOut() );
        m_underlineText->setChecked( state->underline() );
        m_iconButton->setIcon( Config::ImageFactory::newInstance()->icon(state->symbol()) );
        m_withIcon->setChecked( state->symbol() != QString() );
        m_withColorBackground->setChecked( state->itemColor() != QColor() );

        m_withIcon->setDisabled(false);
    }

    void TagFactory::ok()
    {
        Config::Configuration settings( "tags" );
        Config::Configuration::clear( settings.fileName() );

        QStringList tagsName;

        QList<NoteTag*> tags = m_itemToTag.values();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            tags[i]->save();
            tagsName << tags[i]->name();
        }

        settings.setValue( "General", "tags", tagsName );

        QList<State*> states = m_itemToState.values();
        for ( int i=0 ; i<states.size() ; ++i )
        {
            states[i]->save();
        }

        accept();
    }

    void TagFactory::tagApply( QAction * action )
    {
    }

    QStringList TagFactory::tagsNames()
    {
        Config::Configuration settings( "tags" );
        return settings.values( "General", "tags" );
    }
}
