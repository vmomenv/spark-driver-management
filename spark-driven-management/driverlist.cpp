#include "driverlist.h"

#include <QLabel>

driverList::driverList(QWidget *parent) : QWidget(parent)
{
    QWidget *List =new QWidget(this);
    QLabel *label=new QLabel(List);
    label->setText("111");

}
