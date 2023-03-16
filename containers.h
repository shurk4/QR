#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "qdir.h"
#include "xlsConverter.h"
#include "extras.h"

#include "qdatetime.h"
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

struct ItemInfo
{
    QString ctnNumber;
    bool fullLoad = false;
    bool used = false;
};

namespace Ui {
class containers;
}

class containers : public QDialog
{
    Q_OBJECT

public:
    explicit containers(QWidget *parent = nullptr);
    ~containers();

    xlsConverter converter;

    void showTable_1(QVector<QVector<QString>> &inTab);

    void showTable_2(QVector<QVector<QString>> &inTab);

    void collectItemsCtn(QString item, ItemInfo itemInfo);

    std::string toSymbol(int in);

    bool emptyCell(QString &str);

    bool isCntNum(std::string str);

private slots:

    void on_pushButtonSpecOpen_clicked();

    void on_pushButtonInvOpen_clicked();

    void on_pushButtonSpecDown_clicked();

    void on_pushButtonSpecUp_clicked();

    void on_pushButtonInvDown_clicked();

    void on_pushButtonInvUp_clicked();

    void on_pushButtonSpecItemSelect_clicked();

    void on_pushButtonSpecAnalyze_clicked();

    void on_pushButtonResult_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButton_clicked();

private:
    Ui::containers *ui;

    QDir lastPath = QDir::homePath();
    QString fileName;
    int currentTab_1 = 0;
    int currentTab_2 = 0;

//    int serial = 0;
    int itemNum = 0;

    std::multimap<QString, ItemInfo> itemCtn;
};

#endif // CONTAINERS_H
