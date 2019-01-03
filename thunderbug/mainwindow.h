#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QString>
#include <QStringListModel>
#include <QFileSystemModel>
#include <QList>
#include <QProcess>
#include <QFileInfo>
#include "wtreplay.h"
#include "thunderbug.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update_dxdiag_info();

    void setTabElement(int tab, thunderbug::WTReportElementStatus status);

    QString get_bugdescription();

    int get_selected_difficulty();

    int get_selected_gamemode();

    QString get_extra_gamemode();

private slots:
    void on_GenerateButton_clicked();

    void on_BugSelectButton_currentIndexChanged(int index);

    void on_radioButton_8_toggled(bool checked);

    void on_addScreenshotButton_clicked();

    void on_removeScreenshotButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_addReplayButton_clicked();

    void on_removeReplayButton_clicked();

    void on_scanReplayButton_clicked();

    void on_addgameLogButton_clicked();

    void on_removeGameLogButton_clicked();

    void on_addGameLogfromReplayButton_clicked();

    void on_scanGameLogButton_clicked();

    void on_addLauncherLogButton_clicked();

    void on_removLauncherLogButton_clicked();

    void on_scanLauncherLogButton_clicked();

    void on_textEdit_textChanged();

    void on_filterReplaycheckBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

extern MainWindow* w;

#endif // MAINWINDOW_H
