#ifndef HELP_H
#define HELP_H

#include <QDialog>

enum HelpType
{
    TXTFILES
};

namespace Ui {
class help;
}

class help : public QDialog
{
    Q_OBJECT

public:
    help(QWidget *parent = nullptr);
    ~help();

    void showTextfilesHelp();

private slots:
    void on_pushButton_clicked();

private:
    Ui::help *ui;
};

#endif // HELP_H
