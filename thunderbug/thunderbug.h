#ifndef THUNDERBUG_H
#define THUNDERBUG_H
#include <QDir>
#include <QStringListModel>

#include "wtreplay.h"

#define NB_REPORT_TYPE 9
#define NB_REPORT_ELMT 7

class thunderbug
{
public:
    thunderbug();

    bool SteamInstall;

    QDir* WarThunderMainDir;
    QDir* WarThunderGameLogDir;
    QDir* WarThunderReplaysDir;
    QDir* WarThunderlauncherLogDir;
    QDir* WarThunderscreenshotsDir;
    QDir* ThunderbugDir;
    QDir* OutputDir;

    QFile* WarThunderconfigFile;

    WTreplaylistModel* av_replay_list_model;
    WTreplaylistModel* se_replay_list_model;

    QStringListModel* av_clog_list_model;
    QStringListModel* se_clog_list_model;

    QStringListModel* av_llog_list_model;
    QStringListModel* se_llog_list_model;

    QStringListModel* screenshots_list_model;

    enum WTbugType {
        WTBT_general_gameplay_and_balance,
        WTBT_client_and_launcher_crashes,
        WTBT_graphics_and_devices,
        WTBT_UI_text_and_localisation,
        WTBT_game_controls_and_devices,
        WTBT_maps_missions_and_AI,
        WTBT_flight_and_damage_model,
        WTBT_sound_and_devices,
        WTBT_connection_and_updates
    };

    enum WTReportElement {
        WTRE_GAMEMODE,
        WTRE_DESCRIPTION,
        WTRE_REPLAYS,
        WTRE_CLIENTLOGS,
        WTRE_LAUNCHERLOGS,
        WTRE_SCREENSHOTS,
        WTRE_DXDIAG
    };

    const QString WTbugType_str[NB_REPORT_TYPE] = {        "general_gameplay_and_balance",
                                                           "client_and_launcher_crashes",
                                                           "graphics_and_devices",
                                                           "UI_text_and_localisation",
                                                           "game_controls_and_devices",
                                                           "maps_missions_and_AI",
                                                           "flight_and_damage_model",
                                                           "sound_and_devices",
                                                           "connection_and_updates"
	};

    enum WTReportElementStatus {
        WTRES_DISABLED,
        WTRES_MANDATORY,
        WTRES_OPTIONAL,
        WTRES_COMPLETED,
    };

    WTbugType BugReportType;
    WTReportElementStatus BugReportStatus [NB_REPORT_ELMT] = {WTRES_DISABLED};

    // Rules of which elements are needed depending on the bug report type
    const WTReportElementStatus ReportElementtable[NB_REPORT_TYPE][NB_REPORT_ELMT] = {
        //    GAMEMODE           DESC                REPLAY              CLOG                LLOG                SCREEN              DXDIAG
        {WTRES_OPTIONAL,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_OPTIONAL  }, //WTBT_general_gameplay_and_balance
        {WTRES_DISABLED,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_OPTIONAL,     WTRES_OPTIONAL,     WTRES_MANDATORY }, //WTBT_client_and_launcher_crashes
        {WTRES_DISABLED,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_MANDATORY,    WTRES_MANDATORY }, //WTBT_graphics_and_devices
        {WTRES_DISABLED,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_MANDATORY }, //WTBT_UI_text_and_localisation
        {WTRES_MANDATORY,   WTRES_MANDATORY,    WTRES_OPTIONAL,     WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_MANDATORY }, //WTBT_game_controls_and_devices
        {WTRES_MANDATORY,   WTRES_MANDATORY,    WTRES_OPTIONAL,     WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_OPTIONAL  }, //WTBT_maps_missions_and_AI
        {WTRES_OPTIONAL,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_DISABLED  }, //WTBT_flight_and_damage_model
        {WTRES_OPTIONAL,    WTRES_MANDATORY,    WTRES_OPTIONAL,     WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_DISABLED,     WTRES_MANDATORY }, //WTBT_sound_and_devices
        {WTRES_DISABLED,    WTRES_MANDATORY,    WTRES_DISABLED,     WTRES_OPTIONAL,     WTRES_OPTIONAL,     WTRES_OPTIONAL,     WTRES_MANDATORY }, //WTBT_connection_and_updates
    };

    void get_dxdiaglog();
    void get_replay_list();
    void get_clog_list();
    void get_llog_list();
    QFile* get_coresponding_clog(WTreplay* replay);
    bool find_gamedir();
    bool create_new_bugreport();
};

extern thunderbug* thbg;

#endif // THUNDERBUG_H
