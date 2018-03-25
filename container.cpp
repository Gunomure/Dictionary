#include "container.h"

Container::Container(QString rusWord, QString nelusWord, QString fontCode, QString comment)
{
    this->rusWord = rusWord;
    this->nelusWord = nelusWord;
    this->fontCode = fontCode;
    this->comment = comment;
}

Container::Container()
{
    this->rusWord = "";
    this->nelusWord = "";
    this->fontCode = "";
    this->comment = "";
}

void Container::setRusWord(QString rusWord)
{
    this->rusWord = rusWord;
}

void Container::setNelusWord(QString nelusWord)
{
    this->nelusWord = nelusWord;
}

void Container::setFontCode(QString fontCode)
{
    this->fontCode = fontCode;
}

void Container::setComment(QString comment)
{
    this->comment = comment;
}

QString Container::getRusWord()
{
    return this->rusWord;
}

QString Container::getNelusWord()
{
    return this->nelusWord;
}

QString Container::getFontCode()
{
    return this->fontCode;
}

QString Container::getComment()
{
    return this->comment;
}

const bool Container::operator <(const Container& other) const
{
    return QString::compare(rusWord, other.rusWord) < 0 ? true : false;
}
