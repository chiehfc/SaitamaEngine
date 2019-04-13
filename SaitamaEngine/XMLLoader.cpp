#include "pch.h"
#include "XMLLoader.h"

XMLLoader::XMLLoader()
{
    //tinyxml2::XMLError eResult = m_xmlDocument.LoadFile("gameObject.xml");    
}


tinyxml2::XMLElement* XMLLoader::LoadAndReturnRootXmlElement(const char* resourceString)
{
    tinyxml2::XMLError eResult = m_xmlDocument.LoadFile(resourceString);

    return m_xmlDocument.RootElement();
}
