#ifndef TXTFILES_H
#define TXTFILES_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QDir>

#include "xlsConverter.h"
#include "extras.h"
#include "config.h"

enum FileStatus
{
    NEW,
    DELETED,
    ADDED
};

enum DisplayType
{
    TXT,
    INV,
    QR,
    RESULT
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

    void readConfig();
    void setDisplayType(DisplayType type);

    xlsConverter converter;
    void toCodes();

    void showTable_1(const QVector<QVector<QString>> &inTab);

    void showTable_2(const QVector<QVector<QString>> &table);
    void showDocs();

private slots:
    void on_pushButtonInv_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonTxt_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButtonAddQr_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonShow_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButtonOpenQr_clicked();

    void on_pushButtonFirstCell_clicked();

    void on_pushButtonLastCell_clicked();

    void on_pushButtonAnalyze_clicked();

    void on_pushButtonItemCol_clicked();

    void on_pushButtonQrCol_clicked();

    void on_pushButtonQrItemCol_clicked();

    void on_pushButtonQrAnalyze_clicked();

private:
    Ui::txtFiles *ui;

    UserConfig config;

    std::vector<FileData> docsInfo;
    QVector<QVector<QVector<QString>>> data;
    QVector<QVector<QString>> *items;

    QDir lastPath = QDir::homePath();
    DisplayType displayType;
    QString invoiceFileName;    
    QStringList fileNames;

    int currentTab = 0;
    int currentTabQr = 0;
    int currentDoc = 0;

    int serial = 0;
    int itemNum = 0;

    bool invoiceShowed = false;
};

#endif // TXTFILES_H
