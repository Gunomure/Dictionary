#ifndef CONTAINER_H
#define CONTAINER_H
#include <QString>

class Container
{
private:
    QString rusWord;
    QString nelusWord;
    QString fontCode;
    QString comment;
public:
    Container(QString rusWord, QString nelusWord, QString fontCode, QString comment);
    Container();
    void setRusWord(QString rusWord);
    void setNelusWord(QString nelusWord);
    void setFontCode(QString fontCode);
    void setComment(QString comment);

    QString getRusWord();
    QString getNelusWord();
    QString getFontCode();
    QString getComment();
    const bool operator <(const Container &other) const;
};

#endif // CONTAINER_H
