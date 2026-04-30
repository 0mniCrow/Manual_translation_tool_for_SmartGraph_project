#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "translationxmlparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void addColumn();
    void removeColumn();
    void loadProject();
    void saveProject();
private:
    TranslationTableModel* _model_;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
