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

#ifndef ITEXTEDITION_H
#define ITEXTEDITION_H

#include <Qt>
#include <QColor>
#include <QFont>

#include "../config/Configuration.h"

namespace Item
{

    class ITextEdition
    {
    public:

        ITextEdition();
        virtual ~ITextEdition();

        virtual void setBold( bool checked ) = 0;
        virtual void setItalic( bool italic ) = 0;
        virtual void setAlignment( Qt::Alignment ) = 0;
        virtual void setFontUnderline( bool underLine ) = 0;
        virtual void setTextColor( const QColor & color ) = 0;
        virtual void setFontFamily( const QFont & font ) = 0;
        virtual void setFontPointSize( int weight ) = 0;
        virtual void setFontStrikeOut ( bool strikeOut ) = 0;

        bool bold();
        bool italic();
        int alignment();
        bool underline();
        const QColor & textColor();
        const QString & fontFamily();
        int fontPointSize();
        bool fontStrikeOut();

        void saveTextEdition( Config::Configuration & settings, const QString & group, const QString & subGroup );
        void loadTextEdition( Config::Configuration & settings, const QString & group, const QString & subGroup );

    protected:
        bool m_bold;
        bool m_italic;
        int m_alignment;
        bool m_underLine;
        QColor m_colorFont;
        QString m_font;
        int m_weight;
        bool m_strikeOut;

    };

}

#endif // ITEXTEDITION_H
