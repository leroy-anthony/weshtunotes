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

#ifndef STATE_H
#define STATE_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>

#include "../interface/ITextEdition.h"
#include "../item/NoteItem.h"
#include "../config/Configuration.h"

namespace Item
{
    class NoteItem;
}

namespace Tag
{

    class State : public Item::ITextEdition, public Item::IVisualItemEdition
    {
    public:
        State( NoteTag * tag );
        ~State();

        void setItem( Item::NoteItem * item );

        const QString & name();
        void setName( const QString & name );

        void setBold( bool checked );
        void setItalic( bool italic );
        void setFontStrikeOut ( bool strikeOut );

        void setAlignment( Qt::Alignment );
        Qt::Alignment alignment();

        void setFontUnderline( bool underLine );
        void setTextColor( const QColor & color );
        void setFontFamily( const QFont & font );
        void setFontPointSize( int weight );

        void setItemColor( const QColor & color );
        void setSymbol( const QString & icon );

        void load( Config::Configuration & settings, const QString & tagName, const QString & state );
        void load();
        void save();

        void apply();

        static State * newState( NoteTag * tag );

    private:
        NoteTag * m_tag;
        Item::NoteItem * m_item;

        QString m_name;
    };

}

#endif // STATE_H
