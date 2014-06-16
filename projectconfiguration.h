#ifndef PROJECTCONFIGURATION_H
#define PROJECTCONFIGURATION_H
#include <QStringList>
#include <filecopysettings.h>

class ProjectConfiguration
{
private:
QStringList watchedFileList;
FileCopySettings fileCopySettings;
int savingTimeDelay;
QString currentProjectFileName;

public:
    ProjectConfiguration();
    void saveToFile(QString filename);
    void openFromFile(QString filename);
    void setWatchedFileList(QStringList fileList);
    void setFileCopySettings(FileCopySettings settings);
    void setSavingTimeDelay(int msec);
    int getSavingTimeDelay();
    QStringList getWatchedFileList();
    FileCopySettings getFileCopySettings();
    void setCurrentProjectFileName(QString filename);
    QString getCurrentProjectFileName();
};

#endif // PROJECTCONFIGURATION_H
