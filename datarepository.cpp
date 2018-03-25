#include "datarepository.h"
#include <QVector>
#include <QString>
#include <QtAlgorithms>
#include <QCoreApplication>

DataRepository::DataRepository()
{
    //applicationDirPath возвращает путь,где лежти программа. Плюс думаю не зависит от платформы
    this->fileManager = new FileManager();
    //считываем в вектор данные из файла
    QVector<QString> fileStrings = this->fileManager->readFile();
    //собираем вектор нужных структур
    for(int i=0;i < fileStrings.length(); i += 4)
    {
        addContainer(fileStrings[i], fileStrings[i+1], fileStrings[i+2], fileStrings[i+3]);
    }
    //сортируем вектор контейнеров по алфавиту по русским словам
    sort();
}

void DataRepository::sort()
{
    //в классе Container определен оператор "<",поэтому не нужно писать лямбда выражение
    qSort(this->containers.begin(), this->containers.end());
}

void DataRepository::addContainer(QString rusWord, QString nelusWord, QString fontCode, QString comment)
{
    //добавляем в вектор контейнеров новый контейнер с заданными параметрами
    this->containers.push_back(Container(rusWord, nelusWord, fontCode, comment));
}

QVector<Container> DataRepository::getContainers()
{
    return this->containers;
}

QVector<Container> DataRepository::filtration(QString subString, bool category)
{
    QVector<Container> filtered;
    /*
     * indexof возвращает номер позиции первого вхождения подстроки в строку.
     * Мне нужно отфильтровать так,чтобы вхождение было в самом начале,поэтому сравниваю с 0
     * Qt::CaseInsensitive значит не учитывать регистр слов
     * В зависимости от категории фильтра,фильтрую либо по русским словам,либо по нелусу
     */
    if(category)
    {
        for(int i=0;i < this->containers.size(); ++i)
        {
            if(this->containers[i].getRusWord().indexOf(subString, 0, Qt::CaseInsensitive) == 0)
            {
                filtered.push_back(this->containers[i]);
            }
        }
    }
    else
    {
        for(int i=0;i < this->containers.size(); ++i)
        {
            if(this->containers[i].getNelusWord().indexOf(subString, 0, Qt::CaseInsensitive) == 0)
            {
                filtered.push_back(this->containers[i]);
            }
        }
    }
    return filtered;
}

void DataRepository::SetNewData(QString oldWord, QString newdWord)
{
    this->fileManager->changeData(oldWord, newdWord);
}

void DataRepository::writeAllData()
{
    this->fileManager->writeAllData(this->containers);
}

void DataRepository::deleteContainer(Container container2delete)
{
    for(int i=0;i < this->containers.size(); ++i)
    {
        if(
           this->containers[i].getRusWord() == container2delete.getRusWord() &&
           this->containers[i].getNelusWord() == container2delete.getNelusWord() &&
           this->containers[i].getFontCode() == container2delete.getFontCode() &&
           this->containers[i].getComment() == container2delete.getComment()
          )
        {
            this->containers.remove(i);
            break;
        }
    }
    //переписываем файл
    writeAllData();
}

void DataRepository::writeManyString()
{
    for(int i=0;i < 1000; ++i)
    {
        this->fileManager->setWord("testData" + QString::number(i),
                                   "testData" + QString::number(i),
                                   "testData" + QString::number(i),
                                   "testData" + QString::number(i));
    }
}

void DataRepository::writeContainer(Container container)
{
        addContainer(container.getRusWord(),
                      container.getNelusWord(),
                      container.getFontCode(),
                      container.getComment());
        sort();
        this->fileManager->setWord(container.getRusWord(),
                                   container.getNelusWord(),
                                   container.getFontCode(),
                                   container.getComment());
}

void DataRepository::changeData(Container oldContainer, Container newContainer)
{
    for(int i=0;i < this->containers.size(); ++i)
    {
        if(
           this->containers[i].getRusWord() == oldContainer.getRusWord() &&
           this->containers[i].getNelusWord() == oldContainer.getNelusWord() &&
           this->containers[i].getFontCode() == oldContainer.getFontCode() &&
           this->containers[i].getComment() == oldContainer.getComment()
          )
        {
            this->containers.replace(i, newContainer);
            break;
        }
    }
    //переписываем файл
    writeAllData();
}
