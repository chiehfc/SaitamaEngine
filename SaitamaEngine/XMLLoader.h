#pragma once
#include "Saitama.h"

class XMLLoader
{
public:
    XMLLoader();

    tinyxml2::XMLElement* GetRoot(void) { return m_xmlDocument.RootElement(); }

    //tinyxml2::XMLElement* LoadAndReturnRootXmlElement(const char* resourceString);

private:
    tinyxml2::XMLDocument m_xmlDocument;
};

