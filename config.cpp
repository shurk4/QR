#include "config.h"

class ConfigData : public QSharedData
{
public:

};

Config::Config() : data(new ConfigData)
{
    QFile inputFile("config.cfg");

        if (inputFile.open(QIODevice::ReadOnly))
        {
           QDataStream in(&inputFile);
           in >> config;
           inputFile.close();
        }
}

Config::Config(const Config &rhs)
    : data{rhs.data}
{

}

Config &Config::operator=(const Config &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Config::~Config(){}

bool Config::contains(QString key)
{
    return config.contains(key);
}

QString Config::getValue(QString key)
{
    return config[key];
}

void Config::setValue(QString key, QString value)
{
    config[key] = value;
}

void Config::saveConfig()
{
    QFile file("config.cfg");

     if (file.open(QIODevice::WriteOnly))
     {
        QDataStream in(&file);
        in << config;
        file.close();
     }
}
