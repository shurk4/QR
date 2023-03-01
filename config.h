#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

class ConfigData;

class Config
{
//    Q_OBJECT
public:
    Config();
    Config(const Config &);
    Config &operator=(const Config &);
    ~Config();

    QString getSourcePath();
    QString getDestPath();
    QString getVersionPath();

    void setSourcePath(QString str);
    void setDestPath(QString str);
    void setVersionPath(QString str);

    void saveConfig();
    void save(QString path);

private:
    QSharedDataPointer<ConfigData> data;

    QVector<QString> settingsData;

    QString sourcePath = "empty";
    QString destPath = "empty";
    QString versionPath = "empty";
};

#endif // CONFIG_H
