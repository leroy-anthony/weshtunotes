/*
*/

#include "ImportDataFromXml.h"

#include <QFile>
#include <QXmlStreamReader>
#include <KDebug>

#include "../explorer/TreeExplorer.h"
#include "../basket/Basket.h"

namespace Data
{

    ImportDataFromXml::ImportDataFromXml( Explorer::TreeExplorer * explorer, const QString & filePath ):
            m_file(filePath),
            m_treeEplorer(explorer)
    {
        if (m_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_xml.setDevice(&m_file);
        }
    }

    void ImportDataFromXml::importFromFile()
    {
        while (m_xml.readNextStartElement())
        {
            kDebug() << m_xml.name();

            if (m_xml.name() == "kweshtunotes")
            {
                readRoot();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readRoot()
    {
        kDebug() << m_xml.name();
        while (m_xml.readNextStartElement())
        {
            if (m_xml.name() == "baskets")
            {
                readBaskets();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readBaskets()
    {
        kDebug() << m_xml.name();
        while (m_xml.readNextStartElement())
        {
            if (m_xml.name() == "basket")
            {
                readBasket();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readBasket()
    {
        kDebug() << m_xml.name() << m_xml.attributes().value("id");

        //m_treeEplorer->loadBasket( 0, m_xml.attributes().value("id").toString() );
        //loadBasket( m_xml.attributes().value("id") );

        while (m_xml.readNextStartElement())
        {
            if (m_xml.name() == "scene")
            {
                readScene();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readScene()
    {
        kDebug() << m_xml.name();
        while (m_xml.readNextStartElement())
        {
            if (m_xml.name() == "handle")
            {
                readHandle();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readHandle()
    {
        kDebug() << m_xml.name();
        while (m_xml.readNextStartElement())
        {
            if (m_xml.name() == "item")
            {
                readItem();
            }
            else
            {
                m_xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readItem()
    {
        kDebug() << m_xml.name();
        m_xml.skipCurrentElement();
    }

}
