#include "VisualAspect.h"

namespace Config
{

    VisualAspect::VisualAspect()
    {
    }

    QString VisualAspect::gradiantBackground( const QColor & color )
    {
        return QString("border: none; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2)")
                .arg(color.lighter(lighterIntensity).name())
                .arg(color.name());
    }

    int VisualAspect::lighterIntensity = 150;
    int VisualAspect::widthHandleControl = 9;

    QColor VisualAspect::defaultColorNote = QColor("#F7F7C8");

}
