#include "filemanager.h"
#include <QFile>
#include <QVector>
#include <QDebug>
#include <QTextDecoder>
#include <QTextCodec>
#include <QCoreApplication>

//работа будет проводиться с одним файлом,поэтому сразу запомним путь
//считывает файл и преобразует данные к удобному виду
//добавляет в конец файла новые данные

FileManager::FileManager()
{
    this->filePath = QCoreApplication::applicationDirPath() + "/file.txt";
    this->backupPath = QCoreApplication::applicationDirPath() + "/backup.txt";
}

QVector<QString> FileManager::readFile()
{
    QVector<QString> fileStrings;//содержит все строки файла
    //если файл file.txt существует,пытаемся его открыть
    if(QFile::exists(this->filePath))
    {
        // создаем объект класса QFile
        QFile file(this->filePath);
        //пытаемся открыть файл в режиме чтения
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
         {
             //устанавливаем кодировку,так как считываем мы массив байт и его надо как то кодировать
             QTextCodec *codec = QTextCodec::codecForName("UTF-8");
             QTextDecoder *decoder = codec->makeDecoder();
             while(!file.atEnd())
             {
                 QByteArray chunk = file.readLine();
                 //добавляем в вектор считанную строку. Кодируем байты в строку с помощью decoder
                 QString temp = decoder->toUnicode(chunk);
                 //simplified убирает \n \t \r
                 temp = temp.simplified();
                 fileStrings.push_back(temp);
             }
            file.close();
            delete decoder;
         }
         else
         {
             qDebug()<< "don't open file";
         }
    }
    else //если файл file не существует,пытаемся открыть файл backup.txt
    {

    }

    return fileStrings;
}

//если не удалось считать главный файл,то считываем бекап
QVector<QString> FileManager::readBackup()
{
    QVector<QString> fileStrings;
    //если файл существует
    if(QFile::exists(this->backupPath))
    {
        // создаем объект класса QFile
        QFile file(this->backupPath);
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            //устанавливаем кодировку,так как считываем мы массив байт и его надо как то кодировать
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QTextDecoder *decoder = codec->makeDecoder();
            //считываем весь файл до конца
            while(!file.atEnd())
            {
                QByteArray chunk = file.readLine();
                //добавляем в вектор считанную строку. Кодируем байты в строку с помощью decoder
                QString temp = decoder->toUnicode(chunk);
                //simplified убирает \n \t \r
                temp = temp.simplified();
                fileStrings.push_back(temp);
            }
           file.close();
           delete decoder;
        }
    }
    return fileStrings;
}

void FileManager::setWord(QString ruWord, QString nelusWord, QString comment, QString fontCode)
{
    QFile file(this->filePath); // создаем объект класса QFile
    //пытаемся открыть файл в режиме записи
    //Если такого файла не существует,он создается
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream writeStream(&file); // Создаем объект класса QTextStream
        writeStream.setCodec("UTF-8");//устанавливаем кодировку UTF-8,иначе будут кракозябры в файле
        //записываем по очереди данные,каждое на новую строку
        writeStream << ruWord + "\n" + nelusWord + "\n" + fontCode + "\n" + comment + "\n"; // Посылаем строку в поток для записи
        //закрываем файл
        file.close();
    }
    else
    {
        qDebug()<< "don't open file";
        return;
    }
    copyFile();
}

void FileManager::changeData(QString oldWord, QString newWord)
{
    QFile file(this->filePath); // создаем объект класса QFile
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
            return;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line == oldWord)
        {
            file.seek(file.pos() - line.length() - 1); // ещё единичка для перевода строки
            line.clear();
            line = newWord;
            in << line;  //хочу записать измененную строку в то же место откуда взял
        }
    }
    file.close();
    copyFile();
}

void FileManager::run()
{
    QFile file(this->filePath);
    //если такого файла нет,он создается
    if (file.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        QTextStream writeStream(&file); // Создаем объект класса QTextStream
        writeStream.setCodec("UTF-8");//устанавливаем кодировку UTF-8,иначе будут кракозябры в файле
        for(int i=0;i < this->vec2Write.size(); ++i)
        {
            //записываем по очереди данные,каждое на новую строку
            //без "\n",так как они итак уже содержатся в считанных ранее данных
            writeStream << vec2Write[i].getRusWord() + "\n" +
                           vec2Write[i].getNelusWord() + "\n" +
                           vec2Write[i].getFontCode() + "\n" +
                           vec2Write[i].getComment() + "\n"; // Посылаем строку в поток для записи
        }
        //закрываем файл
        file.close();
    }
    copyFile();
}

void FileManager::writeAllData(QVector<Container> vec2Write)
{
    this->vec2Write = vec2Write;
    run();
}

void FileManager::copyFile()
{
    QFile(this->backupPath).remove();
    QFile::copy(this->filePath, this->backupPath);
}
