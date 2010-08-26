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

#include "Configuration.h"

#include <QFileInfo>
#include <QCoreApplication>

#include <Nepomuk/Resource>

#include "settings.h"
#include "../main/general.h"

namespace Config
{

    Configuration::Configuration():
            KConfig( "kweshtunotesrc" )
    {
    }

    Configuration::Configuration( const QString & config ):
            KConfig( Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets"+QDir::separator()+config )
    {
    }

    Configuration::~Configuration()
    {
    }

    void Configuration::saveLastBasket( const QString & name )
    {
        Configuration settings;
        settings.setValue( "config", "lastBasket", name );
    }

    QString Configuration::loadLastBasket()
    {
        Configuration settings;
        return settings.valueGroup( "config", "lastBasket", "" );
    }

    void Configuration::setValue( const QString & groupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        groupConfig.writeEntry( key, value );
        groupConfig.sync();
    }

    void Configuration::setValue( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        KConfigGroup subGroup = groupConfig.group( subGroupKey );
        subGroup.writeEntry( key, value );
        subGroup.sync();
    }

    QString Configuration::valueGroup( const QString & groupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.readEntry( key, defaultValue ).toString();
    }

    void Configuration::removeValue( const QString & groupKey, const QString & key )
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.deleteEntry( key );
    }

    QString Configuration::valueSubGroup( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.group(subGroupKey).readEntry( key, defaultValue ).toString();
    }

    QStringList Configuration::values( const QString & groupKey, const QString & key ) const
    {
        KConfigGroup configGroup = group( groupKey );
        return configGroup.readEntry( key, QStringList() );
    }

    void Configuration::removeSubGroup( const QString & group, const QString & subGroup )
    {
        KConfigGroup configGroup( this, group );
        configGroup.group( subGroup ).deleteGroup();
    }

    void Configuration::removeGroup( const QString & group )
    {
        KConfigGroup configGroup( this, group );
        configGroup.deleteGroup();
    }

}
