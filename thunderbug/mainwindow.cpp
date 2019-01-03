#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QButtonGroup>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thunderbug.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->availableReplay_listView->setModel(thbg->av_replay_list_model);
    ui->availableReplay_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->addedReplay_listView->setModel(thbg->se_replay_list_model);
    ui->addedReplay_listView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ui->availableGameLog_listView->setModel(thbg->av_clog_list_model);
    ui->availableGameLog_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->addedGameLog_listView->setModel(thbg->se_clog_list_model);
    ui->addedGameLog_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->availableLauncherLog_listView->setModel(thbg->av_llog_list_model);
    ui->availableLauncherLog_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->addedLauncherLog_listView->setModel(thbg->se_llog_list_model);
    ui->addedLauncherLog_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->screenchotslistView->setModel(thbg->screenshots_list_model);
    ui->screenchotslistView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // init tabbar with general gameplay and balance
    ui->BugSelectButton->setCurrentIndex(0);
    for(int i=0; i<NB_REPORT_ELMT; i++)
    {
        setTabElement(i,thbg->ReportElementtable[0][i]);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GenerateButton_clicked()
{
    if(thbg->create_new_bugreport())
    {
        QMessageBox msgbox;
        msgbox.setText("Bug report created.");

        QPushButton *msgbox_startnewButton = msgbox.addButton("start new", QMessageBox::ActionRole);
        QPushButton *msgbox_opendirButton = msgbox.addButton("open dir", QMessageBox::ActionRole);
        QPushButton *msgbox_finishButton = msgbox.addButton("finish", QMessageBox::ActionRole);

        msgbox.exec();

        if (msgbox.clickedButton() == msgbox_finishButton) {
            exit(0);
        } else if (msgbox.clickedButton() == msgbox_startnewButton) {

        } else if (msgbox.clickedButton() == msgbox_opendirButton) {
            QDesktopServices::openUrl(thbg->OutputDir->path());
            //exit(0);
        }
    }
    else
    {
        QMessageBox::warning(
            this,
            "ThunderBug",
            "Bug report incomplete !" );
    }
}

/*
 * BUG REPORT TYPE CHANGE
 * */
void MainWindow::on_BugSelectButton_currentIndexChanged(int index)
{
    for(int i=0; i<NB_REPORT_ELMT; i++)
    {
        setTabElement(i,thbg->ReportElementtable[index][i]);
    }
    thbg->BugReportType = static_cast<thunderbug::WTbugType >(index);
    memcpy(thbg->BugReportStatus, thbg->ReportElementtable[0], NB_REPORT_ELMT);
    this->show();
}

void MainWindow::setTabElement(int tab, thunderbug::WTReportElementStatus status)
{
    switch(status)
    {
    case thunderbug::WTRES_DISABLED:
        ui->tabWidget->setTabEnabled(tab,0);
		ui->tabWidget->tabBar()->setTabTextColor(tab,QColor(128,128,128));
        break;
    case thunderbug::WTRES_OPTIONAL:
        ui->tabWidget->setTabEnabled(tab,1);
		ui->tabWidget->tabBar()->setTabTextColor(tab,QColor(0,0,0));
        break;
    case thunderbug::WTRES_MANDATORY:
        ui->tabWidget->setTabEnabled(tab,1);
		ui->tabWidget->tabBar()->setTabTextColor(tab,QColor(255,0,0));
        break;
    case thunderbug::WTRES_COMPLETED:
        ui->tabWidget->setTabEnabled(tab,1);
		ui->tabWidget->tabBar()->setTabTextColor(tab,QColor(0,128,0));
        break;
    }
}

/*
 * GAME MODE
 * */
void MainWindow::on_radioButton_8_toggled(bool checked)
{
    if(checked) ui->customgametype->setEnabled(1);
    else ui->customgametype->setEnabled(0);
}

/*
 * ADD / REMOVE SCREENSHOTS
 * */
void MainWindow::on_addScreenshotButton_clicked()
{
    QStringList addedscreenshotlist = QFileDialog::getOpenFileNames(this, "select file(s)", thbg->WarThunderscreenshotsDir->path(), "Images (*.png *.xpm *.jpg)");

    for(int i=0 ; i<addedscreenshotlist.size() ; i++)
    {
        QFile* newscreenshot = new QFile(addedscreenshotlist.at(i));
        if(newscreenshot->exists())
            thbg->screenshots_list_model->setStringList(thbg->screenshots_list_model->stringList()+QStringList(newscreenshot->fileName()));
        else
            QMessageBox::warning(
                this,
                "File not found",
                addedscreenshotlist.at(i));

        newscreenshot->~QFile();
    }
}

void MainWindow::on_removeScreenshotButton_clicked()
{
    int row = ui->screenchotslistView->currentIndex().row();

    thbg->screenshots_list_model->removeRow(row);
}

/*
 * CHANGE OUTPUT DIRECTORY
 * */
void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory",
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty())
        thbg->OutputDir->setPath(dir);
}

/*
 * DXDIAG
 * */

void MainWindow::on_pushButton_2_clicked()
{
    thbg->get_dxdiaglog();

    update_dxdiag_info();
}

void MainWindow::update_dxdiag_info()
{
    // look for the file in thunderbug dir
    QFileInfo dxdiagfileinfo(thbg->ThunderbugDir->path()+"\\dxdiag.txt");

    if(dxdiagfileinfo.exists() && dxdiagfileinfo.isFile()) {
        QString dxdiagname = dxdiagfileinfo.absoluteFilePath();
        QString dxdiagtime = dxdiagfileinfo.lastModified().toString();
        QString msg = "dxdiag file found : "+dxdiagname+"\nfrom : "+dxdiagtime+"\nIf your computer config changed since, please generate a new one.";
        ui->dxdiaginfo->setText(msg);
    }
    else
        ui->dxdiaginfo->setText("no dxdiag file found");
}

/*
 * ADD / REMOVE REPLAYS
 * */
void MainWindow::on_addReplayButton_clicked()
{
    WTreplay* new_replay = new WTreplay(thbg->av_replay_list_model->get_replay(ui->availableReplay_listView->currentIndex())->File->fileName());

    for(int i=0; i<thbg->se_replay_list_model->rowCount(); i++)
    {
        if(thbg->se_replay_list_model->get_replay(i)->File->fileName() == new_replay->File->fileName())
        {
            new_replay->~WTreplay();
            return;
        }
    }
    thbg->se_replay_list_model->addreplay(new_replay);
}

void MainWindow::on_removeReplayButton_clicked()
{
    if(thbg->se_replay_list_model->rowCount())
    {
        thbg->se_replay_list_model->delete_replay(ui->addedReplay_listView->currentIndex());
    }
}

void MainWindow::on_filterReplaycheckBox_toggled(bool checked)
{
    if(checked)
    {
        QList<int> replays_to_delete;
        // for each replay in the "availlable replays" model
        for(int i=0; i<thbg->av_replay_list_model->rowCount(); i++)
        {
            // if same mode and type
            /*if(thbg->av_replay_list_model->get_replay(i)->GameMode == get_selected_gamemode()
                    && thbg->av_replay_list_model->get_replay(i)->Difficulty == get_selected_difficulty())
            {
                // we keep it
            }
            else
            {
                thbg->av_replay_list_model->delete_replay(i);
                i--;
            }*/
        }
    }
    else
    {
        thbg->get_replay_list();
    }
}

void MainWindow::on_scanReplayButton_clicked()
{
    thbg->get_replay_list();
}

/*
 * ADD / REMOVE CLIENT LOG
 * */
void MainWindow::on_addgameLogButton_clicked()
{
    QStringList newclog = thbg->av_clog_list_model->data(ui->availableGameLog_listView->currentIndex()).toStringList();

	// check if file is not already added
    if(thbg->se_clog_list_model->stringList().contains(newclog.at(0)))
    {
        return;
    }

    thbg->se_clog_list_model->setStringList(thbg->se_clog_list_model->stringList()+newclog);
}

void MainWindow::on_removeGameLogButton_clicked()
{
    thbg->se_clog_list_model->removeRows(ui->addedGameLog_listView->currentIndex().row(),1);
}

void MainWindow::on_addGameLogfromReplayButton_clicked()
{
    for(int i=0; i<thbg->se_replay_list_model->rowCount(); i++)
    {
        QFile* clogfile = thbg->get_coresponding_clog(thbg->se_replay_list_model->get_replay(i));
        if(clogfile!=nullptr)
        {
            QString clog = QFileInfo(*clogfile).fileName();
            //qDebug() << "clogfound : "+clog;
            if(!thbg->se_clog_list_model->stringList().contains(clog))
                thbg->se_clog_list_model->setStringList(thbg->se_clog_list_model->stringList()+QStringList(clog));
        }
        clogfile->~QFile();
    }
    //qDebug() << thbg->se_clog_list_model->rowCount() << "clog";
}

void MainWindow::on_scanGameLogButton_clicked()
{
    thbg->get_clog_list();
}

/*
 * ADD / REMOVE LAUNCHERS LOG
 * */
void MainWindow::on_addLauncherLogButton_clicked()
{
    QStringList newllog = thbg->av_llog_list_model->data(ui->availableLauncherLog_listView->currentIndex()).toStringList();

    if(thbg->se_llog_list_model->stringList().contains(newllog.at(0)))
    {
        return;
    }

    thbg->se_llog_list_model->setStringList(thbg->se_llog_list_model->stringList()+newllog);
}

void MainWindow::on_removLauncherLogButton_clicked()
{
    thbg->se_llog_list_model->removeRows(ui->addedLauncherLog_listView->currentIndex().row(),1);
}

void MainWindow::on_scanLauncherLogButton_clicked()
{
    thbg->get_llog_list();
}

/*
 * BUG DESCRIPTION
 * */
void MainWindow::on_textEdit_textChanged()
{

}

QString MainWindow::get_bugdescription()
{
    return ui->textEdit->toPlainText();
}

/*
 * GAME MODE / DIFFICUMTY
 * */
int MainWindow::get_selected_difficulty()
{
    // fetch Game mode button
    QButtonGroup DiffGroup(this);
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
    {
        DiffGroup.addButton(allButtons[i],i);
    }
    qDebug() << "diff : " << DiffGroup.checkedId();
    return DiffGroup.checkedId();
}

int MainWindow::get_selected_gamemode()
{
    // fetch Game type button
    QButtonGroup ModeGroup(this);
    QList<QRadioButton *> allButtons = ui->groupBox_2->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
    {
        ModeGroup.addButton(allButtons[i],i);
    }
    qDebug() << "mode : " << ModeGroup.checkedId();
    return ModeGroup.checkedId();
}

QString MainWindow::get_extra_gamemode()
{
    return ui->customgametype->text();
}
