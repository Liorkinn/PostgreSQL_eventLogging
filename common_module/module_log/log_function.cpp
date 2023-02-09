#include "log_function.h"
#include "../declaration_path.h"
#include "QFile"
#include "qtextstream.h"
#include "variable_log.h"
#include <QDateTime>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

void messageToFile(QtMsgType type, const QMessageLogContext& Context, const QString& msg)
{

    QString FileName = variable_log::instance()->pathLogDebug();
    QFile fileAbiLogDebug(FileName);

    if (!fileAbiLogDebug.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&fileAbiLogDebug);
    switch (type) {
    case QtDebugMsg:
        out << msg << Context.file << endl;
        break;
    case QtWarningMsg:
        out << "Warning: " << msg << Context.file << endl;
        break;
    case QtCriticalMsg:
        out << "Critical: " << msg << ", " << Context.file << endl;
        break;
    case QtFatalMsg:
        out << "Fatal: " << msg << ", " << Context.file << endl;
        abort;
    }
}

void classLogFunction::WriteToLog(QString NameProg, int prClear)
{

    char command[100]; //формируемая строка для запуска команды, удаления предыдущего файла

    /*! Если программа запускается автономно, тогда производим очистку созданного файла
     * prClear = Module (0)  не чистим (которые вызываются как модули )
     * prClear = Prog (1)  очищаем (которые вызываются отделно : программа first_abi) */
    if (prClear == Prog) {
        //фоормируем строку на удаление предыдущего файла с логом
        sprintf(command, "sudo rm -f  %s", variable_log::instance()->pathLogDebug().toLocal8Bit().data());
        system(command); // Удаляем файл лога
    }

    /*! Переопределяем вывод команды Debug в файл лога, а не в консоль */
    qInstallMessageHandler(messageToFile);

    //Запишем в файл лога дату создания файла и программу, которая запустила  его
    QString strDateTime = QDateTime::currentDateTime().toString("dd.MM.yy -hh:mm");
    qDebug() << strDateTime << endl;
    if (prClear == Prog)
        qDebug() << "Запуск программы " << NameProg << endl;
    else
        qDebug() << "Запуск модуля " << NameProg << endl;
}

void classLogFunction::Read_Log_param()
{
    QString FileName; ///< Имя с Файлом настрйки
    char line[1024]; ///<  Переменные для считывания из файла
    char* lek; ///< Переменная
    unsigned int i = 0; ///< счетчик
    QList<QString> list; ///< Список необходимых параметров

    //! Открываем файл для чтения настроек который находится по дирректории PATH_SETTINGS файла declaration_path.h
    FileName = QString(PATH_SETTINGS);

    QFile fileSetting(FileName);

    if (!fileSetting.open(QFile::ReadOnly)) {
        qDebug() << QString("Не удалось открыть файл настроек").toUtf8() << endl;
        return;
    }

    //! Считываем и заполняем из этого файла необходимую информацию
    while (fileSetting.readLine(line, sizeof(line)) != -1) {
        if (line[0] == '#')
            continue;
        i = 0;
        lek = strtok(line, "\t");

        list.clear();
        while (lek != NULL) {
            list.append(QString(lek));
            lek = strtok(NULL, "\t");
            i++;
        }

        //******** Считываем данные о необходимости логирования ******
        //!     \b I.Считываем  \b секцию \b Логирование \b отладочной \b информации \b ("pathAbiDebug") */
        // qDebug() << "list = " << list;
        if (list[0] == "pathAbiDebug:") {
            // qDebug() << "list = " << list;
            QString Namef;
            Namef = list[1];

            //! Если указанный  в \b pathAbiDebug файл существует на файовой системе,
            //! тогда выставляем признак логирования отладочной информации в файл \b (PrDebug=1).
            //! Если указанный  в \b pathAbiDebug файл  НЕ существует на файловой стстеме,
            //! тогда выставляем логирования отладочной информации в файл \b (PrDebug=0)
            //! и вся отладочная информаци будет выводится в консоль.
            QFile fileLog(Namef);
            qDebug() << "Файл для логирования" << Namef << endl;
            if (fileLog.exists())
                variable_log ::instance()->setPrDebug(LogFile);
            else
                variable_log::instance()->setPrDebug(NoLog);
        }

        //! В переменную PathLogDebug считываем значение pathLogAbiDebug - полный путь к файлу
        //! для логирования информации
        if (list[0] == "pathLogAbiDebug:")
            variable_log::instance()->setPathLogDebug(QString(list[1]));
        //! За переключение логирования информации в файл отвечает общий модуль log_function.h
    }
    fileSetting.close();
}
