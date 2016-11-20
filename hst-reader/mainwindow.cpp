#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(historyReader != NULL)
        delete historyReader;
}

void MainWindow::on_findFileButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open .hst file:"), "D:\\Projects\\MQL5 History", tr("History file (*.hst)"));
    ui->filePathEdit->setText(filePath);
    ui->textBrowser->insertPlainText(filePath + "\n");
    qDebug() << filePath;
}

void MainWindow::on_pushButton_clicked()
{
    historyReader = new HstReader(filePath);
    historyReader->readFromFile();
    ui->textBrowser->insertPlainText(historyReader->getHeaderString() + "\n\n");

    for(uint i = 0; i < historyReader->getHistorySize(); i++)
    {
        ui->textBrowser->insertPlainText(historyReader->getHistoryString(i) + "\n");
        qDebug() << historyReader->getHistoryString(i) << "\n";
    }

    QString tempMsg = QString("MW: File readed. History size - %1\n\n").arg(historyReader->getHistorySize());
    ui->textBrowser->insertPlainText(tempMsg);
    qDebug() << tempMsg;

    delete historyReader;
}
