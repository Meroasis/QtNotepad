#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length:" + QString::number(0) + "      lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln:" + QString::number(0) + "      Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("  梁光恒"));
    ui->statusbar->addPermanentWidget(author);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    findDialog dlg;
    dlg.exec();
}


void MainWindow::on_action_R_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();
}

