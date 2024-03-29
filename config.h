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
#include <QMap>

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

    void setSourcePath(QString const str);
    void setDestPath(QString const str);
    void setVersionPath(QString const str);

    void saveConfig();
    void save(QString path);

private:
    QSharedDataPointer<ConfigData> data;

    QVector<QString> settingsData;

    QString sourcePath = "empty";
    QString destPath = "empty";
    QString versionPath = "empty";
};

class UserConfig
{
     QMap<QString,QString> config;

public:
     UserConfig();
     ~UserConfig();

     void write();
     void read();

     void set(QString const name, QString const value);
     bool get(QString const name, QString &value);
};

#endif // CONFIG_H
