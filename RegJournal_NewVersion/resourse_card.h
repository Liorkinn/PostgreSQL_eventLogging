#ifndef RESOURSE_CARD_H
#define RESOURSE_CARD_H

#include <QDialog>
//#include "dbworker.h"
namespace Ui {
class resourse_card;
}

class resourse_card : public QDialog
{
    Q_OBJECT

public:
    explicit resourse_card(QString textLabel, QString description,QWidget *parent = nullptr);
    ~resourse_card();
private slots:
    void on_pushButton_clicked();

private:
    Ui::resourse_card *ui;

signals:
    void setText(QString description_t, QString type_control_t, int s_num);

public:
    QString desc;
    QString type_oper;
    int sort;
};

#endif // RESOURSE_CARD_H
