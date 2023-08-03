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
#include "config.h"
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
    void readConfig();
    void showTab(const QVector<QVector<QString>> &inTab);
    void showTabQr(const QVector<QVector<QString>> &inTab);

    int currentTab = 0;
    int currentTabQr = 0;
    int currentCompare = 0;

    QString invoiceFileName;
    QString qrFileName;
    QDir lastPath = QDir::homePath();

    bool configLoaded = false;
    bool showInvoice = false;
    bool showQr = false;
    bool analyzed = false;
    bool fullscreen = false;
    bool addedToInv = false; // Если привязки добавлялись вручную

    bool newCompare();
    bool filesOk();
    bool compared(const std::string a, const std::string b);
    bool itQty(const std::string str);
    bool tabAlreadyAdded(const int tab);
    bool fileIsUsed(const QString path);

    void qrNewFileClear();
    void qrClear();

private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonTabUp_clicked();

    void on_pushButtonTabDown_clicked();

    void on_pushButtonOpenQR_clicked();

    void on_pushButtonTabDownQR_clicked();

    void on_pushButtonTabUpQR_clicked();

    void on_pushButtonFirstQty_clicked();

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

    void on_pushButtonLastQty_clicked();

    void on_pushButtonScaleDown_clicked();

    void on_pushButtonScaleUp_clicked();

    void on_checkBoxFilesPath_stateChanged(int arg1);

    void on_lineEditQRLenght_textEdited(const QString &arg1);

private:
    Ui::qrDialog *ui;

    help *helpWindow;

    UserConfig *config = new UserConfig;

    std::ofstream file;

    std::vector<std::vector<int>> selectedQrCols;
    std::vector<Compare> compares;
    std::vector<int> addedQrTabs;

    QVector<QVector<QString>> tempInvoice;
    QVector<QVector<QString>> tempQr;
    QVector<QVector<QString>> notUsedQr;

    int qrCodeLenght = 31;
    int tableFontSize;

signals: // отправка данных
    void toTextFiles(std::vector<std::vector<std::string>>);

public slots: // Приём данных
    void fromTextFiles(QVector<QVector<QString>> data);
};

#endif // QRDIALOG_H
