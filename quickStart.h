#ifndef QUICKSTART_H
#define QUICKSTART_H

#include <QDialog>
#include <QFileDialog>

#include "TableSelect.h"
#include "qrCodes.h"

namespace Ui {
class QuickStart;
}

class QuickStart : public QDialog
{
    Q_OBJECT

public:
    Invoice invoice;
    QR qr;
    Result* readyToWrite;

    explicit QuickStart(QWidget *parent = nullptr);
    ~QuickStart();

private slots:

    void on_pushButtonSaveAs_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButtonStart_clicked();

    void on_pushButtonInvTable_clicked();

    void on_pushButtonQrTable_clicked();

private:
    Ui::QuickStart *ui;    

public slots:
    void slotInvoice(Invoice in);
    void slotQr(QR in);
    void slotCancel(QString in);
};

#endif // QUICKSTART_H
