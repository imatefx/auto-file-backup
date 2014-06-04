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

     xmlWriter.writeTextElement("saveToDiffDir", QString::number(this->fileCopySettings.getSaveToDiffDir()));
     xmlWriter.writeTextElement("destinationDir", this->fileCopySettings.getDestinationDir());
     xmlWriter.writeTextElement("prefixString", this->fileCopySettings.getPrefixString());
     xmlWriter.writeTextElement("suffixString", this->fileCopySettings.getSuffixString());
     xmlWriter.writeTextElement("suffixDate", QString::number(this->fileCopySettings .getHasSuffixDate()));
     xmlWriter.writeTextElement("suffixDateFormat", this->fileCopySettings.getSuffixDateFormat());
     xmlWriter.writeTextElement("suffixAfterDateTime", this->fileCopySettings.getSuffixAfterDateTime());

     xmlWriter.writeEndElement();

     xmlWriter.writeEndElement();
     xmlWriter.writeEndDocument();

     file.close();
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
            else if(Rxml.name() == "File")
            {
               watchedFileList.append(Rxml.readElementText());
//               qDebug() <<"Filelist " << watchedFileList;
               Rxml.readNext();
            }
            else if(Rxml.name() == "FileCopySettings")
            {
               Rxml.readNext();
            }
            else if(Rxml.name() == "saveToDiffDir")
            {
//               qDebug() <<"saveToDiffDir " << Rxml.readElementText();
               fileCopySettings.setSaveToDiffDir(Rxml.readElementText().toInt());
               Rxml.readNext();
            }
            else if(Rxml.name() == "destinationDir")
            {
//               qDebug() <<"destinationDir " << Rxml.readElementText();
               fileCopySettings.setDestinationDir(Rxml.readElementText());
               Rxml.readNext();
            }
            else if(Rxml.name() == "prefixString")
            {
//               qDebug() <<"prefixString " << Rxml.readElementText();
               fileCopySettings.setPrefixString(Rxml.readElementText());
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixString")
            {
//               qDebug() <<"suffixString " << Rxml.readElementText();
               fileCopySettings.setSuffixString(Rxml.readElementText());
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixDate")
            {
//               qDebug() <<"suffixDate " << Rxml.readElementText();
               fileCopySettings.setHasSuffixDate(Rxml.readElementText().toInt());
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixDateFormat")
            {
//               qDebug() <<"suffixDateFormat " << Rxml.readElementText();
               fileCopySettings.setSuffixDateFormat(Rxml.readElementText());
               Rxml.readNext();
            }
            else if(Rxml.name() == "suffixAfterDateTime")
            {
//               qDebug() <<"suffixAfterDateTime " << Rxml.readElementText();
               fileCopySettings.setSuffixAfterDateTime(Rxml.readElementText());
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
