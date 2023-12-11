#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QPainter>
#include <QFile>

enum Type
{
    MERGE_ITEMS
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Type type, QWidget *parent = nullptr);
    Dialog(Type type, QList<QListWidgetItem*> &selectedItems);

    void hideAll();

    ~Dialog();

private:
    Ui::Dialog *ui;
    void setStyle();

signals:
    void sendResultString(QString);

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonOk_clicked();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DIALOG_H
