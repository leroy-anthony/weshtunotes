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

#include "ColorCombo.h"

#include <QStylePainter>
#include <QAbstractItemView>
#include <QPalette>

ColorCombo::ColorCombo()
{
    QStringList colorNames = QColor::colorNames();
    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);
        insertItem(i, "");
        QPixmap pixmap(48,24);
        pixmap.fill(color);
        setItemData(i, pixmap, Qt::DecorationRole);
    }

    setFixedWidth(64);
}

void ColorCombo::paintEvent ( QPaintEvent * event )
{
    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    //painter.setPen(currentIndex());
    //QPixmap pixmap(48,24);
   // pixmap.fill(currentText());
   // painter.drawPixmap(4,4,44,20,pixmap);
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
