#include "help.h"
#include "ui_help.h"

help::help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);

    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);

//    ui->listWidget->addItem("Простая выборка");

//    ui->listWidget->addItem("Сопоставление по нескольким столбцам");

//    ui->listWidget->addItem("Ручное сопоставление");

    //    ui->listWidget->addItem("При ошибке");
}

help::~help()
{
    delete ui;
}

void help::showTextfilesHelp()
{
    ui->textBrowser->append("<h3>Выборка кодов из текстовых файлов и таблиц:</h3>");
    ui->textBrowser->append("1) Открыть файл инвойса\n"
                            "2) Отметить колонку item\n"
                            "3) Отметить первую и последнюю ячейку количества\n"
                            "4) Нажать \"Анализировать\"\n"
                            "\t После этого в правом нижнем углу появится список контейнеров(если их несколько) и находящихся в них позиций\n"
                            "5) Далее надо открыть текстовые файлы или файл таблицы с QR кодами\n");
    ui->textBrowser->append("<h4>Работа с текстовыми файлами:</h4>");
    ui->textBrowser->append("Нажать \"Открыть тектовые\" после чего имена файлов появятся в списке справа, при нажатии на элемент списка выводится список кодов этого файла\n");
    ui->textBrowser->append("<h4>Работа с таблицами:</h4>");
    ui->textBrowser->append("1) Нажать \"Открыть Таблицу QR\"\n"
                            "2) Выбрать колонку QR\n"
                            "3) Выбрать колонку item\n"
                            "4) Нажать \"Анализивать\", после чего итем коды появятся в списке справа, при нажатии на элемент списка выводится список кодов этого итема");
    ui->textBrowser->append("<h4>Подготовка результата:</h4>");
    ui->textBrowser->append("Для подготовки результата надо выбрать итем из списка справа и нажать кнопку \"В коды\" или дважды кликнуть по элементу списка,\n"
                                "После чего в основной таблице отобразится содержимое результирующего файла.\n"
                                "Для сохранение результата надо нажать кнопку \"Сохранить\", выбрать путь для сохранения и нажать \"ok\"");

    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->textBrowser->setTextCursor(cursor);
}

void help::on_pushButton_clicked()
{
    this->close();
}

