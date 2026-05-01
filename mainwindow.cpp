#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QItemSelectionModel * old_model = ui->tableView_translations->selectionModel();
     _model_ = new TranslationTableModel(ui->tableView_translations);
    ui->tableView_translations->setModel(_model_);
    delete old_model;
    connect(ui->Button_add,&QPushButton::clicked,this,&MainWindow::addColumn);
    connect(ui->Button_remove,&QPushButton::clicked,this,&MainWindow::removeColumn);
    connect(ui->Button_load,&QPushButton::clicked,this,&MainWindow::loadProject);
    connect(ui->Button_save,&QPushButton::clicked,this,&MainWindow::saveProject);
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
    return;
}

void MainWindow::addColumn()
{
    _model_->insertColumns(_model_->columnCount(),1);
    return;
}

void MainWindow::removeColumn()
{
    _model_->removeColumns(_model_->columnCount()-1,1);
    return;
}

void MainWindow::loadProject()
{
    QString addr(QFileDialog::getOpenFileName(
                         this,"Chose file to load",
                         QDir::currentPath(),
                         "XML files (*.xml)",nullptr,
                         QFileDialog::DontUseNativeDialog));
    if(addr.isEmpty())
    {
        return;
    }
    ui->line_load->setText(addr);
    QList<TranslElem> elements;
    QList<QString> languages;
    QSet<QString> windows;
    if(TranslationXMLParser::loadProject(addr,elements,languages,windows))
    {
        _model_->loadElements(elements,windows,languages);
    }
    return;
}

void MainWindow::saveProject()
{
    QString addr = QFileDialog::getSaveFileName(this,"Chose file to save",
                                                QDir::currentPath(),
                                                "XML files (*.xml)",nullptr,
                                                QFileDialog::DontUseNativeDialog);
    if(addr.isEmpty())
    {
        return;
    }
    ui->line_save->setText(addr);
    TranslationXMLParser::saveProject(addr,_model_->getElements(),
                                      _model_->getLanguages(),
                                      _model_->getWindows());
    return;
}
