#include "projectconfiguration.h"
#include<QtXml/QtXml>
#include<QFileDialog>
#include <QObject>
#include<QMessageBox>

ProjectConfiguration::ProjectConfiguration()
{
}
void ProjectConfiguration::saveToFile(QString filename)
{

     QFile file(filename);
     file.open(QIODevice::WriteOnly);

     QXmlStreamWriter xmlWriter(&file);
     xmlWriter.setAutoFormatting(true);
     xmlWriter.writeStartDocument();

     xmlWriter.writeStartElement("AutoFileBackup");

     xmlWriter.writeStartElement("WatchedFileList");
     foreach (QString watchedFile, watchedFileList)
     {
         xmlWriter.writeTextElement("File", watchedFile);
     }
     xmlWriter.writeEndElement();

     xmlWriter.writeStartElement("FileCopySettings");

     xmlWriter.writeTextElement("IsSavingToDifferentFolder", QString::number(this->fileCopySettings.getIsSavingToDifferentFolder()));
     xmlWriter.writeTextElement("destinationDir", this->fileCopySettings.getDestinationDir());
     xmlWriter.writeTextElement("saveToSubDir", this->fileCopySettings.getSaveToSubDir());
     xmlWriter.writeTextElement("createDateBasedFolder", QString::number(this->fileCopySettings .getCreateDateBasedFolder()));
     xmlWriter.writeTextElement("dateBasedFolderFormat", this->fileCopySettings.getDateBasedFolderFormat());
     xmlWriter.writeTextElement("prefixString", this->fileCopySettings.getPrefixString());
     xmlWriter.writeTextElement("suffixString", this->fileCopySettings.getSuffixString());
     xmlWriter.writeTextElement("hasSuffixDate", QString::number(this->fileCopySettings .getHasSuffixDate()));
     xmlWriter.writeTextElement("suffixDateFormat", this->fileCopySettings.getSuffixDateFormat());
     xmlWriter.writeTextElement("suffixAfterDateTime", this->fileCopySettings.getSuffixAfterDateTime());

     xmlWriter.writeEndElement();

     xmlWriter.writeStartElement("AdvancedSettings");
         xmlWriter.writeTextElement("SavingTimeDelay", QString::number(this->getSavingTimeDelay()));
     xmlWriter.writeEndElement();

     xmlWriter.writeEndElement();
     xmlWriter.writeEndDocument();

     file.close();
     setCurrentProjectFileName(filename);
     qDebug() <<"Saved";
}
void ProjectConfiguration::openFromFile(QString filename)
{
    QXmlStreamReader Rxml;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox Msgbox;
        QString msg = "Error: Cannot read file " + filename + ": " + file.errorString();
        Msgbox.setText(msg);
        Msgbox.exec();
    }
    Rxml.setDevice(&file);
    Rxml.readNext();

    while(!Rxml.atEnd())
    {
        Rxml.readNext();
        if(Rxml.isStartElement())
        {
            if(Rxml.name() == "AutoFileBackup")
            {
               Rxml.readNext();
            }
            else if(Rxml.name() == "WatchedFileList")
            {
               Rxml.readNext();
            }
            else if(Rxml.name() == "AdvancedSettings")
            {
               Rxml.readNext();
            }
            else if(Rxml.name() == "File")
            {
               this->watchedFileList.append(Rxml.readElementText());
//               qDebug() <<"Filelist " << watchedFileList;
               Rxml.readNext();
            }
            else if(Rxml.name() == "FileCopySettings")
            {
               Rxml.readNext();
            }
            else if(Rxml.name() == "IsSavingToDifferentFolder")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"saveToDiffDir " << value;
               this->fileCopySettings.setIsSavingToDifferentFolder(value.toInt());
//               qDebug() <<"saveToDiffDir " << this->fileCopySettings.getSaveToDiffDir();
               Rxml.readNext();
            }
            else if(Rxml.name() == "SavingTimeDelay")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"saveToDiffDir " << value;
               this->setSavingTimeDelay(value.toInt());
//               qDebug() <<"saveToDiffDir " << this->fileCopySettings.getSaveToDiffDir();
               Rxml.readNext();
            }
            else if(Rxml.name() == "destinationDir")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"destinationDir " << value;
               this->fileCopySettings.setDestinationDir(value);
//               qDebug() <<"destinationDir " << this->fileCopySettings.getDestinationDir();
               Rxml.readNext();
            } else if(Rxml.name() == "saveToSubDir")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"saveToSubDir " << value;
               this->fileCopySettings.setSaveToSubDir(value);
//               qDebug() <<"saveToSubDir " << this->fileCopySettings.getSaveToSubDir();
               Rxml.readNext();
            }
            else if(Rxml.name() == "prefixString")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"prefixString " << value;
               this->fileCopySettings.setPrefixString(value);
//               qDebug() <<"prefixString " << this->fileCopySettings.getPrefixString();
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixString")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"suffixString " << value;
               this->fileCopySettings.setSuffixString(value);
               Rxml.readNext();
            }
            else if(Rxml.name() == "hasSuffixDate")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"hasSuffixDate " << value;
               this->fileCopySettings.setHasSuffixDate(value.toInt());
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixDateFormat")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"suffixDateFormat " << value;
               this->fileCopySettings.setSuffixDateFormat(value);
               Rxml.readNext();
            }
            else if(Rxml.name() == "createDateBasedFolder")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"hasSuffixDate " << value;
               this->fileCopySettings.setCreateDateBasedFolder(value.toInt());
               Rxml.readNext();
            }
            else if(Rxml.name() == "dateBasedFolderFormat")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"suffixDateFormat " << value;
               this->fileCopySettings.setDateBasedFolderFormat(value);
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixAfterDateTime")
            {
                QString value = Rxml.readElementText();
//               qDebug() <<"suffixAfterDateTime " << value;
               this->fileCopySettings.setSuffixAfterDateTime(value);
               Rxml.readNext();
            }
            else
            {
//              qDebug() <<"Error " << Rxml.readElementText();
              Rxml.raiseError(QObject::tr("Not a AutoFileBackup Project file"));
              Rxml.readNext();
            }
        }
        else
        {
            Rxml.readNext();
        }
    }
    file.close();
    if (Rxml.hasError())
    {
        QMessageBox Msgbox;
        QString msg = "Error: Failed to parse file " + filename + ": " + Rxml.errorString();
        Msgbox.setText(msg);
        Msgbox.exec();
    }
    else if (file.error() != QFile::NoError)
    {
        QMessageBox Msgbox;
        QString msg ="Error: Cannot read file " + filename +  ": " + file.errorString();
        Msgbox.setText(msg);
        Msgbox.exec();
    }
    else
        setCurrentProjectFileName(filename);

}
QStringList ProjectConfiguration::getWatchedFileList()
{
    return watchedFileList;
}
FileCopySettings ProjectConfiguration::getFileCopySettings()
{
    return fileCopySettings;
}
void ProjectConfiguration::setFileCopySettings(FileCopySettings settings)
{
    fileCopySettings = settings;
}
void ProjectConfiguration::setWatchedFileList(QStringList fileList)
{
    watchedFileList = fileList;
}

QString ProjectConfiguration::getCurrentProjectFileName()
{
    return currentProjectFileName;
}
void ProjectConfiguration::setCurrentProjectFileName(QString filename)
{
    currentProjectFileName = filename;
}
int ProjectConfiguration::getSavingTimeDelay()
{
    return savingTimeDelay;
}
void ProjectConfiguration::setSavingTimeDelay(int msec)
{
    savingTimeDelay = msec;
}
