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
#include <QDesktopServices>
#include <QUrlInfo>

#include "../main/general.h"
#include "../config/Configuration.h"
#include "../scene/ToolBarScene.h"
#include "../config/ImageFactory.h"

namespace Item
{

    CustomTextEdit::CustomTextEdit():
            QTextBrowser()
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setFrameStyle(QFrame::Box | QFrame::Plain);
        setFrameShape(QFrame::NoFrame);
        setFontPointSize(8);
        setContentsMargins(0,0,0,0);
        setFixedHeight(26);
        setAcceptRichText(true);
        setTextInteractionFlags(Qt::TextBrowserInteraction|Qt::TextEditorInteraction);

        setOpenExternalLinks( true );
        setOpenLinks ( false );

        Scene::ToolBarScene * toolBar = Scene::ToolBarScene::toolBarScene();

        connect( this, SIGNAL(textChanged()), this, SLOT(adaptSizeFromText()));
        connect( this, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), toolBar, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
        connect( this, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(openAnchor(const QUrl&)));
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

            QString fileName = QDir::searchPaths( Config::Constant::dirDataKey )[0] + QDir::separator();
            if ( source->urls().size() > 0 )
            {
                fileName += QUrl::toPercentEncoding(source->urls()[0].toString());
            }
            else
            {
                static char str[] = "abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                srand(QTime::currentTime().msec());
                for (int i=0; i<20; i++)
                {
                    int index = makeRand(62);
                    fileName.append(QChar(str[index]));
                }
            }
            fileName += QDateTime::currentDateTime().toString("_yyyy_MM_dd_hh_mm_ss_zzz") + ".jpg";

            image.save( fileName );
            cursor.insertImage( image, fileName );
        }
        else if (source->hasUrls())
        {
            QList<QUrl> urls = source->urls();
            for ( int i=0 ; i<urls.size() ; ++i )
            {
                QFileInfo fileInfo(urls[i].path());
                QTextCursor cursor = this->textCursor();
                cursor.insertHtml("<a href=\""+fileInfo.filePath()+"\"><img src=\"icon:application-msword.png\" />"+fileInfo.fileName()+"</a>");
            }
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

    void CustomTextEdit::openAnchor( const QUrl & url )
    {
        QDesktopServices::openUrl(url);
    }

}
