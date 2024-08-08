#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H

#include <QListWidgetItem>

class CustomList : public QListWidgetItem
{
public:
    CustomList(QString name);
};

#endif // CUSTOMLIST_H
