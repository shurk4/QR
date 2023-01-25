#ifndef TEXTFILES_H
#define TEXTFILES_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "qmessagebox.h"
#include "ui_textfiles.h"

enum FileStatus
{
    NEW,
    DELETED,
    ADDED
};

struct FileData
{
    std::string name;
    FileStatus status;
};

namespace Ui {
class textFiles;
}

class textFiles : public QDialog
{
    Q_OBJECT

public:
    explicit textFiles(QWidget *parent = nullptr);
    ~textFiles();

    void showTable(std::vector<std::vector<std::string>> &table);

    void showTable2(std::vector<std::vector<std::string>> &table);

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonItemCode_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButtonOk_clicked();

    void on_pushButtonShow_clicked();

private:
    Ui::textFiles *ui;

    int currentDoc = 0;

    std::vector<FileData> docsData;
    std::vector<std::vector<std::vector<std::string>>> data;
    std::vector<std::vector<std::string>> invoice;
    std::vector<std::vector<std::string>> result;

    bool invoiceShowed = false;

signals: // отправка данных
    void fromTextFiles(std::vector<std::vector<std::string>>);

public slots: // Приём данных
    void toTextFiles(std::vector<std::vector<std::string>> data);
};

#endif // TEXTFILES_H
