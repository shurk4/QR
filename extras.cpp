#include "extras.h"

Extras::Extras()
{

}

QVector<QVector<QString>> Extras::toQvecConvert(const std::vector<std::vector<std::string>> &data)
{
    QVector<QVector<QString>> qData;

    for(int row = 0; row < data.size(); row++)
    {
        QVector<QString> tempRow;

        for(int col = 0; col < data[row].size(); col++)
        {
            tempRow.push_back(QString::fromStdString(data[row][col]));
        }

        qData.push_back(tempRow);
    }

    return qData;
}

void Extras::showTable(const QVector<QVector<QString>> &data, QTableWidget *table)
{
    table->clear();
    if(data.empty()) return;

    int rows;

    if(data.size() > 10000)
    {
        rows = 10000;
    }
    else
    {
        rows = data.size();
    }

    table->setRowCount(rows);
    table->setColumnCount(data[0].size());
    table->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G"); // Имена столбцов вместо цифр

    for(int row = 0; row < data.size(); row++)
    {
        if(data[row].size() > table->columnCount())
        {
            table->setColumnCount(data[row].size());
        }
        for(int col = 0; col < data[row].size(); col++)
        {
            QTableWidgetItem *tbl = new QTableWidgetItem(data[row][col]);

            table->setItem(row, col, tbl);
        }
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void Extras::setTableSettings(QVector<QVector<CellSettings>> settings, QTableWidget *table)
{
    qDebug() << "------Extras::setTableSetting settings.size(): " << settings.size() << " settings[0].size(): " << settings[0].size();
    for(int row = 0; row < settings.size(); row++)
    {
        for(int col = 0; col < settings.size(); col++)
        {
//            if(settings[row][col].mergedRows > 0 && settings[row][col].mergedRows < settings.size() && settings[row][col].mergedCols > 0 && settings[row][col].mergedCols < settings[row].size())
//            {
                if(table->rowSpan(row, col) < 2 && table->columnSpan(row, col < 2))
                {
                    table->setSpan(row, col, row + settings[row][col].mergedRows, col + settings[row][col].mergedCols);
                    qDebug() << "Extras::setTableSettings: " << row << " " << col << " settings[row][col].mergedRows: " << settings[row][col].mergedRows
                             << "settings[row][col].mergedCols: " << settings[row][col].mergedCols;
                    qDebug() << "Extras::setTableSettings: " << row << " " << col << " table->rowSpan(row, col): " << table->rowSpan(row, col)
                             << " table->columnSpan: " << table->rowSpan(row, col);
                }
//            }
        }
    }
//        table->resizeColumnsToContents();
//        table->resizeRowsToContents();
}

void Extras::scaleTable(const int size, QTableWidget *table)
{
    QFont font;
    font.setPointSize(size);

    table->setFont(font);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

std::string Extras::IntToSymbol(const int in)
{
    std::string str;
        str += ('A' + in);
    return str;
}

bool Extras::itQty(const std::string str)
{
    if(Extras::emptyCell(QString::fromStdString(str))) return false;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool Extras::isCtnNumber(QString str)
{
    if(str.size() != 11) return false;

    QRegExp regx("[A-Z]{4}[0-9]{7}");
    if(regx.exactMatch(str)) return true;
    return false;
}

bool Extras::emptyCell(const QString &str)
{
        return str.size() == 0 || str[0] == '\0';
}

void Extras::readConfig()
{

}
