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

#include "SceneFactory.h"

#include <QDebug>

#include "FreeScene.h"
#include "LayoutScene.h"
#include "../data/DataManager.h"

namespace Scene
{

    SceneFactory::SceneFactory()
    {
    }

    AbstractScene * SceneFactory::newScene( const QString & fileName )
    {
        Data::DataManager settings( fileName );

        return newSceneInterne( settings.valueGroup("scene","type",FreeScene::type) );
    }

    AbstractScene * SceneFactory::newScene( const QString & fileName, const QString & type )
    {
        Data::DataManager settings( fileName );

        return newSceneInterne( type );
    }

    AbstractScene * SceneFactory::newSceneInterne( const QString & type )
    {
        AbstractScene * scene = 0;

        if ( type == FreeScene::type )
        {
            scene = new FreeScene();
        }

        if ( type == LayoutScene::type )
        {
            scene = new LayoutScene();
        }

        return scene;
    }

}
