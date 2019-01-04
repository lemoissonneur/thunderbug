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

    bool SteamInstall;  /**< true if WarTunder is installed in Steam's directory (not if Steam is installed or not)*/

    QDir* WarThunderMainDir;  /**< WarThunder main directory */
    QDir* WarThunderGameLogDir;  /**< WarThunder /.game_logs directory */
    QDir* WarThunderReplaysDir;  /**< WarThunder /Replays directory */
    QDir* WarThunderlauncherLogDir;  /**< WarThunder /.launcher_log directory */
    QDir* WarThunderscreenshotsDir;  /**< WarThunder /Screenshots directory */
    QDir* ThunderbugDir;  /**< Thunderbug main directory (default path for bug report output and store dxdiag file) */
    QDir* OutputDir;  /**< output directory for the bug report files */

    QFile* WarThunderconfigFile;  /**< WarThunder config file*/

    WTreplaylistModel* av_replay_list_model;  /**< model for the availlables replay list*/
    WTreplaylistModel* se_replay_list_model;  /**< model for the user selected replay list*/

    QStringListModel* av_clog_list_model;  /**< model for the availlables client logs list*/
    QStringListModel* se_clog_list_model;  /**< model for the user selected client logs list*/

    QStringListModel* av_llog_list_model;  /**< model for the availlables launcher logs list*/
    QStringListModel* se_llog_list_model;  /**< model for the user selected launcher logs list*/

    QStringListModel* screenshots_list_model;  /**< model for the user selected screenshots list*/

    /**
     * @brief list of all possible bug type to report
     */
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

    /**
     * @brief list of possible elements needed for a bug report
     */
    enum WTReportElement {
        WTRE_GAMEMODE,
        WTRE_DESCRIPTION,
        WTRE_REPLAYS,
        WTRE_CLIENTLOGS,
        WTRE_LAUNCHERLOGS,
        WTRE_SCREENSHOTS,
        WTRE_DXDIAG
    };

    /**
     * @brief bug type strings
     */
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

    /**
     * @brief possible status of each report element
     */
    enum WTReportElementStatus {
        WTRES_DISABLED, /**< element not needed for the current bug type (tab is dasabled in the UI)*/
        WTRES_MANDATORY, /**< element is needed for the current bug type */
        WTRES_OPTIONAL, /**< element is optional for the current bug type*/
        WTRES_COMPLETED, /**< element have been completed by the user*/
    };

    WTbugType BugReportType;  /**< selected bug type to be reported by the user*/
    WTReportElementStatus BugReportStatus [NB_REPORT_ELMT] = {WTRES_DISABLED}; /**< current status of each elements(tab) */

    /**
     * @brief Rule of which elements are needed depending on the bug report type
     */
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

    /**
     * @brief generate dxdiag.txt log in the thunderbug main directory
     */
    void get_dxdiaglog();

    /**
     * @brief populate 'av_replay_list_model' with the replays files found in WarThunder/Replays directory
     */
    void get_replay_list();

    /**
     * @brief populate 'av_clog_list_model' with the files found in WarThunder/.game_logs directory
     */
    void get_clog_list();

    /**
     * @brief populate 'av_llog_list_model' with the files found in WarThunder/.launcher_log directory
     */
    void get_llog_list();

    /**
     * @brief get client log file corresponding to the same time period of the replay
     * @param replay address
     * @return QFile of the corresponding log (nullptr if failed)
     */
    QFile* get_coresponding_clog(WTreplay* replay);

    /**
     * @brief search for WarThunder directory on the computer and sets all 'WarThunder*Dir' members
     *   we search for the 'launcher.exe' file in 'C:/Program Files/War Thunder' and 'C:\Program Files (x86)\Steam\steamapps\common\War Thunder'
     *   if thoses two fail, we ask the user to select a directory, if we can't find the launcher there either, return false
     * @return bool : false if not found
     */
    bool find_gamedir();

    /**
     * @brief move the user selected files in 'OutputDir' directory and generate bugreport.txt
     * @return bool : false if the bug report is incomplete
     */
    bool create_new_bugreport();
};

extern thunderbug* thbg;

#endif // THUNDERBUG_H
