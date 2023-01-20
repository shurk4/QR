#include "textfiles.h"
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

void textFiles::showTable(std::vector<std::vector<std::string> > *table)
{
    // ПОКАЗАТЬ ТАБЛИЦУ!!!
}

void textFiles::on_pushButtonOpen_clicked()
{
    QStringList fileNames;



        fileNames = QFileDialog::getOpenFileNames(nullptr, "Выбрать текстовые файлы", "Текстовые файлы", "*.txt");

        for (auto xfile : fileNames)
        {
            QFile file (xfile);
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);
            std::vector<std::string> codes;
            while (!in.atEnd())
            {
                      QString line = in.readLine();
                      codes.push_back(line.toStdString());
            }
            data.push_back(codes);
        }
}

