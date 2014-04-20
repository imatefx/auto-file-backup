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

private:
    Ui::AutoFileBackup *ui;
    int newLogTableRow();
    void newWatchedFilesTableRow(QString fileName);
    void addLog(QString statusText, QString  value,QString logLevel);
    void addNewWatchFile(QString  file);
};

#endif // AUTOFILEBACKUP_H
