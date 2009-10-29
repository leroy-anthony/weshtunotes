#include "SceneFactory.h"

#include "FreeScene.h"
#include "LayoutScene.h"

#include <QDebug>

namespace Scene
{

    SceneFactory::SceneFactory()
    {
    }

    AbstractScene * SceneFactory::newScene( const QString &  fileName )
    {
        Config::Configuration settings( fileName );
        settings.beginGroup("scene");

        return newSceneInterne( fileName, settings.value("type",FreeScene::type).toString() );
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

        scene->load( fileName );

        return scene;
    }

}
