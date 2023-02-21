#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>

class ConfigData;

class Config
{
//    Q_OBJECT
public:
    Config();
    Config(const Config &);
    Config &operator=(const Config &);
    ~Config();

    bool contains(QString key);
    QString getValue(QString key);
    void setValue(QString key, QString value);

    void saveConfig();

private:
    QSharedDataPointer<ConfigData> data;
    QMap<QString, QString> config;
};

#endif // CONFIG_H
