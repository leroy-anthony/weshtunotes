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

#include "../item/ITextEdition.h"
#include "../item/NoteItem.h"
#include "../config/Configuration.h"

namespace Item
{
    class NoteItem;
}

namespace Tag
{

    class State : public Item::ITextEdition, Item::IVisualItemEdition
    {
    public:
        State( NoteTag * tag );

        void setItem( Item::NoteItem * item );
        const QString & symbol();
        const QString & name();
        void setName( const QString & name );

        void setBold( bool checked );
        bool bold();

        void setItalic( bool italic );
        bool italic();

        void setFontStrikeOut ( bool strikeOut );
        bool fontStrikeOut();

        void setAlignment( Qt::Alignment );
        void setFontUnderline( bool underLine );

        void setTextColor( const QColor & color );
        const QColor & textColor();

        void setFontFamily( const QFont & font );
        const QString & fontFamily();

        void setFontPointSize( int weight );
        int fontPointSize();

        void setItemColor( const QColor & color );
        const QColor & itemColor(); // todo: remove thss !!

        void load( Config::Configuration & settings );
        void load();
        void save();

        void apply();

        static State * newState( NoteTag * tag );

    private:
        NoteTag * m_tag;
        Item::NoteItem * m_item;

        QString m_name;
        QString m_symbol;
        bool m_bold;
        bool m_italic;
        int m_alignment;
        bool m_underLine;
        bool m_strikeOut;
        QColor m_colorFont;
        QColor m_colorItem;
        QString m_font;
        int m_weight;
    };

}

#endif // STATE_H
