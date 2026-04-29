#ifndef TRANSLATIONTABLEMODEL_H
#define TRANSLATIONTABLEMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QList>
#include <QVariant>

#define OBJECT_NAME_COLUMN 0
#define OBJECT_CLASS_COLUMN 1
#define OBJECT_TRANSLATION_COLUMN 2
#define LANGUAGE_ROW 0

struct TranslElem
{
    QString     _object_name_;
    QString     _class_name_;
    QMap<QString,QString> _translations_;
    TranslElem(){}
    TranslElem(const TranslElem& other);
    TranslElem(TranslElem&& other);
    bool operator==(const TranslElem& other);
    bool operator!=(const TranslElem& other);

};

class TranslationTableModel:public QAbstractItemModel
{
    Q_OBJECT
private:
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
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool insertColumns(int column, int count, const QModelIndex &parent) override;
    bool removeColumns(int column, int count, const QModelIndex& parent) override;
    const QList<TranslElem>& getElements() const;
    const QList<QString>& getLanguages() const;
    bool loadElements(QList<TranslElem> &elements, QList<QString> &languages);
};

#endif // TRANSLATIONTABLEMODEL_H
