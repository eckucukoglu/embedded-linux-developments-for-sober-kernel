#include "mainwindow.h"
#include "ui_mainwindow.h"

//----------------------------------------
// Author:
// Alican Buyukcakir 2015
//
//----------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_urlLineEdit_returnPressed()
{
    ui->webView->setUrl(QUrl::fromUserInput(ui->urlLineEdit->text()));
}

void MainWindow::on_browserBackButton_clicked()
{
    ui->webView->back();
}

void MainWindow::on_browserForwardButton_clicked()
{
    ui->webView->forward();
}

void MainWindow::on_browserStopButton_clicked()
{
    ui->webView->stop();
}

void MainWindow::on_browserRefreshButton_clicked()
{
    ui->webView->reload();
}

void MainWindow::on_webView_loadFinished(bool arg1)
{
    ui->urlLineEdit->setText(ui->webView->url().toString());
    this->setWindowTitle(ui->webView->title());
}

void MainWindow::on_zoomInButton_clicked()
{
    ui->webView->setZoomFactor(ui->webView->zoomFactor() * 1.1);
}

void MainWindow::on_zoomOutButton_clicked()
{
    ui->webView->setZoomFactor(ui->webView->zoomFactor() * 0.9);
}
