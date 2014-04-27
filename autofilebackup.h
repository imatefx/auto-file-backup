#ifndef AUTOFILEBACKUP_H
#define AUTOFILEBACKUP_H

#include <QWidget>
#include <QAbstractButton>
#include <QMimeData>

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

    void on_destinationDirBrowseButton_clicked();

    void on_fileCopyButtonBox_clicked(QAbstractButton *button);

    void on_watchedFilesTableWidget_dropped(const QMimeData *mimeData);

private:
    Ui::AutoFileBackup *ui;
    void addLog(QString statusText, QString  value);
    bool addNewWatchFile(QString  file);
    bool copyFileAsBackup(QString sourceFile, QString destinationDir, QString prefixString, QString suffixString, bool suffixDate, QString suffixDateFormat, QString suffixAfterDateTime);
    void resetFileCopySettings();
    void saveFileCopySettings();
    void loadFileCopySettings();
};

#endif // AUTOFILEBACKUP_H
