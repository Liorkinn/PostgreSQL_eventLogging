#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include  "/home/liora/ControlProg/common_module/declaration_path.h"

#include "xmlparser.h"
#include "dbworker.h"
#include "checkboxdelegate.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

struct{
 const int modelColumns=4,
 descriptionCount=5,
 dbnameIdx=0,
 schemaIdx=1,
 tableIdx=2,
 descriptionIdx=3,
 sort_num =4;
}treeConstants;

//Objects
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    XmlParser xml;
    std::vector <QDomElement> dbworker;
    dbWorker *dbwork;
    QString UserName = QDir::home().dirName();
    QString dbname;

//Functions
public:
    void CreateUI_TreeWidget(const QString &description);
    void settingsWidget();
    void buttonChange();
    void removeRows();
    void checkTreeWidget();
    void AddElements();
    void create_UI( const QStringList &headers );


    void CreateUI();



public:
 //   QWidget *form;
    QComboBox *comboBox;
    QTableView *table1;
    checkboxDelegate *delegate;
    QLabel *labeldb;
    QHBoxLayout *hightlayout;
    QHBoxLayout *lowlayout;
    QVBoxLayout *vlayout;
    QVBoxLayout *mainLayout;
    QString usr = QDir::home().dirName();

private slots:
    void on_pushButton_ckeck_clicked();
    void on_comboBox_db_currentIndexChanged(int index);
    void on_pushButton_get_clicked();
    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
