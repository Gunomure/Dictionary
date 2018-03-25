#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QString>
#include <QThread>
#include <QVector>
#include "container.h"

class FileManager: public QThread
{
private:
    QString filePath, backupPath;
    QVector<Container> vec2Write;
public:
    FileManager();
    QVector<QString> readFile();
    QVector<QString> readBackup();
    //записывает в конец файла 4 слова,каждое на новую строку
    void setWord(QString ruWord, QString nelusWord, QString comment, QString fontCode);
    //находит в файле нужное слово и меняет его на новое
    void changeData(QString oldWord, QString newdWord);
    void run();
    void writeAllData(QVector<Container> vec2Write);
    void copyFile();
};

#endif // FILEMANAGER_H
