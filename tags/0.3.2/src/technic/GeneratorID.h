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

#ifndef GENERATORID_H
#define GENERATORID_H

#include <QString>
#include <QMap>
#include <QSet>

namespace Technic
{
    class GeneratorID
    {
    public:
        GeneratorID( const QString & name, bool withGeneratedId = true );

        const QString & id() const;
        void setId( const QString & id );
        void regenerateId();

    private:
        QString newId();

        QString m_name;
        QString m_nameId;

    private:
        static QMap< QString, QSet<QString> > m_cacheId;
        static int m_sizeKey;

    };
}

#endif // GENERATORID_H
