#include "indextextlist.h"

IndexTextList::IndexTextList()
{

}

void IndexTextList::insert(QString text, QString index)
{
    textList << text;
    indexList << index;
    emit textInserted();
}
