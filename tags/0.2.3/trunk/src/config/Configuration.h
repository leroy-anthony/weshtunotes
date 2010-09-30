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
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <QDir>

#include <KConfig>

namespace Config
{

    class Configuration : protected KConfig
    {
    public:
        Configuration();
        Configuration( const QString & config );
        ~Configuration();

        void setValue( const QString & group, const QString & key, QVariant value );
        void setValue( const QString & group, const QString & subGroup, const QString & key, QVariant value );

        QString valueSubGroup( const QString & group, const QString & subGroup, const QString & key, QVariant defaultValue ) const;
        QString valueGroup( const QString & group, const QString & key, QVariant defaultValue ) const;
        QStringList values( const QString & group, const QString & key ) const;

        void removeValue( const QString & groupKey, const QString & key );
        void removeSubGroup( const QString & group, const QString & subGroup );
        void removeGroup( const QString & group );

        static void saveLastBasket( const QString & name );
        static QString loadLastBasket();

    };

}

#endif // CONFIGURATION_H_
