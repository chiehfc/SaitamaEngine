#include "pch.h"
#include "XMLLoader.h"

XMLLoader::XMLLoader()
{
    tinyxml2::XMLError eResult = m_xmlDocument.LoadFile("gameObject.xml");    
}

