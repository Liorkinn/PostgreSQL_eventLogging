#include "xmlparser.h"

std::vector<QDomElement> XmlParser::ListElements(QDomElement root, QString tagName)
{
    ///< Получаем элементы по тегу.
    QDomNodeList items = root.elementsByTagName(tagName);
    ///< Cоздаем массив, который будем заполнять.
    std::vector<QDomElement> itemList;
    ///< Добавляем элементы в itemList.
    for (int i = 0;i < items.count(); i++)
    {
        QDomNode itemNode = items.at(i);
        if (itemNode.isElement())
            itemList.push_back(itemNode.toElement());
    }
    return itemList;
}


bool XmlParser::open(QString path)
{
    QDomDocument document;
    QFile file(path);
    ///< Если файл не удалось открыть - программа вернет false.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    ///< Если XML-документ имеет ошибки - программа вернет false
    if (!document.setContent(&file))
        return false;

    ///< Освобождаем ресурсы.
    file.close();

    ///< Присваиваем первый элемент в переменную root.
    this->root = document.firstChildElement();
    ///< Возвращаем true.
    return true;
}

QDomElement XmlParser::getRoot() const
{
    return root;
}


