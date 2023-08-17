#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Type type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    switch (type) {
    case MERGE_ITEMS:


        break;
    default:
        break;
    }
}

Dialog::Dialog(Type type, QList<QListWidgetItem *> &selectedItems) : Dialog(type)
{
    for(auto &i : selectedItems)
    {
        QLabel *lbl = new QLabel(i->text());
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->layoutSelectedItems->addWidget(lbl);
    }
}

void Dialog::hideAll()
{
    ui->widgetMergeItems->hide();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void Dialog::on_pushButtonOk_clicked()
{
    if(ui->lineEditMergeItems->text().isEmpty())
    {
        QMessageBox::information(this, "", "Необходимо ввести имя!");
        return;
    }
    emit sendResultString(ui->lineEditMergeItems->text());
    this->close();
}
