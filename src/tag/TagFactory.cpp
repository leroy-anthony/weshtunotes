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

#include "../main/general.h"
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
            QDialog(parent, Qt::Dialog),
            m_currentState(0),
            m_currentTag(0),
            m_currentItemState(0),
            m_currentItemTag(0)
    {
        setupUi( this );

        m_colorText->setStandardColors();
        m_colorItem->setStandardColors();

        m_boldText->setIcon(QIcon("icon:format-text-bold.png"));
        m_italicText->setIcon(QIcon("icon:format-text-italic.png"));
        m_underlineText->setIcon(QIcon("icon:format-text-underline.png"));
        m_strikeText->setIcon(QIcon("icon:format-text-strikethrough.png"));

        connect(m_newTagButton, SIGNAL(clicked()), this, SLOT(newTag()));
        connect(m_newStateButton, SIGNAL(clicked()), this, SLOT(newState()));
        connect(m_tagsTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(loadTagOrState(QTreeWidgetItem*, int)));

        connect(m_nameTagOrState, SIGNAL(editingFinished()), this, SLOT(changeNameTagOrState()));

        connect(m_withColorBackground, SIGNAL(stateChanged(int)), this, SLOT(withColorBackground(int)));
        connect(m_colorItem, SIGNAL(colorChanged(const QColor &)), SLOT(changeItemColor(const QColor &)));

        connect(m_boldText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_italicText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_underlineText, SIGNAL(released()), this, SLOT(changeState()));
        connect(m_strikeText, SIGNAL(released()), this, SLOT(changeState()));

        connect(m_fontText, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(changeFontState(const QFont&)));
        connect(m_colorText, SIGNAL(colorChanged(const QColor &)), SLOT(changeTextColor(const QColor &)));
        connect(m_sizeText, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSizeFontState(int)));

        connect(m_okButton, SIGNAL(clicked()), this, SLOT(ok()));
        connect(m_quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    }

    void TagFactory::loadTags()
    {
        Config::Configuration settings( "tags" );

        //m_menu = new QMenu();


        QStringList tags = settings.childGroups();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            NoteTag * tag = new NoteTag( 0, tags[i] );

            QTreeWidgetItem * itemTag = new QTreeWidgetItem( m_tagsTree, QStringList(tags[i]) );
            m_itemToTag[ itemTag ] = tag;

            settings.beginGroup( tags[i] );
            QStringList states = settings.childGroups();
            for ( int j=0 ; j<states.size() ; ++j )
            {
                State * state = new State(tag);
                state->setName( states[j] );
                tag->addState( state );

                QTreeWidgetItem * itemState = new QTreeWidgetItem( itemTag, QStringList(states[j]) );
                m_itemToState[ itemState ] = state;
            }
            settings.endGroup();

        }

    }

    void TagFactory::newTag()
    {
        Tag::NoteTag * noteTag = NoteTag::newTag();

        QTreeWidgetItem * itemTag = new QTreeWidgetItem( m_tagsTree, QStringList(noteTag->name()) );
        m_itemToTag[ itemTag ] = noteTag;

        QList<State*> states = noteTag->states();
        for ( int i=0 ; i<states.size() ; ++i )
        {
            QTreeWidgetItem * itemState = new QTreeWidgetItem( itemTag, QStringList(states[i]->name()) );
            m_itemToState[ itemState ] = states[i];
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
        }
        else
        {
            m_colorItem->setDisabled(false);
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
            //m_currentState->setStrike( m_boldText->down() );
        }
    }

    void TagFactory::loadTagOrState( QTreeWidgetItem * item , int column )
    {
        if ( m_currentState != 0 )
        {
            m_currentState->save();
        }
        else if ( m_currentTag != 0 )
        {
            m_currentTag->save();
        }

        if ( m_itemToState.contains( item ) )
        {
            m_currentItemState = item;
            loadState( m_itemToState[item] );
        }
        else if ( m_itemToTag.contains( item ) )
        {
            m_currentItemTag = item;
            loadTag( m_itemToTag[item] );
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
        state->load();

        m_currentState = state;
        m_nameTagOrState->setText( state->name() );
        m_colorItem->setCurrentColor( state->itemColor() );
        m_boldText->setChecked( state->bold() );
        m_italicText->setChecked( state->italic() );
        m_fontText->setCurrentFont( state->fontFamily() );
        m_colorText->setCurrentColor( state->textColor() );
        m_sizeText->setCurrentIndex( m_sizeText->findText( QString("%1").arg(state->fontPointSize()) ) );
    }

    void TagFactory::ok()
    {
        QList<NoteTag*> tags = m_itemToTag.values();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            tags[i]->save();
        }

        QList<State*> states = m_itemToState.values();
        for ( int i=0 ; i<states.size() ; ++i )
        {
            states[i]->save();
        }

        accept();
    }

    void TagFactory::quit()
    {
        reject();
    }

    //QMenu * TagFactory::m_menu = 0;

    void TagFactory::showMenuTag()
    {
        //m_menu->popup(QCursor::pos());
        //menu.show();
    }

    void TagFactory::tagApply( QAction * action )
    {
        qDebug() << action->text();
    }

    QStringList TagFactory::tagsNames()
    {
        Config::Configuration settings( "tags" );
        QStringList tags = settings.childGroups();

        return tags;
    }
}
