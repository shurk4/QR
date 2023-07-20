#ifndef TABLESELECT_H
#define TABLESELECT_H

#include <QDialog>
#include <QFileDialog>
#include <QScreen>
#include "qrCodes.h"

namespace Ui {
class TableSelect;
}

class TableSelect : public QDialog
{
    Q_OBJECT

public:
    explicit TableSelect(QWidget *parent = nullptr);
    ~TableSelect();

private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonItemFirst_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButtonItemLast_clicked();

    void on_pushButtonQtyCol_clicked();

private:
    Ui::TableSelect *ui;

    std::string path;
    Invoice invoice;

signals:
    void signalInvoice(Invoice);
    void signalCancel(QString);
};

#endif // TABLESELECT_H
