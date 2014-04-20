#include "autofilebackup.h"
#include "ui_autofilebackup.h"
#include <QFileSystemWatcher>
#include <QTime>
#include <QFileInfo>
#include <QFileDialog>
QFileSystemWatcher *folderMonitor;
QFileSystemWatcher *fileMonitor;
QString logLevel = "Debug";
AutoFileBackup::AutoFileBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoFileBackup)
{
    ui->setupUi(this);
}

AutoFileBackup::~AutoFileBackup()
{
    delete ui;
}

void AutoFileBackup::on_addNewFileButton_clicked()
{

    //File open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.*)"));

    addLog("Starting monitor",fileName,logLevel);

    folderMonitor = new QFileSystemWatcher(this);
    fileMonitor = new QFileSystemWatcher(this);

    connect(fileMonitor, SIGNAL(fileChanged(const QString &)), this,
            SLOT(fileChanged(const QString &)));

    connect(folderMonitor, SIGNAL(directoryChanged(const QString &)), this,
            SLOT(directoryChanged(const QString &)));

    addNewWatchFile(fileName);
//    fileMonitor->addPath(fileName);
//    watchedFileName = fileName;

//    ui->logTable->setItem(0, 0, new QTableWidgetItem(QTime::currentTime().toString()));
//    ui->logTable->setItem(0, 0, new QTableWidgetItem(fileName));
//    ui->logList->addItem(QTime::currentTime().toString() + ": Watch File : " + fileName);
//    QFileInfo watchedFileInfo(fileName);
//    folderMonitor->addPath(watchedFileInfo.absoluteDir().path());
//    ui->logList->addItem(QTime::currentTime().toString() + ": Watch Folder : " + watchedFile.absoluteDir().path());


}

void AutoFileBackup::on_removeFileButton_clicked()
{

        ui->logTable->setItem(0, 0, new QTableWidgetItem(QTime::currentTime().toString()));
}

void AutoFileBackup::fileChanged(const QString &path)
{

//    logFileDirectoryChangeEvent("File Modified",path);
//    QFileInfo watchedFile(path);
//    if (watchedFile.exists())
//    {
////        ui->fileChangeLogText->append(path + " File exists");
//    }
//    else{
////        ui->fileChangeLogText->append(path + " File does not exists");
//    }
}

void AutoFileBackup::directoryChanged(const QString &path)
{
//    ui->textEdit->append("Directory changed path :" + path + " file to monitor : " + watchedFileName);
//    QFileInfo watchedFile(watchedFileName);
//    if (watchedFile.exists())
//    {
//        ui->textEdit->append(path + " Dir Exists");
//        if(watch_file->files().contains(watchedFileName))
//        {
//            ui->textEdit->append(watchedFileName + " Watch Exists");
//        }
//        else
//        {
//            ui->textEdit->append(watchedFileName + " Watch does not exists");
//            watch_file->addPath(watchedFileName);
//        }
//    }
//    else{
//        ui->textEdit->append(path + " Dir Does not exists");
//    }
}

int AutoFileBackup::newLogTableRow()
{
    int currentRow = ui->logTable->rowCount();
    ui->logTable->setRowCount(currentRow + 1);
    return currentRow;
}

void AutoFileBackup::addLog(QString statusText, QString value, QString logLevel)
{
    int lastRowIndex = newLogTableRow();
//     ui->settingsButton->setText(QString::number(currentRow));
    QString item = QTime::currentTime().toString() +":"+statusText +":"+ value;
    ui->logList->addItem(item);
//    ui->logTable->setItem(lastRowIndex, 0, new QTableWidgetItem(QTime::currentTime().toString()));
//    ui->logTable->setItem(lastRowIndex, 1, new QTableWidgetItem(statusText));
//    ui->logTable->setItem(lastRowIndex, 2, new QTableWidgetItem(value));
}

void AutoFileBackup::addNewWatchFile(QString file)
{
    if (fileMonitor->addPath(file))
    {
        addLog("File watch added",file,logLevel);
        newWatchedFilesTableRow(file);
        QFileInfo watchedFileInfo(file);
        folderMonitor->addPath(watchedFileInfo.absoluteDir().path());
        addLog("Folder watch added",watchedFileInfo.absoluteDir().path(),logLevel);
    }
    ui->watchedFilesList->clear();
    ui->watchedFilesList->addItems(fileMonitor->files());
}

void AutoFileBackup::newWatchedFilesTableRow(QString fileName)
{
//    int currentRow = ui->watchedFilesTable->rowCount();
//    ui->watchedFilesTable->setRowCount(currentRow + 1);
//    ui->watchedFilesTable->setItem(currentRow, 0, new QTableWidgetItem(QTime::currentTime().toString()));
//    ui->watchedFilesTable->setItem(currentRow, 1, new QTableWidgetItem(fileName));
}
