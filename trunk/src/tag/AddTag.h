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

#ifndef ADDTAG_H
#define ADDTAG_H

#include <QPushButton>
#include <QMouseEvent>

#include <kmenu.h>

#include "../config/ImageFactory.h"

namespace Item
{
    class NoteItem;
}

namespace Tag
{

    class AddTag : public QPushButton
    {
        Q_OBJECT

    public:
        AddTag( Item::NoteItem * noteItem );

    protected:
        void mousePressEvent( QMouseEvent * e );
        void loadTagsMenu();

    private:
        KMenu m_menu;
        Item::NoteItem * m_noteItem;

    };

}

#endif // ADDTAG_H
