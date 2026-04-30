#ifndef TRANSLATIONTABLEMODEL_H
#define TRANSLATIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QList>
#include <QVariant>

#define OBJECT_WINDOW_NAME 0
#define OBJECT_NAME_COLUMN 1
#define OBJECT_CLASS_COLUMN 2
#define OBJECT_TRANSLATION_COLUMN 3
#define LANGUAGE_ROW 0

struct TranslElem
{
    QString     _object_name_;
    QString     _class_name_;
    QString     _window_name_;
    QMap<QString,QString> _translations_;
    TranslElem(){}
    TranslElem(const TranslElem& other);
    TranslElem(TranslElem&& other);
    TranslElem& operator=(const TranslElem& other);
    TranslElem& operator=(TranslElem&& other);
    bool operator==(const TranslElem& other);
    bool operator!=(const TranslElem& other);

};

using WindowsMap = QMap<QString,QList<TranslElem>>;

class TranslationTableModel:public QAbstractTableModel
{
    Q_OBJECT
private:
    QSet<QString>       _windows_;
    QList<TranslElem>   _elements_;
    QList<QString>      _languages_;
    bool checkIndex(const QModelIndex& index) const;
    int getLangDataRow(const QModelIndex& index) const;
    int getLangDataCol(const QModelIndex& index) const;
    void changeLangName(const QString& old_name, const QString& new_name = QString());
public:
    TranslationTableModel(QObject * tata = nullptr);
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant &value, int role) override;
    int rowCount(const QModelIndex& parent= QModelIndex()) const override;
    int columnCount(const QModelIndex& parent= QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    //QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    const QList<TranslElem>& getElements() const;
    const QList<QString>& getLanguages() const;
    const QSet<QString>& getWindows() const;
    bool loadElements(QList<TranslElem>& elements,
                      QSet<QString>& windows,
                      QList<QString>& languages);
};

#endif // TRANSLATIONTABLEMODEL_H
