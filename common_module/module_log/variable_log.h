#ifndef VARIABLE_LOG_H
#define VARIABLE_LOG_H
#include "QString"

/*!
 * \file
 * \brief Класс содержит переменные-синглетоны необходимые для процедур переключения логирования в файл
 * \date Декабрь 2021
 *
 * \details В классе содержатся переменные, которые заполняются при считывании файла настроек - abi_settings
 *  и используются в общем модуле log_finction.h, а токже в процессе работы программы
 */

/*!
 * \brief The PrLog enum - Перечень признаков логирования
 */
enum PrLog {
    LogFile = 1, ///< Значение логирование в файл
    NoLog = 0 ///< Не логировать в файл
};

/*!
 * \brief The PrClFile enum - Перечень признаков работы программы
 */
enum PrClFile {
    Module = 0, ///<  Программа запускается как модуль, очистка файла не требуется
    Prog = 1 ///<  Программа запускается как отдельная программа, требуеься очистка предыдущего файла
};

/*!
 * \brief Класс переменных, которые используются при логировании
 */
class variable_log {
public:
    static variable_log* instance();
    /*!
     * \brief Функция, возвращающая значение  признака подробного логирования
     * \return LogFile -  логировать в файл   NoLog - не логировать
     */
    int prDebug() { return prDebug_; }
    /*!
     * \brief Функция, устанавливающая значение  для признака подробного логирования
     * \param prDebug - устанавливаемое значение
     */
    void setPrDebug(int prDebug) { prDebug_ = prDebug; }

    /*!
     * \brief Функция, возвращающая имя файла для логирования
     * \return имя файла
     */
    QString pathLogDebug() { return pathLogDebug_; }
    /*!
     * \brief Функция, устанавливающая имя файла для логирования
     * \param pathLogDebug - полный путь к имени файла
     */
    void setPathLogDebug(QString pathLogDebug) { pathLogDebug_ = pathLogDebug; }

    /*!
     * \brief Функция, возвращающая признак очистки  файла для логирования
     */
    int prCrFile() { return prCrFile_; }
    /*!
     * \brief Функция, устанавливающая признак очистки файла для логирования
     * \param prCrFile - признак очистки  файла
     */
    void setPrCrFile(int prCrFile) { prCrFile_ = prCrFile; }

private:
    variable_log();
    ~variable_log();
    static variable_log* variable_log_;
    int prDebug_; ///<  Признак подробного логирования (1 - подробно логировать; 0 - не логировать)
    QString pathLogDebug_; ///< Путь к файлу, который включает подробное логирование
    int prCrFile_; ///< Признак очистки файла логирования
};

#endif // VARIABLE_LOG_H
