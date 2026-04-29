#include "translationxmlparser.h"

TranslationXMLParser::TranslationXMLParser()
{

}

bool TranslationXMLParser::loadElements(QDomElement& node, QList<TranslElem>& elements)
{
    bool AllOK = true;
    QDomNode cur_window_obj = node.firstChild();
    while(!cur_window_obj.isNull())
    {
        while(true)
        {
            TranslElem new_elem;
            if(cur_window_obj.nodeName()!="object")
            {
                AllOK=false;
                break;
            }
            QDomElement cur_obj = cur_window_obj.toElement();
            QDomElement cur_obj_name = cur_obj.firstChildElement("name");
            if(cur_obj_name.isNull())
            {
                AllOK=false;
                break;
            }
            new_elem._object_name_ = cur_obj_name.text();
            QDomElement cur_obj_type = cur_obj.firstChildElement("type");
            if(cur_obj_type.isNull())
            {
                AllOK=false;
                break;
            }
            new_elem._class_name_ = cur_obj_type.text();
            QDomElement translations = cur_obj.firstChildElement("translations");
            if(!translations.isNull())
            {
                QDomNode cur_translation = translations.firstChild();
                while(!cur_translation.isNull())
                {
                    QDomElement cur_transl_element = cur_translation.toElement();
                    new_elem._translations_.insert(cur_transl_element.tagName(),
                                                   cur_transl_element.text());
                    cur_translation = cur_translation.nextSibling();
                }
            }
            elements.append(std::move(new_elem));
        }
        cur_window_obj = cur_window_obj.nextSibling();
    }
    return AllOK;
}

bool TranslationXMLParser::loadLanguages(QDomElement &node, QList<QString>& languages)
{
    QDomNode cur_langs = node.firstChild();
    while(!cur_langs.isNull())
    {
        if(cur_langs.nodeName()=="language")
        {
            QDomElement cur_transl = cur_langs.toElement();
            languages.append(cur_transl.text());
        }
        cur_langs = cur_langs.nextSibling();
    }
    return true;
}

bool TranslationXMLParser::loadProject(const QString& file_addr,
                        QList<TranslElem>& elements,
                        QList<QString>& languages)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QDomDocument xml_doc("xmlTrnslDoc");
    if(!xml_doc.setContent(&xml_file))
    {
        xml_file.close();
        return false;
    }
    QDomElement root = xml_doc.documentElement();
    QDomNode cur_node = root.firstChild();
    while(!cur_node.isNull())
    {
        QDomElement cur_elem = cur_node.toElement();
        if(cur_elem.nodeName()=="window")
        {
            loadElements(cur_elem,elements);
        }
        else if(cur_elem.nodeName()=="languages")
        {
            loadLanguages(cur_elem,languages);
        }
        cur_node = cur_node.nextSibling();
    }
    xml_file.close();
    return true;
}

bool TranslationXMLParser::saveProject(const QString& file_addr,
                        const QList<TranslElem>& elements,
                        const QList<QString>& languages)
{
    QFile xml_file(file_addr);
    if(!xml_file.open(QFile::WriteOnly|QFile::Text))
    {
        return false;
    }
    QTextStream xml_stream(&xml_file);
    QDomDocument main_doc;
    QDomElement root = main_doc.createElement("translation");
    main_doc.appendChild(root);
    return true;
}
