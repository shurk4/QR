#include "textfiles.h"
#include "qmessagebox.h"
#include "ui_textfiles.h"

textFiles::textFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textFiles)
{
    ui->setupUi(this);

    this->resize(1280, 720);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowSystemMenuHint);
}

textFiles::~textFiles()
{
    delete ui;
}

void textFiles::showTable(std::vector<std::vector<std::string> > &table)
{
    // Вывести данные в таблицу
        QString labelTabString = QString::number(currentDoc + 1);
        QString labelTabsString = QString::number(data.size());

        ui->labelCurrentDoc->setText(labelTabString);
        ui->labelDocsNum->setText(labelTabsString);
        ui->lineEditName->setText(QString::fromStdString(names[currentDoc]));

            int rows = table.size();
            int cols = table[0].size();

            ui->tableWidget->setRowCount(rows);
            ui->tableWidget->setColumnCount(cols);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                       << "D" << "E" << "F" << "G" << "H"
                                                       << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр

            for(int row = 0; row < table.size(); row++)
            {
                for(int col = 0; col < table[row].size(); col++)
                {
                    if(table[row].size() > cols) cols = table[row].size();
                    QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(table[row][col]));
                    ui->tableWidget->setItem(row, col, tbl);
                }
            }

            // Размеры
            ui->tableWidget->resizeRowsToContents();
            ui->tableWidget->resizeColumnsToContents();
            // Вывести данные в таблицу
}

void textFiles::showTable2(std::vector<std::vector<std::string> > &table)
{
    // Вывести данные в таблицу

            int rows = table.size();
            int cols = table[0].size();

            ui->tableWidget2->setRowCount(rows);
            ui->tableWidget2->setColumnCount(cols);

            ui->tableWidget2->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C"
                                                       << "D" << "E" << "F" << "G" << "H"
                                                       << "I" << "J" << "K" << "L" << "M"); // Имена столбцов вместо цифр

            for(int row = 0; row < table.size(); row++)
            {
                for(int col = 0; col < table[row].size(); col++)
                {
                    if(table[row].size() > cols) cols = table[row].size();
                    QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(table[row][col]));
                    ui->tableWidget2->setItem(row, col, tbl);
                }
            }

            // Размеры
            ui->tableWidget2->resizeRowsToContents();
            ui->tableWidget2->resizeColumnsToContents();
            // Вывести данные в таблицу
}

void textFiles::on_pushButtonOpen_clicked()
{
    QStringList fileNames;

    // Получение списка файлов
    fileNames = QFileDialog::getOpenFileNames(nullptr, "Выбрать текстовые файлы", "Текстовые файлы", "*.txt");

    // Получение данных из списка файлов
    for (auto xfile : fileNames)
    {
        QFile file (xfile);
        // Получение имени файла
        QFileInfo info(file.fileName());
        QString fileName(info.fileName());
        names.push_back(fileName.toStdString());

        // Загрузка данных из файла в поток
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);

        // Обработка данных
        std::vector<std::vector<std::string>> doc; // Данные из одного документа
        while (!in.atEnd())
        {
            std::vector<std::string> col;
            QString line = in.readLine();
            col.push_back(line.toStdString());
            doc.push_back(col);
        }
        data.push_back(doc);
    }

    showTable(data[currentDoc]);
}


void textFiles::on_pushButtonUp_clicked()
{
    if(currentDoc < data.size() - 1)
    {
        currentDoc++;
        showTable(data[currentDoc]);
    }
}


void textFiles::on_pushButtonDown_clicked()
{
    if(currentDoc > 0)
    {
        currentDoc--;
        showTable(data[currentDoc]);
    }
}


void textFiles::on_pushButtonItemCode_clicked()
{
    QString name = ui->lineEditName->text();
    for(int row = 0; row < data[currentDoc].size(); row++)
    {
        data[currentDoc][row].push_back(name.toStdString());
    }

    showTable(data[currentDoc]);
}


void textFiles::on_pushButtonCancel_clicked()
{
    this->close();
}


void textFiles::on_pushButtonAdd_clicked()
{
    for(int row = 0; row < data[currentDoc].size(); row++)
    {
        result.push_back(data[currentDoc][row]);
    }

    showTable2(result);
}

void textFiles::toTextFiles(std::vector<std::vector<std::string> > data)
{
    invoice = data;
    if(!invoice.empty())
    {
        showTable2(invoice);
    }
}


void textFiles::on_pushButtonOk_clicked()
{
    emit fromTextFiles(result);
    this->close();
}

