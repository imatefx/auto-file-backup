#include "filecopysettings.h"
#include <QSettings>

FileCopySettings::FileCopySettings()
{

}

//FileCopySettings::FileCopySettings(bool saveToDiffDir,QString destinationDir,QString prefixString,String suffixString,bool suffixDate,QString suffixDateFormat,QString suffixAfterDateTime)
//{
//    this->SaveToDiffDir       = saveToDiffDir;
//    this->DestinationDir      = destinationDir;
//    this->PrefixString        = prefixString;
//    this->SuffixString        = suffixString;
//    this->SuffixDate          = suffixDate;
//    this->SuffixDateFormat    = suffixDateFormat;
//    this->SuffixAfterDateTime = suffixAfterDateTime;

//}


void FileCopySettings::saveSettings()
{
    QSettings fcSettings;

    fcSettings.setValue("FileCopySettings/SaveToDiffDir" , SaveToDiffDir);
    fcSettings.setValue("FileCopySettings/DestinationDir" , DestinationDir);
    fcSettings.setValue("FileCopySettings/SaveToSubDir" , SaveToSubDir);
    fcSettings.setValue("FileCopySettings/PrefixString" , PrefixString);
    fcSettings.setValue("FileCopySettings/SuffixString" , SuffixString);
    fcSettings.setValue("FileCopySettings/SuffixDate" , HasSuffixDate);
    fcSettings.setValue("FileCopySettings/SuffixDateFormat" , SuffixDateFormat);
    fcSettings.setValue("FileCopySettings/SuffixAfterDateTime" , SuffixAfterDateTime);

}

void FileCopySettings::readSettings()
{
    QSettings fcSettings;

    SaveToDiffDir       = fcSettings.value("FileCopySettings/SaveToDiffDir",false).toBool();
    DestinationDir      = fcSettings.value("FileCopySettings/DestinationDir","").toString();
    SaveToSubDir        = fcSettings.value("FileCopySettings/SaveToSubDir","").toString();
    PrefixString        = fcSettings.value("FileCopySettings/PrefixString","").toString();
    SuffixString        = fcSettings.value("FileCopySettings/SuffixString","").toString();
    HasSuffixDate          = fcSettings.value("FileCopySettings/SuffixDate",true).toBool();
    SuffixDateFormat    = fcSettings.value("FileCopySettings/SuffixDateFormat","d-MM-yyyy_HH-mm-ss-zzz").toString();
    SuffixAfterDateTime = fcSettings.value("FileCopySettings/SuffixAfterDateTime","").toString();

}


bool FileCopySettings::getSaveToDiffDir()
{
    return SaveToDiffDir;
}

QString FileCopySettings::getDestinationDir()
{
    return DestinationDir;
}

QString FileCopySettings::getSaveToSubDir()
{
    return SaveToSubDir;
}

QString FileCopySettings::getPrefixString()
{
    return PrefixString;
}

QString FileCopySettings::getSuffixString()
{
    return SuffixString;
}

bool FileCopySettings::getHasSuffixDate()
{
    return HasSuffixDate;
}

QString FileCopySettings::getSuffixDateFormat()
{
    return SuffixDateFormat;
}

QString FileCopySettings::getSuffixAfterDateTime()
{
    return SuffixAfterDateTime;
}

void FileCopySettings::setSaveToDiffDir(bool inputData)
{
    SaveToDiffDir = inputData;
}

void FileCopySettings::setSaveToSubDir(QString inputData)
{
    SaveToSubDir = inputData;
}

void FileCopySettings::setDestinationDir(QString inputData)
{
    DestinationDir = inputData;
}

void FileCopySettings::setPrefixString(QString inputData)
{
    PrefixString = inputData;
}

void FileCopySettings::setSuffixString(QString inputData)
{
    SuffixString = inputData;
}

void FileCopySettings::setHasSuffixDate(bool inputData)
{
    HasSuffixDate = inputData;
}

void FileCopySettings::setSuffixDateFormat(QString inputData)
{
    SuffixDateFormat = inputData;
}

void FileCopySettings::setSuffixAfterDateTime(QString inputData)
{
    SuffixAfterDateTime = inputData;
}
