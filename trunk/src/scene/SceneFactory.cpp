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

#include "SceneFactory.h"

#include <QDebug>

#include "FreeScene.h"
#include "LayoutScene.h"

namespace Scene
{

    SceneFactory::SceneFactory()
    {
    }

    AbstractScene * SceneFactory::newScene( const QString &  fileName )
    {
        Config::Configuration settings( fileName );
        //settings.beginGroup("scene");

        return newSceneInterne( fileName, settings.valueGroup("scene","type",FreeScene::type) );
    }

    AbstractScene * SceneFactory::newScene( const QString &  fileName, const QString & type )
    {
        Config::Configuration settings( fileName );

        return newSceneInterne( fileName, type );
    }

    AbstractScene * SceneFactory::newSceneInterne( const QString &  fileName, const QString & type )
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
