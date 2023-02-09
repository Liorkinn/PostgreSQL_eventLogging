#include "checkboxdelegate.h"
#include <QCheckBox>
#include <QPainter>

checkboxDelegate::checkboxDelegate(QObject *parent)  :QStyledItemDelegate(parent)
{
}


QWidget* checkboxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const
{
    ///< Создаём элемент выбора(чекбокс), и получаем его состояние за счёт сигнала.
    QCheckBox *checkBox = new QCheckBox(parent);
    connect(checkBox, &QCheckBox::stateChanged,  this, &checkboxDelegate:: _change);
    return checkBox;
}


void checkboxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    ///< Берем данные ячейки из модели таблицы и сообщаем элементу выбора(чекбоксу)
    bool state = index.model()->data(index, Qt::EditRole).toBool();
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setChecked(state);
}


void checkboxDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    ///< Берем данные из элемента выбора(чекбокса) и передаем в модель таблицы
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    bool state = checkBox->isChecked();
    model->setData(index, state, Qt::EditRole);
}


void checkboxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{

        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);

        QStyleOptionButton opt;
        opt.initFrom(checkBox);
        QRect indicatorRect=checkBox->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);

        int editorWidth=indicatorRect.width();
        editor->setGeometry(option.rect.adjusted(option.rect.width()/2-editorWidth/2,0,
                                                 option.rect.width()/2-editorWidth/2,0));
    if(index.siblingAtColumn(6).data() == "true") {
        checkBox->setVisible(false);
    }
}

void checkboxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ///< Запрещаем прорисовку текста.
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);
}





