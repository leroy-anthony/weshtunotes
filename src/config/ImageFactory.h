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

#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include <QMap>
#include <QPixmap>

#include <KIcon>
#include <KIconLoader>
#include <KPixmapCache>

class KUrl;

namespace Config
{
    /*class Image
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
    };*/

    class ImageFactory
    {
    public:
        static ImageFactory * instance();

        const KIcon & icon( const QString & iconId );
        const QPixmap & pixmap( const QString & pixmapId, QPixmap & pix  );
        void clean();

        const QPixmap & loadMimeTypeIcon( const QString & iconName, QPixmap & pix  );
        QString iconPath( const QString & iconName );
        QString iconNameForUrl( const KUrl & url );

    protected:
        ImageFactory();
        ~ImageFactory();

    private:
        QMap<QString,KIcon> m_cacheIcons;
        KPixmapCache m_cachePixmaps;
        KIconLoader kIconLoader;

        static ImageFactory * m_imageFactory;

    };

}

#endif // IMAGEFACTORY_H
