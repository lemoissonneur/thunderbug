#include "thunderbug.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>
#include <QString>
#include <QTranslator>

#include "mainwindow.h"


thunderbug::thunderbug()
{
    av_replay_list_model = new WTreplaylistModel();
    se_replay_list_model = new WTreplaylistModel();

    av_clog_list_model = new QStringListModel();
    se_clog_list_model = new QStringListModel();

    av_llog_list_model = new QStringListModel();
    se_llog_list_model = new QStringListModel();

    screenshots_list_model = new QStringListModel();

    BugReportType = thunderbug::WTBT_general_gameplay_and_balance;  // default report type in the UI
}

void thunderbug::get_dxdiaglog()
{
    // TODO : make it work with '/64bit' we need to compile in 64bit
    QProcess* dxdiagprocess;
    dxdiagprocess = new QProcess();
    qDebug() << "dxdiag generating ... ";
    dxdiagprocess->start("C:\\Windows\\system32\\dxdiag.exe /whql:on /t"+thbg->ThunderbugDir->path()+"\\dxdiag.txt");
    dxdiagprocess->waitForFinished(-1);
    qDebug() << "done";
}

void thunderbug::get_replay_list()
{
    // get all .wrpl file in the /Replays directory
    QStringList nameFilter("*.wrpl");
    QStringList filelist = WarThunderReplaysDir->entryList(nameFilter);

    av_replay_list_model->reset();

    foreach (QString filename, filelist) {
        WTreplay* replay = new WTreplay(WarThunderReplaysDir->path()+"\\"+filename);
        av_replay_list_model->addreplay(replay);
    }

    qDebug() << av_replay_list_model->rowCount() << "replays found";
}

void thunderbug::get_clog_list()
{
    // get all .clog file in the /gamelogs directory
    QStringList nameFilter("*.clog");
    QStringList filelist = WarThunderGameLogDir->entryList(nameFilter);

    av_clog_list_model->setStringList(filelist);

    qDebug() << av_clog_list_model->stringList().size() << "clog found.";
}

void thunderbug::get_llog_list()
{
    QStringList nameFilter("*.txt");
    QStringList filelist = WarThunderlauncherLogDir->entryList(nameFilter);

    av_llog_list_model->setStringList(filelist);

    qDebug() << av_llog_list_model->stringList().size() << "llog found.";
}

QFile* thunderbug::get_coresponding_clog(WTreplay* replay)
{
    QFile* clog_file = nullptr;

    for(int i=0 ; i<av_clog_list_model->stringList().size() ; i++)
    {
        QFileInfo clfi(WarThunderGameLogDir->path()+"/"+av_clog_list_model->stringList().at(i));

        if(clfi.birthTime() < replay->start && clfi.lastModified() > replay->finish)
        {
            clog_file = new QFile(clfi.filePath()+"/"+clfi.fileName());
            break;
        }
    }

    return clog_file;
}

bool thunderbug::find_gamedir()
{
    SteamInstall = false;
    QFileInfo SteamFile("C:\\Program Files (x86)\\Steam\\steamapps\\common\\War Thunder\\launcher.exe");
    QFileInfo StandAloneFile("C:\\Program Files (x86)\\WarThunder\\launcher.exe");

    if(SteamFile.exists() && SteamFile.isFile())
    {
        WarThunderMainDir = new QDir("C:\\Program Files (x86)\\Steam\\steamapps\\common\\War Thunder");
        SteamInstall = true;
    }
    else if(StandAloneFile.exists() && StandAloneFile.isFile())
    {
        WarThunderMainDir = new QDir("C:\\Program Files (x86)\\WarThunder");
    }
    else
    {
        QMessageBox::warning(
            w,
            "ThunderBug",
            "War Thunder directory not found. Please select directory." );

        QString customdir = QFileDialog::getExistingDirectory(w, "Open Directory",
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

        QFileInfo customFile(customdir+"\\launcher.exe");
        if(customFile.exists() && customFile.isFile() )
        {
            WarThunderMainDir = new QDir(customdir);
        }
        else
        {
            QMessageBox::warning(
                w,
                "ThunderBug",
                "War Thunder game not found." );

            return 0;
        }
    }

	// set up games sub directory
    WarThunderGameLogDir = new QDir(WarThunderMainDir->path()+"\\.game_logs");
    WarThunderReplaysDir = new QDir(WarThunderMainDir->path()+"\\Replays");
    WarThunderlauncherLogDir = new QDir(WarThunderMainDir->path()+"\\.launcher_log");
    WarThunderscreenshotsDir = new QDir(WarThunderMainDir->path()+"\\Screenshots");

    WarThunderconfigFile = new QFile(WarThunderMainDir->path()+"\\config.blk");

	// set up thunderbug directory (for output)
    QDir* root = new QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    ThunderbugDir = new QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"\\thunderbug");
    if(!ThunderbugDir->exists()) root->mkdir("thunderbug");

    OutputDir = new QDir(ThunderbugDir->path());

    qDebug() << "MainDir = " << WarThunderMainDir->path();
    qDebug() << "ClogDir = " << WarThunderGameLogDir->path();
    qDebug() << "ReplayDir = " << WarThunderReplaysDir->path();
    qDebug() << "LLogDir = " << WarThunderlauncherLogDir->path();
    qDebug() << "ThBgDir = " << ThunderbugDir->path();

    return 1;
}

bool thunderbug::create_new_bugreport()
{
    // set up the output directory
    QString bugreportname = "report_" + QString::number(BugReportType) + QDateTime::currentDateTime().toString("___dd-MM__HH-mm");
    //qDebug() << bugreportname;
    OutputDir->mkdir(bugreportname);
    OutputDir->setPath(OutputDir->path()+"\\"+bugreportname);
    qDebug() << OutputDir->path();

    int i = 0;
    int cpt = 0;

    // fetch replays
    if(ReportElementtable[BugReportType][thunderbug::WTRE_REPLAYS] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_REPLAYS] == WTRES_OPTIONAL)
    {
        cpt = 0;
        for(i=0; i<se_replay_list_model->rowCount(); i++)
        {
            QFileInfo replayname(*se_replay_list_model->get_replay(i)->File);
            qDebug() << "adding : " << OutputDir->path()+"/"+replayname.fileName();
            se_replay_list_model->get_replay(i)->File->copy(OutputDir->path()+"/"+replayname.fileName());
            cpt ++;
        }
        if(cpt)
        {
            BugReportStatus[WTRE_REPLAYS] = WTRES_COMPLETED;
            w->setTabElement(WTRE_REPLAYS,WTRES_COMPLETED);
        }
    }

    // fetch clog
    if(ReportElementtable[BugReportType][thunderbug::WTRE_CLIENTLOGS] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_CLIENTLOGS] == WTRES_OPTIONAL)
    {
        cpt = 0;
        for(int i=0; i<se_clog_list_model->rowCount(); i++)
        {
            QString clogname(se_clog_list_model->stringList().at(i));
            QFile clogfile(WarThunderGameLogDir->path()+"/"+clogname);
            qDebug() << "adding : " << OutputDir->path()+"/"+clogname;
            clogfile.copy(OutputDir->path()+"/"+clogname);
            cpt ++;
        }
        if(cpt)
        {
            BugReportStatus[WTRE_CLIENTLOGS] = WTRES_COMPLETED;
            w->setTabElement(WTRE_CLIENTLOGS,WTRES_COMPLETED);
        }
    }

    // fetch llog
    if(ReportElementtable[BugReportType][thunderbug::WTRE_LAUNCHERLOGS] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_LAUNCHERLOGS] == WTRES_OPTIONAL)
    {
        cpt = 0;
        for(int i=0; i<se_llog_list_model->rowCount(); i++)
        {
            QString llogname(se_llog_list_model->stringList().at(i));
            QFile llogfile(WarThunderlauncherLogDir->path()+"/"+llogname);
            qDebug() << "adding : " << OutputDir->path()+"/"+llogname;
            llogfile.copy(OutputDir->path()+"/"+llogname);
            cpt ++;
        }
        if(cpt)
        {
            BugReportStatus[WTRE_LAUNCHERLOGS] = WTRES_COMPLETED;
            w->setTabElement(WTRE_LAUNCHERLOGS,WTRES_COMPLETED);
        }
    }

    // fetch screenshots
    if(ReportElementtable[BugReportType][thunderbug::WTRE_SCREENSHOTS] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_SCREENSHOTS] == WTRES_OPTIONAL)
    {
        cpt = 0;
        for(int i=0; i<screenshots_list_model->rowCount(); i++)
        {
            QFile scfile(screenshots_list_model->stringList().at(i));
            QFileInfo scfileinfo(scfile);
            qDebug() << "adding : " << OutputDir->path()+"/"+scfileinfo.fileName();
            scfile.copy(OutputDir->path()+"/"+scfileinfo.fileName());
            cpt ++;
        }
        if(cpt)
        {
            BugReportStatus[WTRE_SCREENSHOTS] = WTRES_COMPLETED;
            w->setTabElement(WTRE_SCREENSHOTS,WTRES_COMPLETED);
        }
    }

    // fetch dxdiag
    if(ReportElementtable[BugReportType][thunderbug::WTRE_DXDIAG] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_DXDIAG] == WTRES_OPTIONAL)
    {
        QFileInfo dxdiagfileinfo(ThunderbugDir->path()+"\\dxdiag.txt");

        if(dxdiagfileinfo.exists() && dxdiagfileinfo.isFile())
        {
            QFile dxdiagfile(ThunderbugDir->path()+"\\dxdiag.txt");
            dxdiagfile.copy(OutputDir->path()+dxdiagfile.fileName());
            BugReportStatus[WTRE_DXDIAG] = WTRES_COMPLETED;
            w->setTabElement(WTRE_DXDIAG,WTRES_COMPLETED);
        }
        else if(ReportElementtable[BugReportType][thunderbug::WTRE_DXDIAG] == WTRES_OPTIONAL)
        {
            BugReportStatus[WTRE_DXDIAG] = WTRES_COMPLETED;
            w->setTabElement(WTRE_DXDIAG,WTRES_COMPLETED);
        }
    }

    // fetch difficulty and game mode
    QString GameDiffMode;
    if(ReportElementtable[BugReportType][thunderbug::WTRE_GAMEMODE] == WTRES_MANDATORY
    || ReportElementtable[BugReportType][thunderbug::WTRE_GAMEMODE] == WTRES_OPTIONAL)
    {
        if(w->get_selected_difficulty() >=0 && w->get_selected_gamemode() >=0)
        {
            GameDiffMode =
                    "Difficulty : " + WTDifficulty_str[w->get_selected_difficulty()] + "\r\n" +
                    "Game Mode : " + ( w->get_selected_gamemode()<4 ? WTGameMode_str[w->get_selected_gamemode()] : w->get_extra_gamemode() ) + "\r\n";

            BugReportStatus[WTRE_GAMEMODE] = WTRES_COMPLETED;
            w->setTabElement(WTRE_GAMEMODE,WTRES_COMPLETED);
        }
    }

    // format description (we dont check status because description is always needed)
    QFile reportfile(OutputDir->path()+"/bugreport.txt");
    QString desc = w->get_bugdescription();

    if(1) // check if description is valid
    {
        if(reportfile.open(QIODevice::WriteOnly))
        {
            QTextStream report_text(&reportfile);
            report_text << "THUNDERBUG REPORT" << "\r\n"
                        << "created : " << "\r\n"
                        << "Bug section : " << WTbugType_str[BugReportType] << "\r\n"
                        << GameDiffMode
                        << "Description : " << desc << "\r\n"
                        << endl;
            reportfile.close();

            BugReportStatus[WTRE_DESCRIPTION] = WTRES_COMPLETED;
            w->setTabElement(WTRE_DESCRIPTION,WTRES_COMPLETED);
        }
    }


    // final check
    for(i=0; i<NB_REPORT_ELMT; i++)
    {
        // if one elements is still needed cancel the report
        if(BugReportStatus[i] == WTRES_MANDATORY)
        {
            OutputDir->removeRecursively();
            OutputDir = new QDir(ThunderbugDir->path());
            qDebug() << "Bug report incomplete, remove all (failed at element "<<i<<")";
            return false;
        }
    }

    // bug report success
    return true;
}
