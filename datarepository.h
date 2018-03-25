#ifndef DATAREPOSITORY_H
#define DATAREPOSITORY_H
#include <QMultiMap>
#include <QVector>
#include <QString>
#include "container.h"
#include "filemanager.h"

class DataRepository
{
private:
    QVector<Container> containers;
    FileManager *fileManager;
public:
    DataRepository();
    void sort();
    void addContainer(QString rusWord, QString nelusWord, QString fontCode, QString comment);
    QVector<Container> getContainers();
    /*
     *выдает отфильтрованные слова по subString
     * если category = true, фильтрация по русским словам
     * если category = false, фильрация по словам из нелуса
     */
    QVector<Container> filtration(QString subString, bool category);
    void SetNewData(QString oldWord, QString newdWord);
    void writeAllData();
    void deleteContainer(Container container2delete);
    void writeManyString();
    void writeContainer(Container container);
    void changeData(Container oldContainer, Container newContainer);
};

#endif // DATAREPOSITORY_H
