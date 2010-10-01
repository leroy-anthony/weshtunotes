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

#ifndef ABSTRACTSCENE_H_
#define ABSTRACTSCENE_H_

#include <QGraphicsScene>

#include "../item/AbstractItem.h"
#include "../scene/CustomGraphicsView.h"

template <typename T> class QList;
class QMimeData;

namespace Handle
{
    class HandleItem;
    class GraphicHandleItem;
}

namespace Scene
{

    class AbstractScene : public QGraphicsScene, public Technic::GeneratorID
    {
        Q_OBJECT

    public:
        AbstractScene(QWidget * parent, const QString & type);
        virtual ~AbstractScene();

        virtual Item::AbstractItem * currentAbstractItem() = 0;
        virtual Handle::HandleItem * currentHandle()       = 0;

        virtual void addItemToScene( Handle::GraphicHandleItem * item ) = 0;
        virtual QGraphicsProxyWidget * addHandleToScene( Handle::HandleItem * handle ) = 0;
        virtual Handle::HandleItem * newHandle( int x, int y, int w ) = 0;
        virtual Item::AbstractItem * newItem( int x, int y ) = 0;

        void loadHandles( const QList<QString> & filesName, QPointF centerPt = QPointF(0,0), int selectionWidth = 0, int selectionHeigth = 0, bool newHandles = false );
        void load( const QString & fileName );
        void save();

        void saveViewOnDisk( const QString & fileName );
        void loadViewFromDisk( const QString & fileName );
        void storeView( CustomGraphicsView * view );
        void restoreView( CustomGraphicsView * view );

        virtual void addData( const QMimeData * data ) = 0;

        const QString & type();
        void setType( const QString & type );

        bool readOnly();
        void setReadOnly( bool readOnly );

        void setDirectoryScene( const QString & directory );
        const QString & directoryScene();

        QList<Handle::HandleItem*> handles();

    public slots:
        virtual void delItem( Handle::HandleItem * h ) = 0;
        virtual void moveItem( Handle::HandleItem * handleItem, int x, int y ) = 0;

        void showMessageStatus();

    protected:
        void buildListHandleToLoad( Handle::HandleItem * h, QStringList & l );
        bool canInsertFromMimeData( const QMimeData *source ) const;
        void insertFromMimeData( const QMimeData *source );

        QTransform m_transformView;
        int m_horizontalScrollBarValueView;
        int m_verticalScrollBarValueView;

        QHash<Handle::HandleItem * ,QGraphicsProxyWidget*> m_handles;
        Handle::HandleItem * m_lastCibleHandle;

        QString m_directoryScene;

        QString m_type;

        bool m_readOnly;

    };

}

#endif // ABSTRACTSCENE_H_
