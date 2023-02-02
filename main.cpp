/*
    Если появятся ошибки надо вернуть дефайны win32, смотреть исходник examples.cpp

    Сделать проверку столбца итем кодов на наличие номеров к-ров и др
*/
#include "qrCodes.h"
#include <windows.h>
#include <sstream>
#include <string>
#include <locale>

#include "mainwindow.h"
#include <QApplication>

using namespace ExcelFormat;
//std::locale russian;

int main(int argc, char** argv)
{
    setlocale( LC_ALL,"Russian" );
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
