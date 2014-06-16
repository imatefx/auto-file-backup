#ifndef AUTOFILEBACKUP_H
#define AUTOFILEBACKUP_H

#include <QWidget>
#include <QAbstractButton>
#include <QMimeData>
#include <filecopysettings.h>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFileSystemWatcher>
#include <projectconfiguration.h>

namespace Ui {
class AutoFileBackup;
}

class AutoFileBackup : public QWidget
{
    Q_OBJECT

public:
    explicit AutoFileBackup(QWidget *parent = 0);
    ~AutoFileBackup();
    bool createConnection();

private slots:
    void on_addNewFileButton_clicked();

    void on_removeFileButton_clicked();

    void directoryChanged(const QString &);

    void delayedDirectoryChanged();

    void fileChanged(const QString & path);

    void delayedFileChanged();

//    void on_saveToDiffDirCheckBox_stateChanged(int arg1);

    void on_suffixDateTimeCheckBox_stateChanged(int arg1);

    void on_destinationDirBrowseButton_clicked();

//    void on_fileCopyButtonBox_clicked(QAbstractButton *button);

    void on_watchedFilesTableWidget_dropped(const QMimeData *mimeData);

    void on_saveProjectButton_clicked();

    void on_openProjectButton_clicked();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);

    void showHideWindow();

    void on_createDateFolderCheckBox_stateChanged(int arg1);

    void on_previewButton_clicked();

    void on_saveProjectAsButton_clicked();

    void on_isSavingToDifferentFolderRadioButton_toggled(bool checked);

    void on_clearLogButton_clicked();

private:
    Ui::AutoFileBackup *ui;
    void addLog(QString statusText, QString  value);
    bool addNewWatchFile(QString  file);
    bool copyFileAsBackup(QString sourceFile, QString destinationDir, QString prefixString, QString suffixString, bool suffixDate, QString suffixDateFormat, QString suffixAfterDateTime);
    void resetFileCopySettings();
    FileCopySettings getFileCopySettings();
    void setFileCopySettings(FileCopySettings fcSettings);
    void insertToWatchTable(QString file);
    void setDropOverlay();

//    // Tray Icon Functions
    void createTrayActions();
    void createTrayIcon();
    void setTrayIcon();

    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *);

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *showHideTray;
    QAction *closeTray;

    QFileSystemWatcher *folderMonitor;
    QFileSystemWatcher *fileMonitor;
    QList<QString> watchedFiles;
    QStringList changedFileQueue;
    ProjectConfiguration projectConfg;
};

#endif // AUTOFILEBACKUP_H
