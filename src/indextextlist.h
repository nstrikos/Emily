#ifndef INDEXTEXTLIST_H
#define INDEXTEXTLIST_H

#include <QStringList>

class IndexTextList : public QObject
{
    Q_OBJECT

public:
    IndexTextList();
    void insert(QString text, QString index);

public:
    QStringList indexList;
    QStringList textList;

signals:
    void textInserted();
};

#endif // INDEXTEXTLIST_H
