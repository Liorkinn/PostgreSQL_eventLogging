#include "resourse_card.h"
#include "ui_resourse_card.h"
#include <QMessageBox>
#include <QDebug>

resourse_card::resourse_card(QString textLabel, QString description, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resourse_card)
{
    ui->setupUi(this);
   // dbWorker *dbwork = new dbWorker();
    ui->comboBox_control->addItem("Обычный", QVariant(0));
    ui->comboBox_control->addItem("Жесткий", QVariant(1));
    ui->lineEdit_sort->setInputMask("0000");
    ui->lineEdit_descr->setText(description);
    ui->label_4->setText(textLabel);
    qDebug() << description;
  //  connect(this, &resourse_card::setText, dbwork, &dbWorker::getText);
}

resourse_card::~resourse_card()
{
    delete ui;
}



void resourse_card::on_pushButton_clicked()
{

    desc = ui->lineEdit_descr->text();
    type_oper = ui->comboBox_control->currentText();
    sort = ui->lineEdit_sort->text().toInt();
    if(ui->lineEdit_sort->text() == 0) {
        QMessageBox::warning(0, "Предупреждение!", "Поле сортировки не может быть пустым!");
    } else {
         QDialog::close();
    }

}
