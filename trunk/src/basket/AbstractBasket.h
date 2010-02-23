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

#ifndef ABSTRACTBASKET_H_
#define ABSTRACTBASKET_H_

#include <QList>

#include "../technic/GeneratorID.h"

namespace Scene
{
    class AbstractScene;
}

namespace Basket
{

    class AbstractBasket : public Technic::GeneratorID
    {
    public:
        AbstractBasket( AbstractBasket * basket, const QString & name );
        AbstractBasket( const QString & name );

        void addChild( AbstractBasket * basket );
        QList<AbstractBasket*> & childrenBasket();

        virtual void save() = 0;
        virtual void load() = 0;
        virtual void del()  = 0;

        const QString & directory();
        const QString & configFilePath();

        Scene::AbstractScene * scene();
        void setScene( Scene::AbstractScene * scene);

        const QString & type();

        const QString & icon();
        void setIcon( const QString & icon );

        int order();
        void setOrder( int order );

    protected:
        const QString & name();

        QString m_directory;
        QString m_name;
        QString m_configFilePath;

        Scene::AbstractScene * m_contentScene;

        QList<AbstractBasket*> m_childrenBasket;

        QString m_type;
        QString m_icon;
        int     m_order;

    };

}

#endif // ABSTRACTBASKET_H_
