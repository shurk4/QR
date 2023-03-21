#include "config.h"
#include <iostream>

class ConfigData : public QSharedData
{
public:

};

Config::Config() : data(new ConfigData)
{
    QFile inputFile("config.cfg");

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QString currentData;
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          if(currentData == "source:")
          {
              sourcePath = in.readLine();
          }
          if(currentData == "dest:")
          {
              destPath = in.readLine();
          }
          if(currentData == "version:")
          {
              versionPath = in.readLine();
          }
          currentData = in.readLine();
       }
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

QString Config::getSourcePath()
{
    return sourcePath;
}

QString Config::getDestPath()
{
    return destPath;
}

QString Config::getVersionPath()
{
    return versionPath;
}

void Config::setSourcePath(QString str)
{
    sourcePath = str;
}

void Config::setDestPath(QString str)
{
    destPath = str;
}

void Config::setVersionPath(QString str)
{
    versionPath = str;
}

void Config::saveConfig()
{
    save(sourcePath);
    save(destPath);
}

void Config::save(QString path)
{
    path += "/config.cfg";
    QFile file(path);

     if (file.open(QIODevice::WriteOnly))
     {
        QTextStream in(&file);

        in << "source:\n" << sourcePath << "\n"
           << "dest:\n" << destPath << "\n"
           << "version:\n" << versionPath;
        file.close();
     }
     else
     {

     }
}

UserConfig::UserConfig()
{
    read();
}

UserConfig::~UserConfig()
{
    this->write();
}

void UserConfig::write()
{
    std::cout << "Try write file!\n";
    QFile file("user.cfg");
    QMap<QString,QString>::iterator it = config.begin();
    if (file.open(QIODevice::WriteOnly))
    {
       std::cout << "File is open!\n";
       QDataStream in(&file);
       in << config;
       std::cout << "Write complited!\n";
       file.close();
    }
    else
    {
        std::cout << "Can't write!\n";
    }
}

void UserConfig::read()
{
    std::cout << "Try ReadFile!\n";
    QFile file("user.cfg");
    if (file.open(QIODevice::ReadOnly))
    {
       std::cout << "File is open!\n";
       QDataStream in(&file);
       in >> config;
       std::cout << "Read complited!\n";
       file.close();
    }
    else
    {
        std::cout << "No file\n";
    }
}

void UserConfig::set(QString name, QString value)
{
    config[name] = value;
}

bool UserConfig::get(QString name, QString &value)
{
    if(config.contains(name))
    {
        value = config[name];
        return true;
    }

    return false;
}
