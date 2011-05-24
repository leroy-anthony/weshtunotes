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
#include "PlasmaScene.h"
#include "../data/DataManager.h"

namespace Scene
{

    SceneFactory::SceneFactory()
    {
    }

    AbstractScene * SceneFactory::newScene( const QString & fileName )
    {
        Data::DataManager settings( fileName );

        return newSceneInterne( type(settings.valueGroup("scene", "type", typeName(FREESCENE))), "" );
    }

    AbstractScene * SceneFactory::newScene( const QString & fileName, SceneFactory::Type type )
    {
        Data::DataManager settings( fileName );

        return newSceneInterne( type, "" );
    }

    AbstractScene * SceneFactory::newScene( const QString & fileName, SceneFactory::Type type, const QString & id )
    {
        Data::DataManager settings( fileName );

        return newSceneInterne( type, id );
    }

    SceneFactory::Type SceneFactory::type( const QString & typeName )
    {
        if ( typeName == "freescene" )
        {
            return FREESCENE;
        }

        if ( typeName == "layoutscene" )
        {
            return LAYOUTSCENE;
        }

        if ( typeName == "plasmascene" )
        {
            return PLASMASCENE;
        }

        return FREESCENE;
    }

    QString SceneFactory::typeName( SceneFactory::Type type )
    {
        if ( type == FREESCENE )
        {
            return "freescene";
        }

        if ( type == LAYOUTSCENE )
        {
            return "layoutscene";
        }

        if ( type == PLASMASCENE )
        {
            return "plasmascene";
        }

        return "freescene";
    }

    AbstractScene * SceneFactory::newSceneInterne( SceneFactory::Type type, const QString & id )
    {
        AbstractScene * scene = 0;

        if ( type == FREESCENE )
        {
            if ( id != "" )
            {
                scene = new FreeScene(id);
            }
            else
            {
                scene = new FreeScene();
            }
        }

        if ( type == LAYOUTSCENE )
        {
            if ( id != "" )
            {
                scene = new LayoutScene(id);
            }
            else
            {
                scene = new LayoutScene();
            }
        }

        if ( type == PLASMASCENE )
        {
            if ( id != "" )
            {
                scene = new PlasmaScene();
            }
            else
            {
                scene = new PlasmaScene();
            }
        }

        return scene;
    }

}
