#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <QtCore>
#include <QString>
#include <QtXml>
#include <vector>
#include <QVector>

/*!
  \file
  \brief Класс парсинга xml файла database.xml
  \date Начало модернизации лето 2022 года
  */

class XmlParser
{

public:
    /*!
       * \brief ListElements - Функция записи элементов xml файла по тегу в std::vector
       * \param element - Параметр для получения элементов по тегу.
       * \param tagName - Тег xml файла.
     */
    std::vector<QDomElement> ListElements(QDomElement element, QString tagName);
    //QDomElement getByTagName();

    /*!
       * \brief open - Функция открытия xml файла.
       * \param path - Параметр пути файла.
    */
    bool open(QString path);
    /*!
       * \brief getRoot - Функция, возвращающая элементы.
    */
    QDomElement getRoot() const;
private:
    QDomElement root;
};
#endif // XMLPARSER_H
