#ifndef TRANSLATIONXMLPARSER_H
#define TRANSLATIONXMLPARSER_H
#include "translationtablemodel.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>
#include <QFile>

class TranslationXMLParser
{
private:
    static bool loadElements(QDomElement &node, QList<TranslElem>& elements);
    static bool loadLanguages(QDomElement& node, QList<QString>& languages);

public:
    TranslationXMLParser();
    static bool loadProject(const QString& file_addr,
                            QList<TranslElem>& elements,
                            QList<QString>& languages);
    static bool saveProject(const QString& file_addr,
                            const QList<TranslElem>& elements,
                            const QList<QString>& languages);
};

#endif // TRANSLATIONXMLPARSER_H
