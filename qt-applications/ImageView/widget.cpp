#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    folder = QDir::currentPath();
    folder.cd("images");

    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif" ;

    files = folder.entryList(filters, QDir::Files|QDir::NoDotAndDotDot);

    index = 0;
    cl = new ClickableLabel("", this);
    QObject::connect(cl, SIGNAL(clicked()), this, SLOT(onLabelClicked()));

    QString imgDir = folder.absolutePath() + "/" + files[index];
    qDebug() << imgDir;
    img = new QImage(imgDir);
    cl->setPixmap(QPixmap::fromImage(*img));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onLabelClicked(){
    files = folder.entryList(filters, QDir::Files|QDir::NoDotAndDotDot);

    index = (index+1) % files.size();
    QString imgDir = folder.absolutePath() + "/" + files[index];
    img = new QImage(imgDir);
    cl->setPixmap(QPixmap::fromImage(*img));
}
