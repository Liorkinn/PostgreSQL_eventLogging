#ifndef DBWORKER_H
#define DBWORKER_H

#include "resourse_card.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDomElement>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QTreeWidget>
#include <QAbstractButton>
#include <QtWidgets>
#include <QSqlTableModel>
#include <QSqlQueryModel>


class dbWorker
{
public:

    dbWorker();
    QSqlDatabase db;
    QSqlDatabase db1,abiDB;
    QSqlQuery *query;
    QMessageBox msgBox;
    resourse_card *res;
    QString schemaname;
    QString tablename;
    QString description;
    QString type_cont;
    int s_num;
public:
    void dbConnect(int n, QComboBox *QComboBox, QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions);
    void colorDbName(int n, QComboBox *QComboBox);
    void addComboBoxElements(int n, QComboBox *comboBox, QString dbName);
    void getControl(QTreeWidget *tree, QPushButton *button);


    void abiConnect(QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions, std::vector <QDomElement> list);
    void checkResourses(QComboBox *combobx, QAbstractItemDelegate *deleg,  QTableView *table, QString usr, QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions);
    void settingTriggers(QComboBox *cmb, QTableView *tbl, const int &state);
    void loggingDBAccess(int action, QComboBox *cmb, QTableView *tbl);
    void updTableState(int action, QTableView *tbl);
};

#endif // DBWORKER_H
