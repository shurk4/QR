#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Type type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint); // Убрать рамку

    setStyle();
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

void Dialog::setStyle()
{
    qDebug("setStyle");
    QFile styleF;
    styleF.setFileName("://res/dialogStyle.css");

    if(styleF.open(QFile::ReadOnly))
    {
        QString qssStr = styleF.readAll();
        this->setStyleSheet(qssStr);
        styleF.close();
        qDebug("txtFiles - Стили установлены");
    }
    else qDebug("Не удалось открыть файл стилей");
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

void Dialog::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
