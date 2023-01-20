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

    void showTable(std::vector<std::vector<std::string>> *table);

private slots:
    void on_pushButtonOpen_clicked();
    std::vector<std::vector<std::string>> data;

private:
    Ui::textFiles *ui;
};

#endif // TEXTFILES_H
