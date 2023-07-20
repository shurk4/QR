#ifndef QRTABLE_H
#define QRTABLE_H

#include <QDialog>
#include <QFileDialog>
#include "qrCodes.h"

namespace Ui {
class QrTable;
}

class QrTable : public QDialog
{
    Q_OBJECT

public:
    explicit QrTable(QWidget *parent = nullptr);
    ~QrTable();

private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonQrCol_clicked();

    void on_pushButtonItemCol_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButtonFirstRow_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::QrTable *ui;

    std::string path;
    QR qr;

signals:
    void signalQr(QR);
};

#endif // QRTABLE_H
