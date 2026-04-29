#include "translationtablemodel.h"

//#TranslElem

TranslElem::TranslElem(const TranslElem& other):
    _object_name_(other._object_name_),
    _class_name_(other._class_name_),
    _translations_(other._translations_)
{
//    std::copy(other._translations_.cbegin(),
//              other._translations_.cend(),
//              _translations_.begin());
    _translations_.detach();
    return;
}
TranslElem::TranslElem(TranslElem&& other):
    _object_name_(std::move(other._object_name_)),
    _class_name_(std::move(other._class_name_)),
    _translations_(std::move(other._translations_))
{
//    _object_name_ = std::move(other._object_name_);
//    _class_name_ = std::move(other._class_name_);
//    _translations_ =std::move(other._translations_);
    return;
}
bool TranslElem::operator==(const TranslElem& other)
{
    return _object_name_==other._object_name_;
}
bool TranslElem::operator!=(const TranslElem& other)
{
    return _object_name_!=other._object_name_;
}


//#Translation Table Model:

TranslationTableModel::TranslationTableModel(QObject *tata):QAbstractItemModel(tata)
{
    return;
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

 void TranslationTableModel::changeLangName(const QString& old_name, const QString& new_name)
 {
    bool del_mod = new_name.isEmpty();
    for(TranslElem& elem: _elements_)
    {
        if(del_mod)
        {
            elem._translations_.remove(old_name);
        }
        else
        {
            if(elem._translations_.contains(new_name))
            {
                return;
            }
            if(elem._translations_.contains(old_name))
            {
                elem._translations_.insert(new_name,elem._translations_.value(old_name));
                elem._translations_.remove(old_name);
            }
            else
            {
                elem._translations_.insert(new_name,QString());
            }
        }
    }
    return;
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
            answer = _elements_.at(getLangDataRow(index))._class_name_;
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
    if(_elements_.isEmpty()||
            !checkIndex(index)||
            (role!=Qt::EditRole))
    {
        return false;
    }
    if(index.column()<OBJECT_TRANSLATION_COLUMN)
    {
        return false;
    }
    int r_col = getLangDataCol(index);
    if(r_col>=_languages_.size())
    {
        return false;
    }
    QString lang = _languages_.at(r_col);
    if(index.row()==LANGUAGE_ROW)
    {
        QString new_lang = value.toString();
//        if(std::any_of(_languages_.begin(),
//                       _languages_.end(),
//                       [&new_lang](const QString& s){return new_lang==s;}))
        if(_languages_.contains(new_lang))
        {
            return false;
        }
        _languages_[r_col] = new_lang;
        emit dataChanged(index,index);
        changeLangName(lang,new_lang);
    }
    else
    {
        if(lang.isEmpty())
        {
            return false;
        }
        QString new_translation = value.toString();
        int r_row = getLangDataRow(index);
        _elements_[r_row]._translations_[lang] = new_translation;
        emit dataChanged(index,index);
    }
    return true;
}

int TranslationTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(_elements_.empty())
    {
        return 0;
    }
    return _elements_.size()+1;
}

int TranslationTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(_elements_.isEmpty())
    {
        return 0;
    }
    return OBJECT_TRANSLATION_COLUMN+_languages_.size();
}

QVariant TranslationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant answer;
    if(role!=Qt::DisplayRole)
    {
        return answer;
    }
    switch(orientation)
    {
    case Qt::Horizontal:
    {
        if(section == LANGUAGE_ROW)
        {
            answer = "Lang";
        }
        else
        {
            answer = "Elem N"+QString::number(section);
        }
    }
        break;
    case Qt::Vertical:
    {
        switch(section)
        {
        case OBJECT_NAME_COLUMN:
        {
            answer = "Name";
        }
            break;
        case OBJECT_CLASS_COLUMN:
        {
            answer = "Class";
        }
            break;
        default:
        {
            answer = "Language N"+QString::number(section-OBJECT_CLASS_COLUMN);
        }
        }
    }
        break;
    }
    return answer;
}
Qt::ItemFlags TranslationTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags fl;
    if(index.column()>=OBJECT_TRANSLATION_COLUMN)
    {
        fl|=Qt::ItemIsEditable;
    }
    fl|=Qt::ItemIsEnabled;
    return fl;
}

bool TranslationTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if((!count)||
            _elements_.empty()||
            column<columnCount(parent))
    {
        return false;
    }
    int r_col = column-OBJECT_TRANSLATION_COLUMN;
    if(_languages_.size()!=r_col)
    {
        return false;
    }
    beginInsertColumns(parent,column,column+count-1);
    while(true)
    {
        QString def_lang_name = "Lang "+QString::number(r_col);
        if(_languages_.contains(def_lang_name))
        {
           ++r_col;
           continue;
        }
        _languages_.append(def_lang_name);
        changeLangName(def_lang_name,def_lang_name);
        break;
    }
    endInsertColumns();
    return true;
}

bool TranslationTableModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    if((!count)||
            _elements_.empty()||
            _languages_.empty()||
            column<columnCount(parent))
    {
        return false;
    }
    int r_col = column-OBJECT_TRANSLATION_COLUMN;
    if(r_col+count>_languages_.size())
    {
        return false;
    }
    beginRemoveColumns(parent,column,column+count-1);
    while(count)
    {
        QString lang = _languages_.at(r_col);
        _languages_.remove(r_col);
        changeLangName(lang);
        --count;
    }
    endRemoveColumns();
    return true;
}

const QList<TranslElem>& TranslationTableModel::getElements() const
{
    return _elements_;
}

const QList<QString>& TranslationTableModel::getLanguages() const
{
    return _languages_;
}

bool TranslationTableModel::loadElements(QList<TranslElem>& elements,
                                         QList<QString>& languages)
{
    beginResetModel();
    _elements_ = std::move(elements);
    _languages_ = std::move(languages);
    endResetModel();
    return true;
}
