#ifndef TEXTFILES_H
#define TEXTFILES_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>

namespace Ui {
class textFiles;
}

class textFiles : public QDialog
{
    Q_OBJECT

public:
    explicit textFiles(QWidget *parent = nullptr);
    ~textFiles();

    void showTable(std::vector<std::vector<std::string>> &table);

    void showTable2(std::vector<std::vector<std::string>> &table);

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonItemCode_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::textFiles *ui;

    int currentDoc = 0;

    std::vector<std::string> names;
    std::vector<std::vector<std::vector<std::string>>> data;
    std::vector<std::vector<std::string>> invoice;
    std::vector<std::vector<std::string>> result;

signals: // отправка данных
    void fromTextFiles(std::vector<std::vector<std::string>>);

public slots: // Приём данных
    void toTextFiles(std::vector<std::vector<std::string>> data);
};

#endif // TEXTFILES_H
