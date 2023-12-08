#ifndef TXTFILES_H
#define TXTFILES_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QDir>
#include <QSettings>
#include <QMenu>
#include <QPainter>

#include "xlsConverter.h"
#include "extras.h"
#include "config.h"
#include "help.h"
#include "dialog.h"

enum DisplayType
{
    TXT,
    INV,
    QR,
    RESULT
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

    //Сохранение настроек в реестр \HKEY_CURRENT_USER\Software\ShurkSoft\QR to TKS\txt
    void readReg();
    void writeReg();

    xlsConverter converter;
    void toCodes();

    void showTable_1(const QVector<QVector<QString>> &inTab);
    void showTable_2(const QVector<QVector<QString>> &table);
    void showDocs();

    void showAutoButton();

    void toLog(QString _log);

private slots:
    void showContextMenu(QPoint pos);

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
    void on_pushButtonHelp_clicked();
    void on_pushButtonAuto_clicked();
    void on_pushButtonClearAll_clicked();
    void on_pushButtonClearFiles_clicked();
    void on_pushButtonClearItems_clicked();
    void on_pushButtonCancelUndoResult_clicked();
    void on_pushButtonUndoRedoResult_clicked();
    void on_pushButtonClearResult_clicked();
    void on_pushButtonMergeFiles_clicked();    
    void on_actionItemQtyFirstCell_triggered();
    void on_actionItemQtyLastCell_triggered();
    void on_actionInvQtyRange_triggered();
    void on_actionInvAnalize_triggered();
    void on_actionInvItemNameCol_triggered();

    void on_actionQrQrCol_triggered();

    void on_actionQrItemCol_triggered();

    void on_actionQrAnalize_triggered();

public slots:
    void hideFilesWidgets();
    // Результат диалогового окна объединения итемов
    void mergeItems(QString newName);
    void markItemsTable();

private:
    Ui::txtFiles *ui;

    void setStyle();

    UserConfig *config;
    help *helpWindow;

//    std::vector<FileData> docsInfo;
//    QVector<QVector<QVector<QString>>> data;
    QVector<QVector<QString>> items;

    // Для объединения итемов mergeItems
    QVector<int> selectedRows;

    QDir lastPath = QDir::homePath();
    DisplayType displayType;
    QStringList fileNames;

    int currentTab = 0;
    int currentTabQr = 0;
    int currentDoc = 0;

    int serial = 0;
    int itemNum = 0;

    bool invoiceShowed = false;

    // Конетекстные меню
    QMenu *invMenu;
    void createInvMenu();
    QMenu *qrMenu;
    void createQrMenu();

    // Действия
    void showResult();

    // invoice
    void invoiceFirstCell();
    void invoiceLastCell();
    void invoiceRangeCell();
    void invoiceItemCol();
    void invoiceAnalize();

    // qr
    void qrQrCol();
    void qrItemCol();
    void qrAnalize();

signals:
    void log(QString);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TXTFILES_H
