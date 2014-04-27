#include "autofilebackup.h"
#include "ui_autofilebackup.h"
#include <QFileSystemWatcher>
#include <QTime>
#include <QFileInfo>
#include <QFileDialog>
#include "filecopysettings.h"
#include <QSettings>
#include <QDebug>

QFileSystemWatcher *folderMonitor;
QFileSystemWatcher *fileMonitor;
QList<QString> watchedFiles;
AutoFileBackup::AutoFileBackup(QWidget *parent) : QWidget(parent), ui(new Ui::AutoFileBackup)
{

    ui->setupUi(this);

    folderMonitor = new QFileSystemWatcher(this);
    fileMonitor = new QFileSystemWatcher(this);

    connect(fileMonitor, SIGNAL(fileChanged(const QString &)), this,
            SLOT(fileChanged(const QString &)));

    connect(folderMonitor, SIGNAL(directoryChanged(const QString &)), this,
            SLOT(directoryChanged(const QString &)));

    connect(ui->watchedFilesTableWidget,SIGNAL(dropped(const QMimeData*)), this , SLOT(on_watchedFilesTableWidget_dropped(const QMimeData *)));

    QCoreApplication::setOrganizationName("Stalin");
//    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Auto File Backup");

    loadFileCopySettings();

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
    addLog("Remove Watch",ui->watchedFilesList->selectedItems().first()->text());
    // Populate the watchedFiles QList with all the files currently monitoring.

    watchedFiles = fileMonitor->files();
    ui->watchedFilesList->clear();
    ui->watchedFilesList->addItems(watchedFiles);



}

void AutoFileBackup::fileChanged(const QString &path)
{

    addLog("File Modified",path);
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
                QFileInfo subDirChecker(saveDir);
                if(!subDirChecker.exists())
                {
                   subDirChecker.dir().mkdir(saveDir);
                }
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



void AutoFileBackup::addLog(QString statusText, QString value)
{

    QString item = QTime::currentTime().toString() +":"+statusText +":"+ value;
    ui->logList->addItem(item);

}

bool AutoFileBackup::addNewWatchFile(QString file)
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
    else
    {
        return false;
    }
    ui->watchedFilesList->clear();
    // Populate the watchedFiles QList with all the files currently monitoring.
    watchedFiles = fileMonitor->files();
    ui->watchedFilesList->addItems(watchedFiles);
    return true;
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

void AutoFileBackup::on_destinationDirBrowseButton_clicked()
{
    QString saveDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->destinationDirLineEdit->setText(saveDir);
}

void AutoFileBackup::on_fileCopyButtonBox_clicked(QAbstractButton *button)
{
    if (button->text() == tr("Apply"))
        saveFileCopySettings();
    else if (button->text() == tr("Discard"))
        loadFileCopySettings();
}

void AutoFileBackup::resetFileCopySettings()
{
    ui->destinationDirBrowseButton->setEnabled(false);
    ui->destinationDirLineEdit->setText("");
    ui->filePrefixLineEdit->setText("");
    ui->fileSuffixLineEdit->setText("");
    ui->saveToDiffDirCheckBox->setCheckState(Qt::Unchecked);
    ui->saveToSubDirLineEdit->setText("");
    ui->suffixAfterDateTimeLineEdit->setText("");
    ui->suffixDateTimeCheckBox->setCheckState(Qt::Checked);
    ui->suffixDateTimeFormatLineEdit->setText("dd-MM-yyyy-hh-mm-ss");
}

void AutoFileBackup::saveFileCopySettings()
{
    FileCopySettings fcSettings;

    if (ui->saveToDiffDirCheckBox->isChecked())
    {
        fcSettings.setSaveToDiffDir(true);
        fcSettings.setDestinationDir(ui->destinationDirLineEdit->text());
    }
    else
    {
        fcSettings.setSaveToDiffDir(false);
        fcSettings.setSaveToSubDir(ui->saveToSubDirLineEdit->text());
    }
    fcSettings.setPrefixString(ui->filePrefixLineEdit->text());
    fcSettings.setSuffixString(ui->fileSuffixLineEdit->text());
    fcSettings.setSuffixDate(ui->suffixDateTimeCheckBox->isChecked());
    fcSettings.setSuffixDateFormat(ui->suffixDateTimeFormatLineEdit->text());
    fcSettings.setSuffixAfterDateTime(ui->suffixAfterDateTimeLineEdit->text());

    fcSettings.saveSettings();

}

void AutoFileBackup::loadFileCopySettings()
{
    FileCopySettings fcSettings;
    fcSettings.readSettings();

    ui->saveToDiffDirCheckBox->setChecked(fcSettings.getSaveToDiffDir());
    ui->destinationDirLineEdit->setText(fcSettings.getDestinationDir());
    ui->saveToSubDirLineEdit->setText(fcSettings.getSaveToSubDir());
    ui->filePrefixLineEdit->setText(fcSettings.getPrefixString());
    ui->fileSuffixLineEdit->setText(fcSettings.getSuffixString());
    ui->suffixDateTimeCheckBox->setChecked(fcSettings.getSuffixDate());
    ui->suffixDateTimeFormatLineEdit->setText(fcSettings.getSuffixDateFormat());
    ui->suffixAfterDateTimeLineEdit->setText(fcSettings.getSuffixAfterDateTime());
}

void AutoFileBackup::on_watchedFilesTableWidget_dropped(const QMimeData *mimeData)
{
    qDebug() << "dropped slot";
    if (mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        for (int i=0; i<urls.count(); ++i)
        {
            QFileInfo watchfile(urls[i].toLocalFile());
            qDebug() << watchfile.absoluteFilePath();
            if (watchfile.isFile())
            {
                if (addNewWatchFile(watchfile.absoluteFilePath()))
                {
                    int row = ui->watchedFilesTableWidget->rowCount();
                    ui->watchedFilesTableWidget->insertRow(row);
                    QTableWidgetItem *newItem = new QTableWidgetItem(watchfile.absoluteFilePath());
                    ui->watchedFilesTableWidget->setItem(row, 0, newItem);
                }
            }
        }
    }
}
