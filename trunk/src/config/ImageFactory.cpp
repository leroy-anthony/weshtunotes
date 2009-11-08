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

#include "ImageFactory.h"

#include "../main/general.h"

#include "kmimetype.h"

namespace Config
{

    QString Image::textBold          = "format-text-bold.png";
    QString Image::textItalic        = "format-text-italic.png";
    QString Image::textStrikeOut     = "format-text-strikethrough.png";
    QString Image::textUnderline     = "format-text-underline.png";
    QString Image::textJustifyLeft   = "format-justify-left.png";
    QString Image::textJustifyCenter = "format-justify-center.png";
    QString Image::textJustifyRight  = "format-justify-right.png";
    QString Image::newBasket         = "document-new.png";
    QString Image::addBasket         = "list-add.png";
    QString Image::deleteAction      = "edit-delete.png";
    QString Image::application       = "basket.png";
    QString Image::exitAction        = "application-exit.png";
    QString Image::basket            = "im-status-message-edit.png";
    QString Image::zoomOriginal      = "zoom-original.png";
    QString Image::zoomIn            = "zoom-in.png";
    QString Image::zoomOut           = "zoom-out.png";
    QString Image::zoomFitBest       = "zoom-fit-best.png";
    QString Image::listScene         = "format-list-unordered.png";

    QMap<QString,KIcon> ImageFactory::m_cacheIcons;
    KPixmapCache ImageFactory::m_cachePixmaps("myapp-pixmaps");
    KIconLoader ImageFactory::kIconLoader;

    ImageFactory::ImageFactory()
    {
    }

    ImageFactory::~ImageFactory()
    {
    }

    void ImageFactory::clean()
    {
        m_cacheIcons.clear();
        m_cachePixmaps.discard();
    }

    const KIcon & ImageFactory::icon( const QString & iconId )
    {
        if ( !m_cacheIcons.contains( iconId ) )
        {
            m_cacheIcons[ iconId ] = KIcon(iconId);
        }

        return m_cacheIcons[ iconId ];
    }

    const QPixmap & ImageFactory::pixmap( const QString & pixmapId, QPixmap & pix )
    {
        if ( !m_cachePixmaps.find( pixmapId, pix ) )
        {
            pix = QPixmap("icon:"+pixmapId);
            m_cachePixmaps.insert( pixmapId, pix );
        }

        return pix;
    }

    const QPixmap & ImageFactory::loadMimeTypeIcon( const QString & iconName, QPixmap & pix )
    {
        if ( !m_cachePixmaps.find( iconName, pix ) )
        {
            pix = kIconLoader.loadMimeTypeIcon( iconName, KIconLoader::Desktop );
            m_cachePixmaps.insert( iconName, pix );
        }

        return pix;
    }

    QString ImageFactory::iconPath( const QString & iconName )
    {
        return kIconLoader.iconPath( iconName, KIconLoader::Desktop );
    }

    QString ImageFactory::iconNameForUrl( const KUrl & url )
    {
        return KMimeType::iconNameForUrl( url );
    }

}
