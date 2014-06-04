#ifndef PROJECTCONFIGURATION_H
#define PROJECTCONFIGURATION_H
#include <QStringList>
#include <filecopysettings.h>

class ProjectConfiguration
{
private:
QStringList watchedFileList;
FileCopySettings fileCopySettings;

public:
    ProjectConfiguration();
    void saveToFile(QString filename);
    void openFromFile(QString filename);
    void setWatchedFileList(QStringList fileList);
    void setFileCopySettings(FileCopySettings settings);
    QStringList getWatchedFileList();
    FileCopySettings getFileCopySettings();
};

#endif // PROJECTCONFIGURATION_H
