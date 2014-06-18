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
#include <projectconfiguration.h>
#include <QSystemTrayIcon>
#include <QMenuBar>
#include<QMessageBox>
#include <QTimer>

AutoFileBackup::AutoFileBackup(QWidget *parent) : QWidget(parent), ui(new Ui::AutoFileBackup)
{

    createTrayActions();
    createTrayIcon();
    setTrayIcon();
    trayIcon->show();

/*
    QString myPath = QCoreApplication::applicationDirPath() + QDir::separator() + "lib" + QDir::separator() + "fonts" ;
    QFileInfo fontsdir; (myPath);
    fontsdir.dir().mkpath(myPath );

    qDebug() << myPath;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", myPath );
    env.insert("QT_QPA_FONTDIR", myPath );

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
*/
    setWindowIcon(QIcon(":/images/backup_ico.png"));
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
    if (!fileName.isEmpty())
    {

//    addLog("Starting monitor",fileName,logLevel);

        addNewWatchFile(fileName);
    }

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

    if(!changedFileQueue.contains(path))
    {
        changedFileQueue.append(path);
        addLog("File added to queue",path);
        ui->changedFileQueueList->clear();
        ui->changedFileQueueList->addItems(changedFileQueue);




        //    qDebug() << "before File Copying        " + QTime::currentTime().toString() ;

        QTimer::singleShot( ui->savingTimeDelaySpinBox->value(), this, SLOT(delayedFileChanged()));

        //    qDebug() << "Finished File Copying         " + QTime::currentTime().toString() ;

    }
    else
        addLog("File already in queue",path);
}

void AutoFileBackup::delayedFileChanged()
{
    //    int lastItemIndex = ui->changedFileQueueList->count() - 1;
    QString path;
    if(!changedFileQueue.isEmpty() )
    {
        path =  changedFileQueue.first();
        changedFileQueue.removeFirst();

        ui->changedFileQueueList->clear();
        ui->changedFileQueueList->addItems(changedFileQueue);

        addLog("Delayed File Modified" + QTime::currentTime().toString() ,path);
        //    ui->changedFileQueueList
        QFileInfo modifiedFile(path);
        if (modifiedFile.exists())
        {

            QString saveDir;
            QString prefixString;
            QString suffixString;
            bool suffixDate;
            QString suffixDateFormat;
            QString suffixAfterDateTime;
            if (ui->isSavingToDifferentFolderRadioButton->isChecked() == true)
            {
                saveDir=ui->destinationDirLineEdit->text();
            }
            else if (ui->isSameFolderRadioButton->isChecked() == true)
            {
                if (ui->saveToSubDirLineEdit->text() == NULL || ui->saveToSubDirLineEdit->text().trimmed() == "" )
                {
                    saveDir= modifiedFile.absoluteDir().path();
                }
                else
                {
                    saveDir= modifiedFile.absoluteDir().path() + QDir::separator() + ui->saveToSubDirLineEdit->text();

                }
            }

            if (ui->createDateFolderCheckBox->isChecked() == true)
                saveDir= saveDir + QDir::separator() + QDateTime::currentDateTime().toString(ui->dateFolderFormatLineEdit->text());

            QDir subDirChecker(saveDir);
            if(!subDirChecker.exists())
            {
                subDirChecker.mkpath(saveDir);
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
}




void AutoFileBackup::directoryChanged(const QString &)
{
//    addLog("Directory Modified",path,logLevel);
    // Check the changed directory for all the watched files , Sometimes editors delete the original file and use a
    // temp for for processing , check if our original file is restored, if restored add it to watch again.

//    qDebug() << "before Starting       " + QTime::currentTime().toString() ;

    QTimer::singleShot( ui->savingTimeDelaySpinBox->value(), this, SLOT(delayedDirectoryChanged()));

//    qDebug() << "Finished Starting     " + QTime::currentTime().toString() ;



}

void AutoFileBackup::delayedDirectoryChanged()
{
//    qDebug() << "After Starting" + QTime::currentTime().toString() ;

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
     QList<QString> OldWatchedFiles;
    // Adds the path to the fileMonitor only if it exists, does not adds if it is already there.

//    qDebug() << "-----------------------------------------------------------------------------------------------------------";

//     qDebug() << "Outer       " + file;
//    foreach (QString var, watchedFiles)
//    {
//        qDebug() << "WatchedFiles                " + var;
//    }


    if(fileMonitor->addPath(file))
    {
//    addLog("New File watch added",file,logLevel);
        // Get the corresponding Parent directory of the file and monitor that directory for changes.
        QFileInfo watchedFileInfo(file);
        folderMonitor->addPath(watchedFileInfo.absoluteDir().path());

//        qDebug() << "Inner              " + file;

        if(!watchedFiles.contains(file))
        {
//            qDebug() << "Inner Not Contains              " + file;
            insertToWatchTable(file);
            watchedFiles.append(file);
        }
        addLog("Watching File " , file);
//        trayIcon->showMessage("New file added", file + " has been added to monitor for changes" ,QSystemTrayIcon::Information,1000);

//        watchedFiles = fileMonitor->files();
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
    QFile qq (sourceFile);
    if(result)
    {
        addLog("Created File ", destinationFile);
        trayIcon->showMessage("File Changed", "File " + sourceFile + " \nhas been backuped to  " + destinationFile ,QSystemTrayIcon::Information,1000);
    }
    else
    {
        addLog("Unable to create file ", destinationFile);
        trayIcon->showMessage("Unable to create file", "Unable to create file " + destinationFile ,QSystemTrayIcon::Critical,1000);
    }
    return result;
}


//void AutoFileBackup::on_saveToDiffDirCheckBox_stateChanged(int arg1)
//{
//    ui->saveToSubDirLineEdit->setEnabled(!arg1);
//    ui->destinationDirLineEdit->setEnabled(arg1);
//    ui->destinationDirBrowseButton->setEnabled(arg1);

////    if(arg1==0)
////    {
////        ui->saveToSubDirLineEdit->setEnabled(true);
////        ui->destinationDirLineEdit->setEnabled(false);
////        ui->destinationDirBrowseButton->setEnabled(false);
////    }
////    else if(arg1==2)
////    {
////        ui->saveToSubDirLineEdit->setEnabled(false);
////        ui->destinationDirLineEdit->setEnabled(true);
////        ui->destinationDirBrowseButton->setEnabled(true);
////    }
//}

void AutoFileBackup::on_suffixDateTimeCheckBox_stateChanged(int arg1)
{

    ui->suffixDateTimeFormatLineEdit->setEnabled(arg1);
//    if(arg1==0)
//    {
//        ui->suffixDateTimeFormatLineEdit->setEnabled(false);
//    }
//    else if(arg1==2)
//    {
//        ui->suffixDateTimeFormatLineEdit->setEnabled(true);
//    }
}

void AutoFileBackup::on_destinationDirBrowseButton_clicked()
{
    QString saveDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->destinationDirLineEdit->setText(saveDir);
}

//void AutoFileBackup::on_fileCopyButtonBox_clicked(QAbstractButton *button)
//{
////    if (button->text() == tr("Apply"))
////        saveFileCopySettings();
////    else if (button->text() == tr("Discard"))
////        loadFileCopySettings();
//}

void AutoFileBackup::resetFileCopySettings()
{
    ui->destinationDirBrowseButton->setEnabled(false);
    ui->destinationDirLineEdit->setText("");
    ui->filePrefixLineEdit->setText("");
    ui->fileSuffixLineEdit->setText("");
    ui->isSavingToDifferentFolderRadioButton->setChecked(false);
    ui->saveToSubDirLineEdit->setText("");
    ui->suffixAfterDateTimeLineEdit->setText("");
    ui->suffixDateTimeCheckBox->setCheckState(Qt::Checked);
    ui->suffixDateTimeFormatLineEdit->setText("dd-MM-yyyy-hh-mm-ss");
}

FileCopySettings AutoFileBackup::getFileCopySettings()
{
    FileCopySettings fcSettings;

    if (ui->isSavingToDifferentFolderRadioButton->isChecked())
    {
        fcSettings.setIsSavingToDifferentFolder(true);
        fcSettings.setDestinationDir(ui->destinationDirLineEdit->text());
    }
    else
    {
        fcSettings.setIsSavingToDifferentFolder(false);
        fcSettings.setSaveToSubDir(ui->saveToSubDirLineEdit->text());
    }
    fcSettings.setCreateDateBasedFolder(ui->createDateFolderCheckBox->isChecked());
    fcSettings.setDateBasedFolderFormat(ui->dateFolderFormatLineEdit->text());
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

    ui->isSavingToDifferentFolderRadioButton->setChecked(fcSettings.getIsSavingToDifferentFolder());
    ui->isSameFolderRadioButton->setChecked(!fcSettings.getIsSavingToDifferentFolder());
    ui->destinationDirLineEdit->setText(fcSettings.getDestinationDir());
    ui->saveToSubDirLineEdit->setText(fcSettings.getSaveToSubDir());
    ui->filePrefixLineEdit->setText(fcSettings.getPrefixString());
    ui->fileSuffixLineEdit->setText(fcSettings.getSuffixString());
    ui->suffixDateTimeCheckBox->setChecked(fcSettings.getHasSuffixDate());
    ui->suffixDateTimeFormatLineEdit->setText(fcSettings.getSuffixDateFormat());
    ui->suffixAfterDateTimeLineEdit->setText(fcSettings.getSuffixAfterDateTime());
    ui->createDateFolderCheckBox->setChecked(fcSettings.getCreateDateBasedFolder());
    ui->dateFolderFormatLineEdit->setText(fcSettings.getDateBasedFolderFormat());
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

void AutoFileBackup::on_saveProjectButton_clicked()
{
    projectConfg.setFileCopySettings(getFileCopySettings());
    projectConfg.setWatchedFileList(watchedFiles);
    projectConfg.setSavingTimeDelay(ui->savingTimeDelaySpinBox->value());
    if(projectConfg.getCurrentProjectFileName().isEmpty())
    {
        QString savefilename= QFileDialog::getSaveFileName(this,"Save AutoFileBackup Project", ".", "Project files (*.afb)");
        projectConfg.saveToFile(savefilename);
    }
    else
        projectConfg.saveToFile(projectConfg.getCurrentProjectFileName());
    ui->openedProjectFileLabel->setText(projectConfg.getCurrentProjectFileName());

}

void AutoFileBackup::on_openProjectButton_clicked()
{
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.afb)"));
     if (!fileName.isEmpty())
     {
         if(projectConfg.openFromFile(fileName))
         {

             folderMonitor = new QFileSystemWatcher(this);
             fileMonitor = new QFileSystemWatcher(this);
             watchedFiles.clear();
//             qDebug() << ui->watchedFilesTableWidget->rowCount();
             clearFileMonitorTable();
             foreach(QString watchFile ,projectConfg.getWatchedFileList())
             {
                 addNewWatchFile(watchFile);
             }
             ui->savingTimeDelaySpinBox->setValue(projectConfg.getSavingTimeDelay());
             setFileCopySettings(projectConfg.getFileCopySettings());
             ui->openedProjectFileLabel->setText(projectConfg.getCurrentProjectFileName());
         }
         else
             ui->openedProjectFileLabel->setText("Error opening file.");
     }

//          qDebug() <<p.getFileCopySettings().getPrefixString();
}

void AutoFileBackup::clearFileMonitorTable()
{
    for (int var = ui->watchedFilesTableWidget->rowCount();  var > 0; var--) {
        ui->watchedFilesTableWidget->removeRow(var - 1);
    }
}


void AutoFileBackup::changeEvent(QEvent *event)
{
//    AutoFileBackup::changeEvent(event);
    if(event->type() == QEvent::WindowStateChange) {
        if(isMinimized())
        {
//            this->hide();
            showHideWindow();
        }
    }
}

void AutoFileBackup::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        showHideWindow();
        event->ignore();
    }
}

void AutoFileBackup::showHideWindow()
{
    if(this->isVisible())
    {
        this->hide();
        showHideTray->setIcon(QIcon(":/images/maximize-button.png"));
        showHideTray->setText("Show Main Window");
        trayIcon->showMessage("I am here.","I will be here monitoring your files." ,QSystemTrayIcon::Information,1000);
    }
    else
    {
        this->setWindowState(Qt::WindowActive);
        this->show();
        showHideTray->setIcon(QIcon(":/images/minimize-button.png"));
        showHideTray->setText("Hide Main Window");
    }
}


void AutoFileBackup::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
        showHideWindow();
}

void AutoFileBackup::setTrayIcon()
{
    trayIcon->setIcon(QIcon(":/images/backup_ico.png"));
}
void AutoFileBackup::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showHideTray);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeTray);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(
            trayIcon,
          SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
           );
}
void AutoFileBackup::createTrayActions()
{
    showHideTray = new QAction(tr("&Hide Main Window"), this);
    connect(showHideTray, SIGNAL(triggered()), this, SLOT(showHideWindow()));
    showHideTray->setIcon(QIcon(":/images/minimize-button.png"));
    closeTray = new QAction(tr("&Exit"), this);
    connect(closeTray, SIGNAL(triggered()), qApp, SLOT(quit()));
    closeTray->setIcon(QIcon(":/images/exit-button.png"));
}

void AutoFileBackup::on_createDateFolderCheckBox_stateChanged(int arg1)
{
    ui->dateFolderFormatLineEdit->setEnabled(arg1);
}


void AutoFileBackup::on_saveProjectAsButton_clicked()
{
    ProjectConfiguration p;
    projectConfg.setFileCopySettings(getFileCopySettings());
    projectConfg.setWatchedFileList(watchedFiles);
    QString savefilename= QFileDialog::getSaveFileName(this,"Save AutoFileBackup Project", ".", "Project files (*.afb)");
    projectConfg.saveToFile(savefilename);
    ui->openedProjectFileLabel->setText(projectConfg.getCurrentProjectFileName());

}

void AutoFileBackup::on_isSavingToDifferentFolderRadioButton_toggled(bool checked)
{
    ui->saveToSubDirLineEdit->setEnabled(!checked);
    ui->destinationDirLineEdit->setEnabled(checked);
    ui->destinationDirBrowseButton->setEnabled(checked);
}

void AutoFileBackup::on_clearLogButton_clicked()
{
    ui->logList->clear();
}
