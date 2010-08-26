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

#include "ImageFactory.h"

#include <KMimeType>
#include <KUrl>

#include "../main/general.h"

namespace Config
{
    ImageFactory * ImageFactory::m_imageFactory = 0;

    ImageFactory *  ImageFactory::newInstance()
    {
        if ( m_imageFactory == 0 )
        {
            m_imageFactory = new ImageFactory();
        }

        return m_imageFactory;
    }

    ImageFactory::ImageFactory():
        m_cachePixmaps("myapp-pixmaps")
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
