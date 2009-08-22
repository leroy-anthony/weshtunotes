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


#include "CustomTextEdit.h"

#include <QUrl>
#include <QDir>
#include <QUrlInfo>

#include "../main/general.h"
#include "../config/Configuration.h"
#include "../scene/ToolBarScene.h"

namespace Item
{

    CustomTextEdit::CustomTextEdit():
            QTextEdit()
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setFrameStyle(QFrame::Box | QFrame::Plain);
        setFrameShape(QFrame::NoFrame);
        setFontPointSize(8);
        setContentsMargins(0,0,0,0);
        setFixedHeight(26);
        setAcceptRichText(true);

        Scene::ToolBarScene * toolBar = Scene::ToolBarScene::toolBarScene();

        connect( this, SIGNAL(textChanged()),  this, SLOT(adaptSizeFromText()));
        connect( this, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), toolBar, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    }

    bool CustomTextEdit::canInsertFromMimeData( const QMimeData *source ) const
    {
        if (source->hasImage())
        {
            return true;
        }
        else
        {
            return QTextEdit::canInsertFromMimeData(source);
        }
    }

    void CustomTextEdit::addData( const QMimeData *source )
    {
        CustomTextEdit::insertFromMimeData( source );
    }

    void CustomTextEdit::insertFromMimeData( const QMimeData *source )
    {
        if (source->hasImage())
        {
            QImage image = qvariant_cast<QImage>(source->imageData());
            QTextCursor cursor = this->textCursor();
            QUrl url = source->urls()[0];
            QString fileName = QDir::searchPaths( Config::Constant::dirDataKey )[0] + QDir::separator() + QUrl::toPercentEncoding(url.toString()) + ".jpg";
            image.save( fileName );
            cursor.insertImage( image, fileName );
        }
        else
        {
            QTextEdit::insertFromMimeData( source );
        }
    }

    void CustomTextEdit::adaptSizeFromText()
    {
        int heightMax =  document()->size().toSize().height();
        setFixedHeight( heightMax+2 );
    }

}
