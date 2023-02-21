#ifndef VERSION_H
#define VERSION_H
#include <QString>
#include <QFile>
#include <QTextStream>

class Version
{
    QString v;
    QString info;
public:
    Version(QString path);

    QString getVersion();
    QString getInfo();
    void setVersion(QString ver);
    void setInfo(QString inf);
    void save(QString path);
};

#endif // VERSION_H
