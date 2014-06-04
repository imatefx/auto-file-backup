#ifndef FILECOPYSETTINGS_H
#define FILECOPYSETTINGS_H

#include <QString>

class FileCopySettings
{
private:
    bool SaveToDiffDir;
    QString DestinationDir;
    QString SaveToSubDir;
    QString PrefixString;
    QString SuffixString;
    bool HasSuffixDate;
    QString SuffixDateFormat;
    QString SuffixAfterDateTime;
public:
    FileCopySettings();
//    FileCopySettings(bool saveToDiffDir, QString destinationDir, QString prefixString, String suffixString, bool suffixDate, QString suffixDateFormat, QString suffixAfterDateTime);
    void saveSettings();
    void readSettings();
    bool getSaveToDiffDir();
    QString getDestinationDir();
    QString getSaveToSubDir();
    QString getPrefixString();
    QString getSuffixString();
    bool getHasSuffixDate();
    QString getSuffixDateFormat();
    QString getSuffixAfterDateTime();

    void setSaveToDiffDir(bool inputData);
    void setDestinationDir(QString inputData);
    void setSaveToSubDir(QString inputData);
    void setPrefixString(QString inputData);
    void setSuffixString(QString inputData);
    void setHasSuffixDate(bool inputData);
    void setSuffixDateFormat(QString inputData);
    void setSuffixAfterDateTime(QString inputData);

};

#endif // FILECOPYSETTINGS_H
