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

#ifndef ABSTRACTBASKET_H_
#define ABSTRACTBASKET_H_

#include <QList>

#include <KUrl>
#include <kpassworddialog.h>
#include <KWallet/Wallet>

#include "../technic/GeneratorID.h"
#include "../synchro/GoogleDocsConnection.h"

namespace Scene
{
    class AbstractScene;
}

namespace Basket
{
    class ItemTreeBasket;

    class AbstractBasket : public QObject, public Technic::GeneratorID
    {
        Q_OBJECT

    public:
        AbstractBasket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basket, const QString & id );
        AbstractBasket( ItemTreeBasket * itemTreeBasket, const QString & id );

        virtual ~AbstractBasket();

        void addChild( AbstractBasket * basket );
        QList<AbstractBasket*> & childrenBasket();

        virtual void save() = 0;
        virtual void load() = 0;
        virtual void del()  = 0;

        const QString & configFile();

        Scene::AbstractScene * scene();
        void setScene( Scene::AbstractScene * scene);

        const QString & type();

        const QString & icon();
        void setIcon( const QString & icon );

        int order();
        void setOrder( int order );

        const QString & name();
        void setName( const QString & name );

        const QString & backgroundColor();
        void setBackgroundColor( const QString & color );


    public slots:
        void commit();
        void update();


    protected:
        void initConfigFile( const QString & id );

        ItemTreeBasket * m_itemTreeBasket;
        Scene::AbstractScene * m_contentScene;
        QList<AbstractBasket*> m_childrenBasket;

        QString m_type;
        QString m_icon;
        int     m_order;
        QString m_name;
        QString m_backgroundColor;

        QString m_configFile;

    };

}

#endif // ABSTRACTBASKET_H_
