#include "autofilebackup.h"
#include "ui_autofilebackup.h"
#include <QFileSystemWatcher>
#include <QTime>
#include <QFileInfo>
#include <QFileDialog>
#include "filecopysettings.h"
#include <QSettings>
#include <QDebug>
#include <QFontDatabase>
#include <QTextStream>
#include <QProcess>
#include<projectconfiguration.h>

QFileSystemWatcher *folderMonitor;
QFileSystemWatcher *fileMonitor;
QList<QString> watchedFiles;
AutoFileBackup::AutoFileBackup(QWidget *parent) : QWidget(parent), ui(new Ui::AutoFileBackup)
{

    QString myPath = QCoreApplication::applicationDirPath() + QDir::separator() + "lib" + QDir::separator() + "fonts" ;
    QFileInfo fontsdir; (myPath);
    fontsdir.dir().mkpath(myPath );

    qDebug() << myPath;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", myPath );
    env.insert("QT_QPA_FONTDIR", myPath );
    setWindowIcon(QIcon(":/icons/appicon.ico"));
    // Load the embedded font.
    QString fontPath = ":/fonts/DejaVuSans.ttf";
    //    QFontDatabase::removeAllApplicationFonts();
    int fontId = QFontDatabase::addApplicationFont(fontPath);

        QFont font("DejaVu Sans",9);
    //        font.setStyleName("Regular");
        this->setFont(font);
    qDebug() << fontId ;
    qDebug() << QWidget::fontInfo().family();
    qDebug() << QWidget::fontInfo().styleName();
    qDebug() << QWidget::fontInfo().style();
    QFontDatabase db1;
    qDebug() << db1.families();


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


//    ui->watchedFilesTableWidget->AdjustToContents;
     ui->watchedFilesTableWidget->setColumnWidth(0,  500);
     ui->watchedFilesTableWidget->setColumnWidth(1,  100);


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


    QList<QTableWidgetSelectionRange> ranges = ui->watchedFilesTableWidget->selectedRanges();


    int selectedCount = ui->watchedFilesTableWidget->selectedItems().count();
    qDebug() << selectedCount;

    while(ui->watchedFilesTableWidget->selectedItems().count() > 0)
    {
        ui->watchedFilesTableWidget->setCurrentItem(ui->watchedFilesTableWidget->selectedItems().last());
        fileMonitor->removePath(ui->watchedFilesTableWidget->currentItem()->text());
        addLog("Remove Watch",ui->watchedFilesTableWidget->currentItem()->text());
        ui->watchedFilesTableWidget->removeRow(ui->watchedFilesTableWidget->currentRow());
        watchedFiles = fileMonitor->files();
    }
        setDropOverlay();

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

void AutoFileBackup::directoryChanged(const QString &)
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

void AutoFileBackup::setDropOverlay()
{
    if(ui->watchedFilesTableWidget->rowCount()<1)
    {
        ui->watchedFilesTableWidget->setStyleSheet("QFrame {background-image: url(:/images/dropfiles.png);background-repeat: no-repeat;background-position: center center;background-attachment: fixed;}");
    }
    else
    {
        ui->watchedFilesTableWidget->setStyleSheet("");
    }
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


        if(!watchedFiles.contains(file))
        {
            insertToWatchTable(file);
        }
        addLog("Watching File :" , file);

        watchedFiles = fileMonitor->files();
    }
    else
    {
        return false;
    }
    setDropOverlay();

//    ui->watchedFilesList->clear();
    // Populate the watchedFiles QList with all the files currently monitoring.

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
    if(result)
    {
        addLog("Created File ", destinationFile);
    }
    else
    {
        addLog("Unable to create file ", destinationFile);
    }
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
//    if (button->text() == tr("Apply"))
//        saveFileCopySettings();
//    else if (button->text() == tr("Discard"))
//        loadFileCopySettings();
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

FileCopySettings AutoFileBackup::getFileCopySettings()
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
    fcSettings.setHasSuffixDate(ui->suffixDateTimeCheckBox->isChecked());
    fcSettings.setSuffixDateFormat(ui->suffixDateTimeFormatLineEdit->text());
    fcSettings.setSuffixAfterDateTime(ui->suffixAfterDateTimeLineEdit->text());

//    fcSettings.saveSettings();
    return fcSettings;

}

void AutoFileBackup::setFileCopySettings(FileCopySettings fcSettings)
{
//    fcSettings.readSettings();

    ui->saveToDiffDirCheckBox->setChecked(fcSettings.getSaveToDiffDir());
    ui->destinationDirLineEdit->setText(fcSettings.getDestinationDir());
    ui->saveToSubDirLineEdit->setText(fcSettings.getSaveToSubDir());
    ui->filePrefixLineEdit->setText(fcSettings.getPrefixString());
    ui->fileSuffixLineEdit->setText(fcSettings.getSuffixString());
    ui->suffixDateTimeCheckBox->setChecked(fcSettings.getHasSuffixDate());
    ui->suffixDateTimeFormatLineEdit->setText(fcSettings.getSuffixDateFormat());
    ui->suffixAfterDateTimeLineEdit->setText(fcSettings.getSuffixAfterDateTime());
}

void AutoFileBackup::on_watchedFilesTableWidget_dropped(const QMimeData *mimeData)
{
    if (mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        for (int i=0; i<urls.count(); ++i)
        {
            QFileInfo watchfile(urls[i].toLocalFile());
            if (watchfile.isFile())
            {
                addNewWatchFile(watchfile.absoluteFilePath());
            }
        }
    }
}

void AutoFileBackup::insertToWatchTable(QString file)
{
    int row = ui->watchedFilesTableWidget->rowCount();
    ui->watchedFilesTableWidget->insertRow(row);
    QTableWidgetItem *newItem = new QTableWidgetItem(file);
    ui->watchedFilesTableWidget->setItem(row, 0, newItem);
}

void AutoFileBackup::on_saveProjectFileButton_clicked()
{
    ProjectConfiguration p;
    FileCopySettings fc ;
    p.setFileCopySettings(getFileCopySettings());
    p.setWatchedFileList(watchedFiles);
    QString savefilename= QFileDialog::getSaveFileName(this,"Save Xml", ".", "Xml files (*.xml)");
    p.saveToFile(savefilename);
}

void AutoFileBackup::on_openProjectButton_clicked()
{
     ProjectConfiguration p;
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.*)"));
     p.openFromFile(fileName);
     foreach(QString watchFile ,p.getWatchedFileList())
     {
         addNewWatchFile(watchFile);
     }
}
