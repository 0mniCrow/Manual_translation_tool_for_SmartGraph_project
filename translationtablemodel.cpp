#include "translationtablemodel.h"

TranslationTableModel::TranslationTableModel(QObject *tata):QAbstractItemModel(tata)
{

}

bool TranslationTableModel::checkIndex(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return false;
    }
    if(index.column()>=OBJECT_TRANSLATION_COLUMN + _languages_.size())
    {
        return false;
    }
    if(index.row()>=_elements_.size()+1)
    {
        return false;
    }
    return true;
}

int TranslationTableModel::getLangDataRow(const QModelIndex& index) const
{
    return index.row()-1;
}
int TranslationTableModel::getLangDataCol(const QModelIndex& index) const
{
    return index.column()-OBJECT_TRANSLATION_COLUMN;
}

QVariant TranslationTableModel::data(const QModelIndex& index, int role) const
{
    if(_elements_.isEmpty() || !checkIndex(index))
    {
        return QVariant();
    }
    QVariant answer;

    if(index.row()==LANGUAGE_ROW)
    {
        if((index.column()>=OBJECT_TRANSLATION_COLUMN)&&
                ((role==Qt::DisplayRole)||
                 (role==Qt::EditRole)))
        {
            answer = _languages_.at(getLangDataCol(index));
        }
    }
    else
    {
        if((index.column()==OBJECT_NAME_COLUMN)&&
                (role==Qt::DisplayRole))
        {
            answer = _elements_.at(getLangDataRow(index))._object_name_;
        }
        else if((index.column()==OBJECT_CLASS_COLUMN)&&
                (role==Qt::DisplayRole))
        {
            answer = _elements_.at(getLangDataRow(index))._object_name_;
        }
        else if((index.column()>=OBJECT_TRANSLATION_COLUMN)&&
                ((role==Qt::DisplayRole)||
                 (role==Qt::EditRole)))
        {
            QString lang = _languages_.at(getLangDataCol(index));
            if(!lang.isEmpty())
            {
                answer = _elements_.at(getLangDataRow(index))._translations_.value(lang);
            }
        }
    }
    return answer;
}

bool TranslationTableModel::setData(const QModelIndex& index, const QVariant &value, int role)
{

}
int TranslationTableModel::rowCount(const QModelIndex& parent) const
{

}
int TranslationTableModel::columnCount(const QModelIndex& parent) const
{

}
QVariant TranslationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

}
Qt::ItemFlags TranslationTableModel::flags(const QModelIndex& index) const
{

}
bool TranslationTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{

}
bool TranslationTableModel::removeColumns(int column, int count, const QModelIndex& parent)
{

}
bool TranslationTableModel::removeRows(int row, int count, const QModelIndex& parent)
{

}
