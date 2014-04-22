#include "autofilebackup.h"
#include "ui_autofilebackup.h"
#include <QFileSystemWatcher>
#include <QTime>
#include <QFileInfo>
#include <QFileDialog>
#include "diff_match_patch/diff_match_patch.h"

QFileSystemWatcher *folderMonitor;
QFileSystemWatcher *fileMonitor;
QString logLevel = "Debug";
QList<QString> watchedFiles;
AutoFileBackup::AutoFileBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoFileBackup)
{
    ui->setupUi(this);

    folderMonitor = new QFileSystemWatcher(this);
    fileMonitor = new QFileSystemWatcher(this);

    connect(fileMonitor, SIGNAL(fileChanged(const QString &)), this,
            SLOT(fileChanged(const QString &)));

    connect(folderMonitor, SIGNAL(directoryChanged(const QString &)), this,
            SLOT(directoryChanged(const QString &)));
}

AutoFileBackup::~AutoFileBackup()
{
    delete ui;
}

void AutoFileBackup::on_addNewFileButton_clicked()
{

    //File open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.*)"));

//    addLog("Starting monitor",fileName,logLevel);

    addNewWatchFile(fileName);


}

void AutoFileBackup::on_removeFileButton_clicked()
{
    fileMonitor->removePath(ui->watchedFilesList->selectedItems().first()->text());
    addLog("Remove Watch",ui->watchedFilesList->selectedItems().first()->text(),logLevel);
    // Populate the watchedFiles QList with all the files currently monitoring.

    watchedFiles = fileMonitor->files();
    ui->watchedFilesList->clear();
    ui->watchedFilesList->addItems(watchedFiles);



}

void AutoFileBackup::fileChanged(const QString &path)
{

    addLog("File Modified",path,logLevel);
    QFileInfo watchedFile(path);
    if (watchedFile.exists())
    {
        QString saveDir;
        QString prefixString;
        QString suffixString;
        bool suffixDate;
        QString suffixDateFormat;
        QString suffixAfterDateTime;
        if (ui->saveToDiffDirCheckBox->checkState() == Qt::Checked)
        {
            saveDir=ui->destinationDirLineEdit->text();
        }
        else if (ui->saveToDiffDirCheckBox->checkState() == Qt::Unchecked)
        {
            if (ui->saveToSubDirLineEdit->text() == NULL || ui->saveToSubDirLineEdit->text().trimmed() == "" )
            {
                saveDir= watchedFile.absoluteDir().path();
            }
            else
            {
                saveDir= watchedFile.absoluteDir().path() + QDir::separator() + ui->saveToSubDirLineEdit->text();
            }
        }
        prefixString = ui->filePrefixLineEdit->text();
        suffixString = ui->fileSuffixLineEdit->text();
        suffixDate =  ui->suffixDateTimeCheckBox->checkState();

        suffixDateFormat = ui->suffixDateTimeFormatLineEdit->text();
        suffixAfterDateTime = ui->suffixAfterDateTimeLineEdit->text();

        copyFileAsBackup(path,saveDir,prefixString,suffixString,suffixDate,suffixDateFormat,suffixAfterDateTime);


//        addLog("File exists",path,logLevel);
    }
    else{
//        addLog("File does not exists",path,logLevel);
    }
}

void AutoFileBackup::directoryChanged(const QString &path)
{
//    addLog("Directory Modified",path,logLevel);
    // Check the changed directory for all the watched files , Sometimes editors delete the original file and use a
    // temp for for processing , check if our original file is restored, if restored add it to watch again.
    foreach (QString watchedFile, watchedFiles)
    {
        QFileInfo watchedFileInfo(watchedFile);
        if (watchedFileInfo.exists())
        {
//            addLog("File Exists",watchedFile,logLevel);
            addNewWatchFile(watchedFile);
        }
        else
        {
//            addLog("File Not Exists",watchedFile,logLevel);

        }

    }
}



void AutoFileBackup::addLog(QString statusText, QString value, QString logLevel)
{

    QString item = QTime::currentTime().toString() +":"+statusText +":"+ value;
    ui->logList->addItem(item);

}

void AutoFileBackup::addNewWatchFile(QString file)
{
    // Adds the path to the fileMonitor only if it exists, does not adds if it is already there.
    if(fileMonitor->addPath(file))
    {
//    addLog("New File watch added",file,logLevel);
        // Get the corresponding Parent directory of the file and monitor that directory for changes.
        QFileInfo watchedFileInfo(file);
        folderMonitor->addPath(watchedFileInfo.absoluteDir().path());
//        addLog("Folder watch added",watchedFileInfo.absoluteDir().path(),logLevel);
    }
    ui->watchedFilesList->clear();
    // Populate the watchedFiles QList with all the files currently monitoring.
    watchedFiles = fileMonitor->files();
    ui->watchedFilesList->addItems(watchedFiles);
}

bool AutoFileBackup::copyFileAsBackup(QString sourceFile,QString destinationDir,QString prefixString, QString suffixString, bool suffixDate, QString suffixDateFormat, QString suffixAfterDateTime)
{
    QFileInfo fileInfo(sourceFile);
    QString suffixDateString ;
    if(suffixDate==true)
    {
        suffixDateString = QDateTime::currentDateTime().toString(suffixDateFormat);
    }
    else
    {
        suffixDateString="";
    }
    QString destinationFile = destinationDir + QDir::separator() + prefixString + fileInfo.baseName() + suffixString + suffixDateString + suffixAfterDateTime + "." + fileInfo.completeSuffix() ;
    bool result = QFile::copy(sourceFile, destinationFile);
    return result;
}


void AutoFileBackup::on_saveToDiffDirCheckBox_stateChanged(int arg1)
{

    if(arg1==0)
    {
        ui->saveToSubDirLineEdit->setEnabled(true);
        ui->destinationDirLineEdit->setEnabled(false);
        ui->destinationDirBrowseButton->setEnabled(false);
    }
    else if(arg1==2)
    {
        ui->saveToSubDirLineEdit->setEnabled(false);
        ui->destinationDirLineEdit->setEnabled(true);
        ui->destinationDirBrowseButton->setEnabled(true);
    }
}

void AutoFileBackup::on_suffixDateTimeCheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->suffixDateTimeFormatLineEdit->setEnabled(false);
    }
    else if(arg1==2)
    {
        ui->suffixDateTimeFormatLineEdit->setEnabled(true);
    }
}
