#ifndef QRDIALOG_H
#define QRDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QScrollBar>
#include <iostream>
#include <fstream>

#include "help.h"
#include "ui_qrDialog.h"
#include "QFileDialog"
#include "QCheckBox"

#include "qdir.h"
#include "xlsConverter.h"
#include "help.h"
#include "extras.h"

struct Compare
{
    int invoiceCol;
    int qrCol;
};

namespace Ui {
class qrDialog;
}

class qrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qrDialog(QWidget *parent = nullptr);
    ~qrDialog();

    xlsConverter converter;
    void showTab(std::vector<std::vector<std::string>> &inTab);
    void showTabQr(std::vector<std::vector<std::string>> &inTab);

//    std::string toSymbol(int in);

    std::ofstream file;

    int currentTab = 0;
    int currentTabQr = 0;
    int currentCompare = 0;

    QString invoiceFileName;
    QString qrFileName;
    QDir lastPath = QDir::homePath();

    bool showInvoice = false;
    bool showQr = false;
    bool analyzed = false;
    bool fullscreen = false;
    bool addedToInv = false; // Если привязки добавлялись вручную

    bool newCompare();
    bool filesOk();
    bool compared(std::string a, std::string b);
    bool emptyCell(std::string &str);
    bool itQty(std::string &str);
    bool tabAlreadyAdded(int tab);

    std::vector<std::vector<int>> selectedQrCols;
    std::vector<Compare> compares;
    std::vector<int> addedQrTabs;

    std::vector<std::vector<std::string>> tempInvoice;
    std::vector<std::vector<std::string>> tempQr;
    std::vector<std::vector<std::string>> notUsedQr;

    void qrNewFileClear();
    void qrClear();

private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonTabUp_clicked();

    void on_pushButtonTabDown_clicked();

    void on_pushButtonOpenQR_clicked();

    void on_pushButtonTabDownQR_clicked();

    void on_pushButtonTabUpQR_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonFirstQty_clicked();

    void on_pushButtonBasicSize_clicked();

    void on_pushButtonColQR_clicked();

    void on_pushButtonAnalyzeInvoice_clicked();

    void on_pushButtonResetInvoice_clicked();

    void on_pushButtonShowInvoice_clicked();

    void on_pushButton_clicked();

    void on_pushButtonAddColsQR_clicked();

    void on_pushButtonAddToQrResult_clicked();

    void on_pushButtonShowQR_clicked();

    void on_pushButtonResetQR_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonShowResult_clicked();

    void on_pushButtonClearResult_clicked();

    void on_pushButtonShowNotUsedCodes_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButtonNew_clicked();

    void on_pushButtonArticulesQr_clicked();

    void on_pushButtonArticulesQrReset_clicked();

    void on_pushButtonAddToInv_clicked();

    void on_pushButtonHelp_clicked();

    void on_pushButtonLastQty_clicked();

private:
    Ui::qrDialog *ui;

    help *helpWindow;

signals: // отправка данных
    void toTextFiles(std::vector<std::vector<std::string>>);

public slots: // Приём данных
    void fromTextFiles(std::vector<std::vector<std::string>> data);
};

#endif // QRDIALOG_H
