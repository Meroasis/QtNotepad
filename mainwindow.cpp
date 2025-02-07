#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextCursor>
#include <QTextDocumentFragment>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChanged = false;
    on_actionNew_triggered();
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length:" + QString::number(0) + "      lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln:" + QString::number(0) + "      Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("  梁光恒"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode =ui->TextEdit->lineWrapMode();

    if( mode == QTextEdit::NoWrap ){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWarp->setChecked(false);
    }
    else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWarp->setChecked(true);

    }
    ui->actionToolBar->setChecked(true);
    ui->actionStasueBar->setChecked(true);
    on_actionLinenumber_triggered(false);
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
    findDialog dlg(this,ui->TextEdit);
    dlg.exec();
}


void MainWindow::on_action_R_triggered()
{
    ReplaceDialog dlg(this,ui->TextEdit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditComfirmed())
        return;
    filePath = "";
    ui->TextEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditComfirmed())
        return;


    QString filename = QFileDialog ::getOpenFileName(this, "打开文件", ".",tr("Text files (*.txt) ;; ALL (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return;

    }
    filePath = filename;
    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    textChanged = false;
}


void MainWindow::saveFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", "无法保存文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).fileName());
    textChanged = false;
}

void MainWindow::on_actionSave_triggered()
{

    if (filePath.isEmpty()) {
        on_actionSaveAs_triggered();
    } else {
        saveFile(filePath);
    }

}


void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt) ;; ALL (*.*)"));
    if (filename.isEmpty()) {
        return; // 用户取消了保存
    }
    filePath = filename;
    saveFile(filePath);

}


void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*"+this->windowTitle());
        textChanged = true;
    }
    statusLabel.setText("length:" + QString::number(ui->TextEdit->toPlainText().length()) +
                        "      lines: " + QString::number(ui->TextEdit->document()->lineCount()));
}

bool MainWindow::userEditComfirmed()
{
    if(textChanged){

        QString path = (filePath != "") ? filePath : "无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\"?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int q = msg.exec();
        switch(q){
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;

        }

    }
    return true;
}



void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}


void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}



void MainWindow::on_actionselectAll_triggered()
{
    ui->TextEdit->selectAll();
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFBGColor_triggered() // 设置字体背景颜色
{
    QColor bgColor = QColorDialog::getColor(Qt::yellow, this, "选择背景颜色");
    if (bgColor.isValid()) {
        QTextCursor cursor = ui->TextEdit->textCursor();
        if (!cursor.selection().isEmpty()) {
            QTextCharFormat format;
            format.setBackground(bgColor);
            cursor.mergeCharFormat(format);
            ui->TextEdit->mergeCurrentCharFormat(format);
        }
    }
}
void MainWindow::on_actionAutoWarp_triggered()
{
    QPlainTextEdit::LineWrapMode mode =ui->TextEdit->lineWrapMode();

    if( mode == QTextEdit::NoWrap ){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionAutoWarp->setChecked(true);
    }
    else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionAutoWarp->setChecked(false);

    }
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok)
        ui->TextEdit->setFont(font);
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}


void MainWindow::on_actionStasueBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStasueBar->setChecked(!visible);
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditComfirmed())
        exit(0);
}


void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int pos = ui->TextEdit->textCursor().position();
    int col = 0;
    int ln = 0;
    int flg = -1;
    QString text = ui->TextEdit->toPlainText();
    for(int i = 0;i < pos;i++){
        if(text[i] == '\n'){
            ln++;
            flg = i;
        }

    }
    flg++;
    col = pos - flg;
    statusCursorLabel.setText("Ln:" + QString::number(ln + 1) + "      Col: " + QString::number(col + 1));



}


void MainWindow::on_actionLinenumber_triggered(bool checked)
{
    ui->TextEdit->hideLineNumberArea(!checked);
}

