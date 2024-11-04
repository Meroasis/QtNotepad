#include "finddialog.h"
#include "ui_finddialog.h"

findDialog::findDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::findDialog)
{
    ui->setupUi(this);
}

findDialog::~findDialog()
{
    delete ui;
}
