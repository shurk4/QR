#include "version.h"

Version::Version(QString path)
{
    QFile inputFile(path);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QString currentData;
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          if(currentData == "version:")
          {
              v = in.readLine();
          }
          if(currentData == "info:")
          {
              info = in.readAll();
          }
          currentData = in.readLine();
       }
       inputFile.close();
    }
}

QString Version::getVersion()
{
    return v;
}

QString Version::getInfo()
{
    return info;
}

void Version::setVersion(const QString ver)
{
    v = ver;
}

void Version::setInfo(const QString inf)
{
    info = inf;
}

void Version::save(const QString path)
{
    QFile file(path);

     if (file.open(QIODevice::WriteOnly))
     {
        QTextStream in(&file);

        in << "version:\n" << v << "\n"
           << "info:\n" << info;
        file.close();
     }
}
