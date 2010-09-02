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


#include "CustomTextEdit.h"

#include <QUrl>
#include <QDir>
#include <QDesktopServices>
#include <QFileIconProvider>
#include <QFocusEvent>
#include <QPainter>
#include <QTime>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QTextLayout>

#include <KUrl>

#include "../main/general.h"
#include "../config/Configuration.h"
#include "../scene/ToolBarScene.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"
#include "../item/NoteItem.h"

namespace Item
{

    CustomTextEdit::CustomTextEdit( NoteItem * noteItem ):
            KTextBrowser(),
            m_noteItem(noteItem)
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

        setFocusPolicy( Qt::StrongFocus );

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

            QString baseDir = Data::DataManager::datasStorePath();
            QString fileName = "";
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

            image.save( baseDir+fileName );
            cursor.insertImage( image, baseDir+fileName );
        }
        else if (source->hasUrls())
        {
            QTextCursor cursor = this->textCursor();
            QList<QUrl> urls = source->urls();
            for ( int i=0 ; i<urls.size() ; ++i )
            {
                if ( urls[i].scheme() == "file" )
                {
                    QFileInfo fileInfo(urls[i].path());

                    QPixmap pix;
                    QString iconName = Config::ImageFactory::newInstance()->iconNameForUrl( urls[i] );
                    Config::ImageFactory::newInstance()->loadMimeTypeIcon( iconName, pix );
                    QString path = Config::ImageFactory::newInstance()->iconPath( iconName );

                    //référence l'image
                    document()->addResource(QTextDocument::ImageResource, QUrl(path), pix);
                    QTextImageFormat imageFormat;
                    //spécifie l'image référencée
                    imageFormat.setName(path);
                    imageFormat.setAnchor(true);
                    imageFormat.setAnchorHref(urls[i].toString());
                    //ajoute l'image référencée
                    cursor.insertImage(imageFormat);

                    cursor.insertHtml("<a href=\""+urls[i].toString()+"\">"+fileInfo.fileName()+"</a>");
                }
                else
                {
                    cursor.insertHtml("<a href=\""+urls[i].toString()+"\">"+urls[i].toString()+"</a>");

                }
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
        setFixedHeight( heightMax-2 );
    }

    void CustomTextEdit::openAnchor( const QUrl & url )
    {
        QDesktopServices::openUrl(url);
    }

    void CustomTextEdit::selectNone()
    {
        QTextCursor c( textCursor() );
        c.clearSelection();
        setTextCursor( c );
    }

    void CustomTextEdit::contextMenuEvent ( QContextMenuEvent * event )
    {
    }

}
