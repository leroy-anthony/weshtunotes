#ifndef IVISUALITEMEDITION_H
#define IVISUALITEMEDITION_H

#include <QColor>

namespace Item
{

    class IVisualItemEdition
    {
    public:

        virtual void setItemColor( const QColor & color ) = 0;
        virtual const QColor & itemColor() = 0;

    };

}

#endif // IVISUALITEMEDITION_H
