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

#ifndef TOOLBARSCENE_H
#define TOOLBARSCENE_H

#include "../main/general.h"

#include <QObject>

class QTextCharFormat;
class QColor;
class QFont;

class KFontComboBox;
class KColorCombo;
class KFontAction;
class KFontSizeAction;
class KAction;
class KIcon;

class MainWindow;

namespace Item
{
    class AbstractItem;
}

namespace Scene
{

    class ToolBarScene : public QObject
    {
        Q_OBJECT

    public:
        ToolBarScene( MainWindow * mainWindow );

        static ToolBarScene * toolBarScene();

    public slots:
        void setBold( bool checked );
        void setItalic( bool checked );
        void setFontStrikeOut( bool strikeOut );
        void setAlignmentRight( bool checked );
        void setAlignmentLeft( bool checked );
        void setAlignmentCenter( bool checked );
        void setFontUnderline( bool checked );
        void setTextBackgroundColor( const QColor & c );
        void setTextColor( const QColor & c );
        void setFontFamily( const QFont & font );
        void setFontPointSize( int size );
        void currentCharFormatChanged( const QTextCharFormat & f );
        void currentItemChanged( Item::AbstractItem * item );

        void setColorItem( const QColor & c );

    private:
        KAction * addAction( QString name );
        KAction * addAction( const KIcon & icon, QString name );

        Item::AbstractItem * currentAbstractItem();

        MainWindow * m_mainWindow;

        KColorCombo     * m_colorText;
        KColorCombo     * m_colorBackgroundText;
        KFontSizeAction * m_actionFontSize;
        KFontComboBox * m_fontComboBox;
        KFontAction   * m_actionFont;

        KColorCombo * m_colorItem;

        static ToolBarScene * m_toolBar;

        KAction * m_strikeoutAction;
        KAction * m_italicAction;
        KAction * m_boldAction;
        KAction * m_underlineAction;
        KAction * m_alignleftAction;
        KAction * m_aligncenterAction;
        KAction * m_alignrightAction;

    };

}

#endif // TOOLBARSCENE_H
