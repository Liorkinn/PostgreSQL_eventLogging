#include "dbworker.h"

#include <QComboBox>
dbWorker::dbWorker()
{

}

void dbWorker::dbConnect(int n, QComboBox *QComboBox,  QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions)
{
    db = QSqlDatabase::addDatabase                  (driverName);
    db.setHostName                                  (hostName);
    db.setPort                                      (port);
    db.setDatabaseName                              (dbName);
    db.setPassword                                  (pwd);
    db.setUserName                                  (userName);
    db.setConnectOptions                            (connectOptions);
    if(db.open()){
        qDebug() << "[Success] Подключение к БД " + dbName + " произошло успешно!";
    }else{
        qDebug() << "[Success] Подключение к БД " + dbName + " произошло с провалом!";
    }
}

void dbWorker::colorDbName(int n, QComboBox *QComboBox)
{
    if(db.open()){
        QComboBox->setItemData( n, QColor( Qt::green ), Qt::BackgroundRole );
    }else{
        QComboBox->setItemData( n, QColor( Qt::red ), Qt::BackgroundRole );
    }
}

void dbWorker::addComboBoxElements(int n, QComboBox *comboBox, QString dbName)
{
    comboBox->addItem(dbName ,QVariant(n));
}

void dbWorker::getControl(QTreeWidget *tree, QPushButton *button)
{

    query = new QSqlQuery(db);
    schemaname = tree->selectionModel()->currentIndex().parent().siblingAtColumn(0).data().toString(); //схема
    tablename = tree->selectionModel()->currentIndex().siblingAtColumn(1).data().toString();  //таблица
    description = tree->selectionModel()->currentIndex().siblingAtColumn(2).data().toString(); //описание
 //   s_num = tree->selectionModel()->currentIndex().siblingAtColumn(3).data().toInt(); //номер
    db = QSqlDatabase::database();



    if(tablename == NULL && description == NULL) {
        QMessageBox::critical(0, "Ошибка добавления", "Необходимо выбрать строку!");
    } else if(button->text() == "Поставить на учёт"){

        res = new resourse_card(schemaname + "." + tablename, description);
        res->exec();
        description = res->desc;
        s_num = res->sort;
        type_cont = res->type_oper;
         if(type_cont == "Жесткий") {
             query->prepare("INSERT INTO chk.\"Table_state\"(description, schemaname, tablename, hard_control, sort_number) VALUES(:bdescription, :bschemaname, :btablename, :hardcontrol, :sortnum);");
             query->bindValue(":bdescription", description);
             query->bindValue(":bschemaname", schemaname);
             query->bindValue(":btablename", tablename);
             query->bindValue(":hardcontrol", "true");
             query->bindValue(":sortnum", s_num);
             if(query->exec())
             {
                 QString str = "CREATE TRIGGER Journal_i AFTER INSERT ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.insert_journal_audit();"
                               "CREATE TRIGGER Journal_u AFTER UPDATE ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.update_journal_audit();"
                               "CREATE TRIGGER Journal_d AFTER DELETE ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.delete_journal_audit();";
                 if(query->exec(str)){
                     qDebug() << "[Success] Триггеры на таблицу " + schemaname + "." + tablename + " успешно добавлены! Выполненный запрос: " + str;}
                 else{
                     qDebug() << "[Error] Триггеры на таблицу " + schemaname + "." + tablename + " не добавлены! Проваленный запрос: " + str;
                 }
                 qDebug() << "[Success] Ресурс " + schemaname + "." + tablename + " успешно добавлен в журнал с контролем типа ЖЕСТКИЙ! Выполненный запрос: " + str;

                 auto rows = tree->currentIndex();
                 tree->model()->removeRow(rows.row(), rows.parent());
             }else
             {
                  QMessageBox::critical(0,"Ошибка","Ошибка постановки защищаемого ресурса на контроль! Данные в журнал не добавлены!");
             }
         }
         else if(type_cont == "Обычный") {

             query->prepare("INSERT INTO chk.\"Table_state\"(description, schemaname, tablename, hard_control, sort_number) VALUES(:bdescription, :bschemaname, :btablename, :hardcontrol, :sortnum);");
             query->bindValue(":bdescription", description);
             query->bindValue(":bschemaname", schemaname);
             query->bindValue(":btablename", tablename);
             query->bindValue(":hardcontrol", "false");
             query->bindValue(":sortnum", s_num);
             if(query->exec())
             {
                 QString str = "CREATE TRIGGER Journal_i AFTER INSERT ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.insert_journal_audit();"
                               "CREATE TRIGGER Journal_u AFTER UPDATE ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.update_journal_audit();"
                               "CREATE TRIGGER Journal_d AFTER DELETE ON " + schemaname +"." + '"' + tablename + '"' + " FOR EACH ROW EXECUTE PROCEDURE chk.delete_journal_audit();"
                               "ALTER TABLE "  + schemaname +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_i;"
                               "ALTER TABLE "  + schemaname +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_u;"
                               "ALTER TABLE "  + schemaname +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_d;";
                 if(query->exec(str)){
                     qDebug() << "[Success] Триггеры на таблицу " + schemaname + "." + tablename + " успешно добавлены! Выполненный запрос: " + str;}
                 else{
                     QMessageBox::critical(0,"Ошибка","Ошибка постановки защищаемого ресурса на контроль!");
                     qDebug() << "[Error] Триггеры на таблицу " + schemaname + "." + tablename + " не добавлены! Проваленный запрос: " + str;
                 }




                 qDebug() << "[Success] Ресурс " + schemaname + "." + tablename + " успешно добавлен в журнал с контролем типа ОБЫЧНЫЙ! Выполненный запрос: " + str;

                 auto rows = tree->currentIndex();
                 tree->model()->removeRow(rows.row(), rows.parent());
                 QMessageBox::information(0, "Успешное добавление", "Триггеры на таблицу " + schemaname + "." + tablename + " успешно добавлены!");
             }else
             {
                  QMessageBox::critical(0, "Ошибка","Ошибка постановки защищаемого ресурса на контроль! Данные в журнал не добавлены!");
             }
         }

    }else if(button->text() == "Убрать с учёта")
    {
        msgBox.setWindowTitle("Просмотр"); msgBox.setText("Вы уверены в удалении?"); msgBox.setMinimumHeight(500);
        QAbstractButton *remove = msgBox.addButton(QObject::tr("Удалить"), QMessageBox::ActionRole);
        QAbstractButton *cancel = msgBox.addButton(QObject::tr("Отмена"), QMessageBox::ActionRole);
        msgBox.exec();
        if(msgBox.clickedButton() == remove){
            QString delTrigger, delUpdate, delDelete;
            delTrigger = "DROP TRIGGER journal_i ON " + schemaname +"." + '"' + tablename + '"' + ";";
            delUpdate  = "DROP TRIGGER journal_u ON " + schemaname +"." + '"' + tablename + '"' + ";";
            delDelete  = "DROP TRIGGER journal_d ON " + schemaname +"." + '"' + tablename + '"' + ";";
            if(query->exec(delTrigger))
            {
                qDebug() << "[Success] Триггер вставки " + schemaname + "." + tablename + " успешно удален! Выполненный запрос: " + delTrigger;
            }else{
                qDebug() << "[Error] Ошибка выполнения запроса: " + delTrigger; QMessageBox::critical(0,"Удаление события", "Ошибка удаления события вставки из " + schemaname + "." + tablename + "!");
            }

            if(query->exec(delUpdate))
            {
                qDebug() << "[Success] Триггер обновления " + schemaname + "." + tablename + " успешно удален! Выполненный запрос: " + delUpdate;
            }else{
                qDebug() << "[Error] Ошибка выполнения запроса: " + delUpdate; QMessageBox::critical(0,"Удаление события", "Ошибка удаления события обновления из" + schemaname + "." + tablename + "!");
            }

            if(query->exec(delDelete))
            {
                qDebug() << "[Success] Триггер удаления" + schemaname + "." + tablename + " успешно удален! Выполненный запрос: " + delDelete;
            }else{
                qDebug() << "[Error] Ошибка выполнения запроса: " + delDelete; QMessageBox::critical(0,"Удаление события", "Ошибка удаления события удаления из:" + schemaname + "." + tablename + "!");
            }

            query->prepare("DELETE FROM chk.\"Table_state\" WHERE schemaname = :bschemaname AND tablename = :btablename;");
            query->bindValue(":bschemaname", schemaname);
            query->bindValue(":btablename", tablename);
            if(query->exec())
            {
               qDebug() << "[Success] Триггеры " + schemaname + "." + tablename + " успешно удалены!";

               auto rows = tree->currentIndex();
               tree->model()->removeRow(rows.row(), rows.parent());

            }else
            {
                QMessageBox::critical(0, "Ошибка удаления","При снятии с учёта возникла ошибка.");
                qDebug() << "[Error] Ошибка выполнения запроса для " + schemaname + "." + tablename + "!";
            }
        }else if(msgBox.clickedButton() == cancel)
            return;

        msgBox.removeButton(remove);
        msgBox.removeButton(cancel);
    }
}

//******************************************************************************************************************************************************//
//******************************************************************************************************************************************************//
//******************************************************************************************************************************************************//
//******************************************************************************************************************************************************//


void dbWorker::abiConnect(QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions, std::vector<QDomElement> list)
{
    for(unsigned int i = 0; i <  list.size(); i++)
    {
        if(list[i].attribute("name") == "abi")
        {
          abiDB = QSqlDatabase::addDatabase                  (driverName,"abi");  ///< Подключаемся к БД abi_db для логирования действий пользователей.
          abiDB.setHostName                                  (hostName);
          abiDB.setPort                                      (port);
          abiDB.setDatabaseName                              (dbName);
          abiDB.setPassword                                  (pwd);
          abiDB.setUserName                                  (userName);
          abiDB.setConnectOptions                            (connectOptions);
        if(abiDB.open())
        {
         qDebug() << "[Success] Подключение к abi произошло успешно. Данные будут логироваться в events_controllaccess!";
        }else
        {
         qDebug() << "[Error] Ошибка подключения к abi. Логирование в таблицу events_controllaccess невозможно!";
        }
          break;
        }
    }
}

void dbWorker::checkResourses(QComboBox *combobx, QAbstractItemDelegate *deleg, QTableView *table, QString usr, QString driverName, QString hostName, int port, QString dbName, QString pwd, QString userName, QString connectOptions)
{
    QSqlTableModel *model1;
    QSqlQueryModel *setquery = new QSqlQueryModel;

    QString dbname = combobx->currentText();
    QModelIndex myIndex;
    db = QSqlDatabase::addDatabase                  (driverName);
    db.setHostName                                  (hostName);
    db.setPort                                      (port);
    db.setDatabaseName                              (dbName);
    db.setPassword                                  (pwd);
    db.setUserName                                  (userName);
    db.setConnectOptions                            (connectOptions);
    db.open();


    setquery->setQuery("SELECT description AS \"Описание\", schemaname AS \"Имя схемы\", tablename AS \"Таблица\", Insert AS \"Событие вставки\", "
                       "update AS \"Событие обновления\", delete AS \"Событие удаления\", hard_control from chk.\"Table_state\" ORDER BY hard_control DESC");
    table->setModel(setquery);
    table->setModel(setquery);

    table->setItemDelegateForColumn            (3, deleg);
    table->setItemDelegateForColumn            (4, deleg);
    table->setItemDelegateForColumn            (5, deleg);
    table->setFocus();
    table->setColumnHidden(6, true);


   //фикс видимости чекбоксов
    for(int row=0; row < setquery->rowCount(); ++row)
    {
        table->openPersistentEditor        (setquery->index(row, 3));
        table->openPersistentEditor        (setquery->index(row, 4));
        table->openPersistentEditor        (setquery->index(row, 5));
    }

    if(db.isOpen()) ///< Проверяем события успешного подключения к БД
       {
          qDebug() << "[Information] Подключение к БД... \n[Success] Успешное подключение. Пользователь: " + usr + ". Подключение к БД: " + dbname + " IP: "+ hostName;
       }else{
          QMessageBox::critical(0, "Подключение к БД " + dbname, "Ошибка! Пожалуйста, проверьте правильность данных: /mnt/abi/settings/database.xml");
          qDebug() << "[Information] Подключение к БД... \n[Error] Ошибка подключения к БД. Пользователь: " + usr + ". Подключение к БД: " + dbname + " IP: "+ hostName;
       }

    if(table->model()->rowCount() == 0 && db.isOpen()) ///< Проверяем событие наличия таблицы Table_state и её пустоту.
    {
        QMessageBox::warning(0, "Отображение таблицы", "Таблица chk.Table_state либо пуста, либо не создана в БД " + dbname);
    }
}

void dbWorker::settingTriggers(QComboBox *cmb, QTableView *tbl, const int &state)
{
    query = new QSqlQuery(db);
    bool states         = (bool)state;
    int columns         = tbl->selectionModel()->currentIndex().column();

    QString schema      = tbl->selectionModel()->currentIndex().siblingAtColumn(1).data().toString();
    QString tablename   = tbl->selectionModel()->currentIndex().siblingAtColumn(2).data().toString();

    if(tbl->selectionModel()->currentIndex().row()+1 && columns == 3 && states == true) ///< Проверка состояния столбца insert
    {
        QString strInsert = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " ENABLE TRIGGER journal_i;";
        if(query->exec(strInsert)){
            loggingDBAccess(1, cmb, tbl);
            qDebug() << "[Success] Триггер insert для таблицы " + schema +"." + '"' + tablename + '"' + " включён! Запрос: " + strInsert;}
        else{
            QMessageBox::critical(0,"Ошибка", "Включение события вставки в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
            qDebug() << "[Error] Ошибка включения триггера insert для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strInsert;}

    }else if(tbl->selectionModel()->currentIndex().row()+1 && columns == 3 && states == false)
    {
        QString strInsert = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_i;";
        if(query->exec(strInsert)){
            loggingDBAccess(2, cmb, tbl);
            qDebug() << "[Success] Триггер insert для таблицы " + schema +"." + '"' + tablename + '"' + "выключен! Запрос: " + strInsert;}
        else{
            QMessageBox::critical(0,"Ошибка", "Выключение события вставки в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
            qDebug() << "[Error] Ошибка выключения триггера insert для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strInsert;}
    }

    if(tbl->selectionModel()->currentIndex().row()+1 && columns == 4 && states == true) ///< Проверка состояния столбца update
    {
       QString strUpdate = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " ENABLE TRIGGER journal_u;";
       if(query->exec(strUpdate)){
            loggingDBAccess(3, cmb, tbl);
           qDebug() << "[Success] Триггер update для таблицы " + schema +"." + '"' + tablename + '"' + "включен! Запрос: " + strUpdate;}
       else{
           QMessageBox::critical(0,"Ошибка", "Включение события обновления в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
           qDebug() << "[Error] Ошибка включения триггера update для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strUpdate;}

    }else if(tbl->selectionModel()->currentIndex().row()+1 && columns == 4 && states == false)
    {
        QString strUpdate = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_u;";
        if(query->exec(strUpdate)){
            loggingDBAccess(4, cmb, tbl);
            qDebug() << "[Success] Триггер update для таблицы " + schema +"." + '"' + tablename + '"' + "выключен! Запрос: " + strUpdate;}
        else{
            QMessageBox::critical(0,"Ошибка", "Выключение события обновления в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
            qDebug() << "[Error] Ошибка выключения триггера update для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strUpdate;}
    }

    if(tbl->selectionModel()->currentIndex().row()+1 && columns == 5 && states == true) ///< Проверка состояния столбца delete
    {
        QString strDelete = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " ENABLE TRIGGER journal_d;";
        if(query->exec(strDelete)){
            loggingDBAccess(5, cmb, tbl);
            qDebug() << "[Success] Триггер delete для таблицы " + schema +"." + '"' + tablename + '"' + "включен! Запрос: " + strDelete;}
        else{
            QMessageBox::critical(0,"Ошибка", "Включение события удаления в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
            qDebug() << "[Error] Ошибка включения триггера delete для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strDelete;}




    }else if(tbl->selectionModel()->currentIndex().row()+1 && columns == 5 && states == false)
    {
        QString strDelete = "ALTER TABLE "  + schema +"." + '"' + tablename + '"' + " DISABLE TRIGGER journal_d;";
        if(query->exec(strDelete)){
            loggingDBAccess(6, cmb, tbl);
        qDebug() << "[Success] Триггер delete для таблицы " + schema +"." + '"' + tablename + '"' + "выключен! Запрос: " + strDelete;}
        else{
            QMessageBox::critical(0,"Ошибка", "Выключение события удаления в " + schema +"." + '"' + tablename + '"' + " произошло с ошибкой!");
            qDebug() << "[Error] Ошибка выключения триггера delete для таблицы " + schema +"." + '"' + tablename + '"' + ". Запрос: " + strDelete;}
    }
}

void dbWorker::loggingDBAccess(int action, QComboBox *cmb, QTableView *tbl)
{
    QString basename = cmb->currentText();
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzzzzz");
    QString user = QDir::home().dirName();
    QString description = tbl->selectionModel()->currentIndex().siblingAtColumn(0).data().toString();
    QString schema      = tbl->selectionModel()->currentIndex().siblingAtColumn(1).data().toString();
    QString tablename   = tbl->selectionModel()->currentIndex().siblingAtColumn(2).data().toString();

    query = new QSqlQuery(abiDB);
    switch (action){
    case 1:
        query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
        query->bindValue(":blogin", user);
        query->bindValue(":btable", schema + "."+tablename);
        query->bindValue(":binfo", "Событие добавления контролируется");
        query->bindValue(":bdatetime", timestamp);
        query->bindValue(":bnamebase", basename);
        query->bindValue(":bdescription", description);
        if(!query->exec()){
            qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
        }
        break;
    case 2:
        query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
        query->bindValue(":blogin", user);
        query->bindValue(":btable", schema + "."+tablename);
        query->bindValue(":binfo", "Событие добавления не контролируется");
        query->bindValue(":bdatetime", timestamp);
        query->bindValue(":bnamebase", basename);
        query->bindValue(":bdescription", description);
        if(!query->exec()){
            qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
        }
        break;
    case 3:
       query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
       query->bindValue(":blogin", user);
       query->bindValue(":btable", schema + "."+tablename);
       query->bindValue(":binfo", "Событие обновления контролируется");
       query->bindValue(":bdatetime", timestamp);
       query->bindValue(":bnamebase", basename);
       query->bindValue(":bdescription", description);
       if(!query->exec()){
           qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
       }
        break;
    case 4:
        query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
        query->bindValue(":blogin", user);
        query->bindValue(":btable", schema + "."+tablename);
        query->bindValue(":binfo", "Событие обновления не контролируется");
        query->bindValue(":bdatetime", timestamp);
        query->bindValue(":bnamebase", basename);
        query->bindValue(":bdescription", description);
        if(!query->exec()){
            qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
        }
        break;
    case 5:
        query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
        query->bindValue(":blogin", user);
        query->bindValue(":btable", schema + "."+tablename);
        query->bindValue(":binfo", "Событие удаления контролируется");
        query->bindValue(":bdatetime", timestamp);
        query->bindValue(":bnamebase", basename);
        query->bindValue(":bdescription", description);
        if(!query->exec()){
            qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
        }
        break;
    case 6:
        query->prepare("INSERT INTO _abi.events_controllaccess (login, name_table, info, data_time, name_base, description) VALUES (:blogin, :btable, :binfo, :bdatetime, :bnamebase, :bdescription);");
        query->bindValue(":blogin", user);
        query->bindValue(":btable", schema + "."+tablename);
        query->bindValue(":binfo", "Событие удаления не контролируется");
        query->bindValue(":bdatetime", timestamp);
        query->bindValue(":bnamebase", basename);
        query->bindValue(":bdescription", description);
        if(!query->exec()){
            qDebug() << "[Success] Ошибка логирования в таблицу events_controlaccess!";
        }
        break;
    default:
        break;
    }
}



