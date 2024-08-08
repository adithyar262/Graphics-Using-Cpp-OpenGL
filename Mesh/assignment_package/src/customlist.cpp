#include "customlist.h"

CustomList::CustomList(QString name)
    : QListWidgetItem()
{
    setText(name);
}
