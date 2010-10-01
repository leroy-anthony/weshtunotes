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

#include "GeneratorID.h"

#include <QDebug>
#include <cstdlib>

namespace Technic
{

    QMap< QString, QSet<QString> > GeneratorID::m_cacheId;
    int GeneratorID::m_sizeKey = 32;

    GeneratorID::GeneratorID( const QString & name ):
            m_name(name)
    {
        m_nameId = QString(m_name+"%1").arg( newId() );
    }

    QString GeneratorID::newId()
    {
        QString alphabet = "0123456789abcdefghijklmnopqrstuvwxyz_";
        QString id = "";
        int nb = 0;

        while ( ((id == "") || m_cacheId[m_name].contains(id)) && nb < 15 )
        {
            id = "";
            for (int i = 0; i < m_sizeKey; ++i)
            {
                id += alphabet[std::rand() % alphabet.size()];
            }
            ++nb;
        }

        m_cacheId[m_name] += (id);

        return id;
    }

    void GeneratorID::setId( const QString & id )
    {
        m_nameId = id;
        QString idStr = QString(id).replace(m_name,"");
        m_cacheId[m_name] += idStr;
    }

    void GeneratorID::regenerateId()
    {
        m_nameId = QString(m_name+"%1").arg( newId() );
    }

    const QString & GeneratorID::id() const
    {
        return m_nameId;
    }



}
