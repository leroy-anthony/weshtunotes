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

#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include <QMap>
#include <QPixmap>

#include <kurl.h>
#include <KIcon>
#include <kiconloader.h>
#include <kiconcache.h>
#include <kpixmapcache.h>

namespace Config
{
    class Image
    {
    public:
        static QString textBold;
        static QString textItalic;
        static QString textStrikeOut;
        static QString textUnderline;
        static QString textJustifyLeft;
        static QString textJustifyCenter;
        static QString textJustifyRight;
        static QString newBasket;
        static QString addBasket;
        static QString deleteAction;
        static QString application;
        static QString exitAction;
        static QString basket;
        static QString zoomOriginal;
        static QString zoomIn;
        static QString zoomOut;
        static QString zoomFitBest;
        static QString listScene;
    };

    class ImageFactory
    {

    public:
        static const KIcon & icon( const QString & iconId );
        static const QPixmap & pixmap( const QString & pixmapId, QPixmap & pix  );
        static void clean();

        static const QPixmap & loadMimeTypeIcon( const QString & iconName, QPixmap & pix  );
        static QString iconPath( const QString & iconName );
        static QString iconNameForUrl( const KUrl & url );

    protected:
        ImageFactory();
        ~ImageFactory();

    private:
        static QMap<QString,KIcon> m_cacheIcons;
        static KPixmapCache m_cachePixmaps;
        static KIconLoader kIconLoader;

    };

}

#endif // IMAGEFACTORY_H
