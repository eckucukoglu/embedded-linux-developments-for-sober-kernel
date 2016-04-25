#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QtGui>
#include <QtCore>
#include "clickablelabel.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QStringList files;
    QDir folder;
    QStringList filters;
    QImage* img;
    ClickableLabel* cl;
    int index;

private slots:
    void onLabelClicked();
};

#endif // WIDGET_H
