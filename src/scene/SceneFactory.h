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

#ifndef SCENEFACTORY_H_
#define SCENEFACTORY_H_

#include "AbstractScene.h"

namespace Scene
{

    class SceneFactory
    {
    public:
        enum Type { FREESCENE, LAYOUTSCENE, PLASMASCENE, MAX };

        SceneFactory();

        static AbstractScene * newScene( const QString & fileName );
        static AbstractScene * newScene( const QString & fileName, SceneFactory::Type type );
        static AbstractScene * newScene( const QString & fileName, SceneFactory::Type type, const QString & id );

        static QString typeName( SceneFactory::Type type );
        static SceneFactory::Type type( const QString & typeName );

    private:
        static AbstractScene * newSceneInterne( SceneFactory::Type type, const QString & id );

    };

}

#endif // SCENEFACTORY_H_
