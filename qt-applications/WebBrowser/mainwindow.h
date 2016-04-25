#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_urlLineEdit_returnPressed();

    void on_browserBackButton_clicked();

    void on_browserForwardButton_clicked();

    void on_browserStopButton_clicked();

    void on_browserRefreshButton_clicked();

    void on_webView_loadFinished(bool arg1);

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
