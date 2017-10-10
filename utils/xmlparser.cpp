#include <iostream>
#include "xmlparser.h"


XmlParser::XmlParser(const char *xml_path) : m_root(NULL)
{
    if ( m_doc.LoadFile( xml_path ) == 0 )
    {
        m_root = m_doc.RootElement();
    }
}

bool XmlParser::is_empty()
{
    return ( m_root == NULL ? true : false );
}

//获取配置节点
XMLElement * XmlParser::get_key_node(const char *module, const char *key)
{
    // 模块节点
    XMLElement *module_node = m_root->FirstChildElement(module);
    if ( module_node == NULL )
    {
        return NULL;
    }

    // 配置节点
    XMLElement *key_node = module_node->FirstChildElement(key);
    if ( key_node == NULL )
    {
        return NULL;
    }

    return key_node;
}

//获取int节点值
int XmlParser::get_int(const char *module, const char *key, int &value)
{
    XMLElement *key_node = get_key_node(module, key);
    if ( key_node == NULL )
    {
        return -1;
    }
    value = key_node->IntText(0);
    return 0;
}

//获取unsigned short节点值
int XmlParser::get_ushort(const char *module, const char *key, unsigned short &value)
{
    XMLElement *key_node = get_key_node(module, key);
    if ( key_node == NULL )
    {
        return -1;
    }
    value = (unsigned short)atoi( key_node->GetText() );
    return 0;
}

//获取string节点值
int XmlParser::get_string(const char *module, const char *key, string &value)
{
    XMLElement *key_node = get_key_node(module, key);
    if ( key_node == NULL )
    {
        return -1;
    }
    value = string( key_node->GetText() );
    return 0; 
}

