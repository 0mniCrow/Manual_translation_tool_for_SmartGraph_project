#include "translationxmlparser.h"

TranslationXMLParser::TranslationXMLParser()
{

}

bool TranslationXMLParser::loadElements(QDomElement& node,
                                        QList<TranslElem>& elements)
{
    bool AllOK = true;
    QString window_name(node.attribute("name"));
    QDomNode cur_window_obj = node.firstChild();
    while(!cur_window_obj.isNull())
    {
        while(true)
        {
            TranslElem new_elem;
            new_elem._window_name_ = window_name;
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
            break;
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
                        QList<QString>& languages,
                        QSet<QString>& windows)
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
            windows.insert(cur_elem.attribute("name"));
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
                        const QList<QString>& languages,
                                       const QSet<QString> &windows)
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
    QMap<QString,QDomElement> windows_map;
    for(const QString& window:windows)
    {
        QDomElement DOM_window = main_doc.createElement("window");
        DOM_window.setAttribute("name",window);
        windows_map.insert(window,DOM_window);
    }
    auto element_it = elements.cbegin();
    while(element_it!= elements.cend())
    {
        auto window_it = windows_map.find(element_it->_window_name_);

        if(window_it!=windows_map.end())
        {
            QDomElement DOM_obj = main_doc.createElement("object");
            QDomElement DOM_obj_name = main_doc.createElement("name");
            QDomText DOM_obj_name_text = main_doc.createTextNode(element_it->_object_name_);
            DOM_obj_name.appendChild(DOM_obj_name_text);
            DOM_obj.appendChild(DOM_obj_name);
            QDomElement DOM_obj_type = main_doc.createElement("type");
            QDomText DOM_obj_type_text = main_doc.createTextNode(element_it->_class_name_);
            DOM_obj_type.appendChild(DOM_obj_type_text);
            DOM_obj.appendChild(DOM_obj_type);
            QDomElement DOM_translations = main_doc.createElement("translations");
            auto transl_it = element_it->_translations_.cbegin();
            while(transl_it!=element_it->_translations_.cend())
            {
                QDomElement DOM_translation = main_doc.createElement(transl_it.key());
                QDomText DOM_translation_name = main_doc.createTextNode(transl_it.value());
                DOM_translation.appendChild(DOM_translation_name);
                DOM_translations.appendChild(DOM_translation);
                ++transl_it;
            }
            DOM_obj.appendChild(DOM_translations);
            window_it->appendChild(DOM_obj);
        }
        ++element_it;
    }

    auto window_it = windows_map.begin();
    while(window_it!=windows_map.end())
    {
        root.appendChild(*window_it);
        ++window_it;
    }
    QDomElement DOM_languages = main_doc.createElement("languages");
    for(const QString& lang:languages)
    {
        QDomElement DOM_language = main_doc.createElement("language");
        QDomText DOM_language_text = main_doc.createTextNode(lang);
        DOM_language.appendChild(DOM_language_text);
        DOM_languages.appendChild(DOM_language);
    }
    root.appendChild(DOM_languages);
    xml_stream<<main_doc.toString();
    xml_file.flush();
    xml_file.close();
    return true;
}
