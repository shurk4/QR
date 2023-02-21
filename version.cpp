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

void Version::setVersion(QString ver)
{
    v = ver;
}

void Version::setInfo(QString inf)
{
    info = inf;
}

void Version::save(QString path)
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
