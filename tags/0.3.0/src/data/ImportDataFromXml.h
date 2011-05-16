/*
*/

#ifndef IMPORTDATAFROMXML_H
#define IMPORTDATAFROMXML_H

#include <QString>
#include <QFile>
#include <QXmlStreamReader>

namespace Explorer
{
    class TreeExplorer;
}

namespace Data
{

    class ImportDataFromXml
    {
    public:
        ImportDataFromXml( Explorer::TreeExplorer * explorer, const QString & file );

        void readRoot();
        void readBaskets();
        void readBasket();
        void readScene();
        void readHandle();
        void readItem();

        void importFromFile();

    private:
        QXmlStreamReader m_xml;
        QFile m_file;
        Explorer::TreeExplorer * m_treeEplorer;

    };

}

#endif // IMPORTDATAFROMXML_H
