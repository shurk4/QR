#ifndef TXTFILES_H
#define TXTFILES_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QDir>

#include "xlsConverter.h"

enum FileStatus
{
    NEW,
    DELETED,
    ADDED
};

struct FileData
{
    QString name;
    FileStatus status;
};

namespace Ui {
class txtFiles;
}

class txtFiles : public QDialog
{
    Q_OBJECT

public:
    explicit txtFiles(QWidget *parent = nullptr);
    ~txtFiles();

    xlsConverter converter;

    void showTable_1(std::vector<std::vector<std::string>> &inTab);

    void showTable_2(std::vector<std::vector<std::string>> &table);

private slots:
    void on_pushButtonInv_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonTxt_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButtonAddQr_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonShow_clicked();

private:
    Ui::txtFiles *ui;

    std::vector<FileData> docsData;
    std::vector<std::vector<std::vector<std::string>>> data;

    QDir lastPath = QDir::homePath();
    QString invoiceFileName;
    int currentTab = 0;
    int currentDoc = 0;

    int serial = 0;
    int itemNum = 0;
//    std::vector<int> numGroup; // Если понадобится добавлять коды в уже добавленому товару

    bool invoiceShowed = false;
};

#endif // TXTFILES_H
