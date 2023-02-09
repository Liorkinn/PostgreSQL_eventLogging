#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

/*!
  \file
  \brief Класс, предназначенный для работы с делегатом чекбоксов
  \date Начало модернизации лето 2022 года
  */

#include <QStyledItemDelegate>
#include <QTableView>
/*!
 * \brief Класс, предназначенный для работы с делегатом элемента выбора(активно/не активно)
 */
class checkboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    checkboxDelegate                (QObject *parent=nullptr);
    /*!
       * \brief createEditor - Функция создания делегата.
       * \param *parent - Параметр, задающий родителя.
       * \param &option - Параметр, задающий отрисовку.
       * \param &index - Параметр индекса модели.
     */
    QWidget *createEditor           (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /*!
       * \brief setEditorData - Функция, обеспечивающая взятие данных ячейки из модели таблицы и передачу чекбоксу
       * \param *editor - Форма приложения
       * \param &index - Параметр индекса модели.
    */
      void setEditorData            (QWidget *editor, const QModelIndex &index) const;
    /*!
       * \brief setModelData - Функция, обеспечивающая взятие данных из чекбокса и передачу в модель таблицы.
       * \param *editor - Параметр от основного окна.
       * \param *model - Пераметр, задающий модель.
       * \param *index - Параметр индекса модели.
    */
      void setModelData             (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    /*!
       * \brief updateEditorGeometry - Функция, определяющая геометрию элемента выбора.
       * \param *editor - Параметр от основного окна.
       * \param &option - Параметр, задающий отрисовку.
       * \param &index - Параметр индекса модели.
     */
      void updateEditorGeometry     (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /*!
       * \brief paint - Функция, скрывающая текст в ячейках-делегатах элемента выбора.
       * \param *painter - Параметр рисовки для ячеек.
       * \param &option - Параметр, задающий отрисовку.
       * \param &index - Параметр индекса модели.
     */
      void paint                    (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    /*!
       * \brief _change - Сигнал, отвечающий за определение состояния элемента выбора.
       * \param int& - Параметр состояния элемента выбора.
     */
    void _change  (const int&);

};

#endif // CHECKBOXDELEGATE_H
