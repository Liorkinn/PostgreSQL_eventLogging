#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbwork = new dbWorker();
    ui->treeWidget->setColumnHidden(3,true);
    CreateUI();
    ui->tableRecovery->setSelectionMode(QAbstractItemView::SingleSelection); /// < Устанавливаем в tableview одиночную выборку строк.
    ui->tableRecovery->setSelectionBehavior(QAbstractItemView::SelectRows); /// < Устанавливаем в tableview одиночную выборку строк.
    ui->tableRecovery->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setCurrentWidget(ui->tab);

    if (xml.open(PATH_DATABASE)) {          ///< В самом начале открывается database.xml, тем самым при успешном открытии происходит наполнение std::vector и заполнение comboBox..
        qDebug() << "[Information] Идёт чтение database.xml... Данные успешно получены!";
            dbworker = xml.ListElements(xml.getRoot(), "database");

            for(unsigned int n = 0; n < dbworker.size(); n++){
                dbwork->dbConnect (n,
                                   ui->comboBox_db,
                                   dbworker[n].attribute("driver"),
                                   dbworker[n].attribute("hostname"),
                                   dbworker[n].attribute("port").toInt(),
                                   dbworker[n].attribute("name"),
                                   "1",
                                   "postgres",
                                   dbworker[n].attribute("options"));

                dbwork->addComboBoxElements(n, ui->comboBox_db, dbworker[n].attribute("name"));
                dbwork->colorDbName(n, ui->comboBox_db);

                comboBox->addItem(dbworker[n].attribute("name"),QVariant(n));
            }
    }else ///< Если файл не откроется - пользователю выводится ошибка.
    {
        qDebug() << "[Error] Файл не открыт! Проверьте права доступа database.xml, убедитесь\nчто файл расположен по пути /mnt/abi/settings/,\nа также проверьте правильность содержимого xml файла.";
    }



    connect(comboBox, &QComboBox::currentTextChanged, this, [this]
    {
        int i = comboBox->currentIndex();
        dbwork->checkResourses (comboBox,
                                delegate,
                                table1,
                                usr,
                                dbworker[i].attribute("driver"),
                                dbworker[i].attribute("hostname"),
                                dbworker[i].attribute("port").toInt(),
                                dbworker[i].attribute("name"),
                                "1",
                                "postgres",
                                dbworker[i].attribute("options"));
    });

    connect(delegate, &checkboxDelegate::_change, [this](const int& state) ///< Слот, обеспечивающий логирование в БД abi_db, а также включение/выключение триггерных функций.
    {
        dbwork->settingTriggers(comboBox,table1,(bool)state);
    });

}


void MainWindow::CreateUI_TreeWidget(const QString &description)
{
    if(description.isEmpty()){
         return;
    }

    QStringList list=description.split(",");

    if(list.length()!=treeConstants.descriptionCount)
        return ;

    QString dbnameStr = list.at(treeConstants.dbnameIdx);
    QString schemaStr = list.at(treeConstants.schemaIdx);
    QString tableStr  = list.at(treeConstants.tableIdx);
    QString descriptionStr= list.at(treeConstants.descriptionIdx);
    QString sort_n= list.at(treeConstants.sort_num);

    auto dbnameFind=ui->treeWidget->findItems(dbnameStr, Qt::MatchCaseSensitive,0);
    QTreeWidgetItem *dbName;
    dbnameFind.isEmpty() ? dbName=new QTreeWidgetItem
                          : dbName=dbnameFind.at(0);

    auto schemaFind=ui->treeWidget->findItems(schemaStr,
                                   Qt::MatchCaseSensitive | Qt::MatchRecursive, 0);
    QTreeWidgetItem *schemaName;
    schemaFind.isEmpty() ? schemaName=new QTreeWidgetItem(dbName)
                        : schemaName=schemaFind.at(0);
    QTreeWidgetItem *model  =new QTreeWidgetItem(schemaName);
    dbName->setText(0, dbnameStr);
    schemaName->setText(0, schemaStr);
    model->setText(1, tableStr);
    model->setText(2, descriptionStr);
    model->setText(3, sort_n);

    dbName->setToolTip(0, "Имя БД");

    schemaName->setToolTip(0, schemaName->parent()->text(0));

    for(int i=0; i<treeConstants.modelColumns; i++){

        switch (i){
        case 0:
            model->setToolTip(i, model->parent()->text(0));
            break;
        case 1:
            model->setToolTip(i, "Таблица");
            break;
        case 2:
            model->setToolTip(i, "Описание таблицы");
            break;
        }
    }
     ui->treeWidget->addTopLevelItem(dbName);

}

void MainWindow::settingsWidget()
{
    ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::buttonChange()
{
    if(ui->pushButton_ckeck->text() == "Показать"){
        ui->pushButton_ckeck->setText("Отобразить для снятия с учёта");
        ui->pushButton_get->setText("Поставить на учёт");
        ui->label->setText("Список  таблиц базы данных " + ui->comboBox_db->currentText() + " для поставки на учёт");
        qDebug() << "[Information] Пользователь " + UserName + " просматривает список таблиц.";
    }else if(ui->pushButton_ckeck->text() == "Отобразить для снятия с учёта"){
        ui->pushButton_ckeck->setText("Отобразить для постановки на учёт");
        ui->pushButton_get->setText("Убрать с учёта");
        ui->label->setText("Список таблиц базы данных " + ui->comboBox_db->currentText() + " для снятия с учёта");
        qDebug() << "[Information] Пользователь " + UserName + " просматривает список  таблиц, поставленные на контроль.";
    }else if(ui->pushButton_ckeck->text() == "Отобразить для постановки на учёт"){
        ui->pushButton_ckeck->setText("Отобразить для снятия с учёта");
        ui->pushButton_get->setText("Поставить на учёт");
        ui->label->setText("Список таблиц базы данных " + ui->comboBox_db->currentText() + " для поставки на учёт");
        qDebug() << "[Information] Пользователь " + UserName + " просматривает список таблиц.";
    }
}

void MainWindow::removeRows()
{
    auto rows = ui->treeWidget->currentIndex();
    ui->treeWidget->model()->removeRow(rows.row(), rows.parent());
}

void MainWindow::checkTreeWidget()
{
    ui->treeWidget->clear();
    int i = ui->comboBox_db->currentIndex();
    dbname = ui->comboBox_db->currentText();
    dbwork->dbConnect (i,
                       ui->comboBox_db,
                       dbworker[i].attribute("driver"),
                       dbworker[i].attribute("hostname"),
                       dbworker[i].attribute("port").toInt(),
                       dbworker[i].attribute("name"),
                       "1",
                       "postgres",
                       dbworker[i].attribute("options"));

    if(dbwork->db.open() && ui->pushButton_get->text() == "Поставить на учёт"){
        AddElements();
        qDebug() << "[Information] Подключение к БД... \n[Success] Успешное подключение. Пользователь: " + UserName + ". Подключение к БД: " + dbname + " IP: "+ dbworker[i].attribute("hostname");
    }else if(dbwork->db.open() && ui->pushButton_get->text() == "Убрать с учёта"){
        AddElements();
    }else{
        QMessageBox::critical(0,"Ошибка соединения","Нет соединения с БД!");
        qDebug() << "[Information] Подключение к БД... \n[Error] Ошибка подключения к БД. Пользователь: " + UserName + ". Подключение к БД: " + dbname + " IP: "+ dbworker[i].attribute("hostname");
    }
}

void MainWindow::AddElements()
{
    QSqlQuery *query = new QSqlQuery(dbwork->db);
    QString dbNames = ui->comboBox_db->currentText();
    if(ui->pushButton_get->text() == "Поставить на учёт"){
        query->exec("SELECT t4.description, t2.nspname, t1.relname from pg_namespace as t2 LEFT JOIN pg_class as t1 on t1.relnamespace = t2.oid and  t1.relkind = 'r'::\"char\""
                                "LEFT JOIN  pg_description as t3 on  t3.objoid = t2.oid LEFT JOIN  pg_description as t4 on  t4.objoid = t1.oid and t4.objsubid=0 WHERE t2.nspname not in ('pg_catalog', 'pg_toast', 'pg_temp_1','information_schema', 'pg_toast_temp_1', 'chk')"
                                "AND t1.relname NOT IN(SELECT tablename FROM chk.\"Table_state\") AND t1.relname IS NOT NULL");
        for(int i = 0; query->next();i++)
        {
            CreateUI_TreeWidget(dbNames+","+ query->value(1).toString()+"," + query->value(2).toString() +"," + query->value(0).toString() +"," + " ");
        }

        if(!query->exec("SELECT * FROM chk.\"Table_state\""))
        {
           QMessageBox::critical(0, "Ошибка!", "Необходимо создать таблицу Table_state в схеме chk для корректной работы программы!");
           return;
        }
    }else if(ui->pushButton_get->text() == "Убрать с учёта"){

        query->exec("SELECT description, schemaname, tablename, sort_number FROM chk.\"Table_state\" ORDER BY sort_number");

        for(int i = 0; query->next();i++)
        {
            CreateUI_TreeWidget(dbNames+","+ query->value(1).toString()+"," + query->value(2).toString() +"," + query->value(0).toString() +"," + query->value(3).toString());
        }

        if(!query->exec("SELECT * FROM chk.\"Table_state\""))
        {
           QMessageBox::critical(0, "Ошибка!", "Необходимо создать таблицу Table_state в схеме chk для корректной работы программы!");
           return;
        }
    }
}

void MainWindow::create_UI(const QStringList &headers)
{           ui->tableRecovery->setRowCount(0);
            ui->tableRecovery->setColumnCount                                 ( 7 );
            ui->tableRecovery->setShowGrid                                    ( true );
            ui->tableRecovery->setHorizontalHeaderLabels                      ( headers );
            ui->tableRecovery->horizontalHeader()->setStretchLastSection      ( true );
            dbwork->query = new QSqlQuery(dbwork->db);
            dbwork->query->exec("SELECT * FROM _abi.events_controldb WHERE type_operation = 'DELETE'");
            for(unsigned int i = 0; dbwork->query->next(); i++)
            {
                ui->tableRecovery->insertRow(i);
                ui->tableRecovery->setItem(i,0, new QTableWidgetItem(dbwork->query->value(0).toString()));
                ui->tableRecovery->setItem(i,1, new QTableWidgetItem(dbwork->query->value(1).toString()));
                ui->tableRecovery->setItem(i,2, new QTableWidgetItem(dbwork->query->value(2).toString()));
                ui->tableRecovery->setItem(i,3, new QTableWidgetItem(dbwork->query->value(3).toString()));
                ui->tableRecovery->setItem(i,4, new QTableWidgetItem(dbwork->query->value(4).toString()));
                ui->tableRecovery->setItem(i,5, new QTableWidgetItem(dbwork->query->value(5).toString()));
                ui->tableRecovery->setItem(i,6, new QTableWidgetItem(dbwork->query->value(6).toString()));
            }
            ui->tableRecovery->horizontalHeader()->resizeSection( 0, 175 );
            ui->tableRecovery->horizontalHeader()->resizeSection( 1, 175 );
}

void MainWindow::CreateUI()
{
    hightlayout = new QHBoxLayout;
    lowlayout = new QHBoxLayout;
    vlayout = new QVBoxLayout;
    mainLayout = new QVBoxLayout;

    comboBox =  new QComboBox;
    table1   =  new QTableView;
    delegate =  new checkboxDelegate;

    labeldb  =  new QLabel ("База данных:");

    //..End create forms...

    table1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table1->horizontalHeader()->setStretchLastSection(true);
    table1->horizontalHeader()->setStretchLastSection(true);
    table1->setSelectionMode(QAbstractItemView::SingleSelection); /// < Устанавливаем в tableview одиночную выборку строк.
    table1->setEditTriggers(QAbstractItemView::NoEditTriggers); /// < Устанавливаем запрет на редактирование ячеек.


    ui->tab_2->setObjectName         ("form");
    ui->tab_2->setMinimumSize        (1050,650);
    setStyleSheet                    ("QWidget#form{background: white;}");

    labeldb->setMinimumWidth(96);
    comboBox->setMinimumWidth(700);

    labeldb->setMaximumSize(78,78);

    hightlayout->addWidget(labeldb); hightlayout->addWidget(comboBox);  hightlayout->addStretch();
    vlayout->addWidget(table1);
    lowlayout->addStretch();

    mainLayout->addLayout(hightlayout); mainLayout->addLayout(vlayout); mainLayout->addLayout(lowlayout);
    ui->tab_2->setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_ckeck_clicked()
{
    buttonChange();
    checkTreeWidget();
}

void MainWindow::on_comboBox_db_currentIndexChanged(int index)
{
    ui->pushButton_ckeck->setText("Показать");
}

void MainWindow::on_pushButton_get_clicked()
{
    dbwork->getControl(ui->treeWidget,ui->pushButton_get);
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    int i = comboBox->currentIndex();
    dbwork->checkResourses (comboBox,
                            delegate,
                            table1,
                            usr,
                            dbworker[i].attribute("driver"),
                            dbworker[i].attribute("hostname"),
                            dbworker[i].attribute("port").toInt(),
                            dbworker[i].attribute("name"),
                            "1",
                            "postgres",
                            dbworker[i].attribute("options"));

    this->create_UI(QStringList()      << ( "Пользователь" )
                                      << ( "Имя БД" )
                                      << ( "Имя таблицы" )
                                      << ( "Тип операции" )
                                      << ( "Дата/время" )
                                      << ( "Имя схемы" )
                                      << ( "Удалённый запрос" ));
}



QString MainWindow::addSymbol(QString letters)
{
    int index = 0;
    if(letters.isEmpty())
        return 0;

    int a = letters.indexOf("(");
    letters = letters.insert(a+1, "'");
    a = letters.indexOf(")");
    letters = letters.insert(a, "'");

    foreach (QString a, letters) {
       if(a == ",") {
           index++;
           letters = letters.insert(index-1, "'"); index++;
           letters = letters.insert(index, "'"); index++;
       }else {
           index++;
       }
    }
    return letters;
}

void MainWindow::on_pushButton_clicked()
{
    QString basename = ui->tableRecovery->selectionModel()->currentIndex().siblingAtColumn(1).data().toString(); //описание
    QString schemaname = ui->tableRecovery->selectionModel()->currentIndex().siblingAtColumn(5).data().toString(); //схема
    QString tablename = ui->tableRecovery->selectionModel()->currentIndex().siblingAtColumn(2).data().toString();  //таблица
    QString sql_data = ui->tableRecovery->selectionModel()->currentIndex().siblingAtColumn(6).data().toString(); //описание

    system(qPrintable("/home/liora/MIS_eventLogging/recovery.sh " + basename + " " + schemaname + " " + tablename + " " + "\"" + addSymbol(sql_data) + "\""));
    qDebug() << addSymbol(sql_data);
    auto rows = ui->tableRecovery->currentIndex();
    ui->tableRecovery->model()->removeRow(rows.row());//..
}

