#ifndef AUTOFILEBACKUP_H
#define AUTOFILEBACKUP_H

#include <QWidget>

namespace Ui {
class AutoFileBackup;
}

class AutoFileBackup : public QWidget
{
    Q_OBJECT

public:
    explicit AutoFileBackup(QWidget *parent = 0);
    ~AutoFileBackup();

private slots:
    void on_addNewFileButton_clicked();

    void on_removeFileButton_clicked();

    void directoryChanged(const QString & path);

    void fileChanged(const QString & path);

    void on_saveToDiffDirCheckBox_stateChanged(int arg1);

    void on_suffixDateTimeCheckBox_stateChanged(int arg1);

private:
    Ui::AutoFileBackup *ui;
    void addLog(QString statusText, QString  value,QString logLevel);
    void addNewWatchFile(QString  file);
    bool copyFileAsBackup(QString sourceFile, QString destinationDir, QString prefixString, QString suffixString, bool suffixDate, QString suffixDateFormat, QString suffixAfterDateTime);
};

#endif // AUTOFILEBACKUP_H
