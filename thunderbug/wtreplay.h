#ifndef WTREPLAY_H
#define WTREPLAY_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QAbstractListModel>

/**
 * @brief The WTreplay class
 */
class WTreplay
{
public:
    /**
     * @brief create empty replay
     */
    WTreplay();
	
    /**
     * @brief create replay and extract info from the given file ( bool Analyse() is called )
     * @param filepath path of the replay file
     */
    WTreplay(QString filepath);
	
    /**
     * @brief destroy replay
     */
    ~WTreplay();

    /**
     * @brief extract map name, game mode, difficulty, etc ...  from the file
     * @return false if failed to open the file or file is not set
     */
    bool Analyse();
	
    /**
     * @brief return a summary of the replay info we extracted from the file
     * @return QString		: empty if file is not set or Analyse() failed
     */
    QString description();

    QString Name;		/**< replay name (file name + extra info) */
    QFile* File;		/**< replay file */
    
    QDateTime start;	/**< start time of the game (file creation time)*/
    QDateTime finish;	/**< finish time of the game (file last edit time)*/

    uint Mapid;			/**< index in the WTMapList array*/

    QString ServerReplayLink;  /**< url to the server side replay*/
	
private:

    /**
     * @brief findmap read the replay File and try to find the map
     * @return bool : true if map is found (1 <= Mapid <= WTMapCount), false if not (Mapid = 0)
     */
	bool findmap();

};









/**
 * @brief The WTreplaylistModel class
 */
class WTreplaylistModel :public QAbstractListModel
{

public:
    WTreplaylistModel(QObject *parent = nullptr);

    /**
     * @brief return the number of replay in the list
     * @param parent
     * @return number of replay in the list
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief
     * @param index of the replay
     * @param role
     * @return description of the requested replay (QString)
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief
     * @param index of the replay
     * @return pointer to the requested replay
     */
    WTreplay* get_replay(const QModelIndex &index);

    /**
     * @brief
     * @param index of the replay
     * @return pointer to the requested replay
     */
    WTreplay* get_replay(int index);

    /**
     * @brief remove replay from the list
     * @param index of the replay
     */
    void delete_replay(const QModelIndex &index);

    /**
     * @brief remove replay from the list
     * @param index of the replay
     */
    void delete_replay(int index);

    /**
     * @brief add replay to the list from file path
     * @param path to the replay file
     */
    void addreplay(QString filepath);

    /**
     * @brief add replay to the list from replay pointer
     * @param address of the replay
     */
    void addreplay(WTreplay* replay);

    /**
     * @brief reset the list
     */
	void reset(void);

private:
    QList<WTreplay*> replaylist;  /**< replay list*/
    int replaycount = 0;  /**< number of replay in the list*/
};



enum WTObjectif {CAPTURE, BATTLE};
enum WTGameMode {PLANE_ONLY, PLANE_TANK_MIXED, PLANE_BOAT_MIXED, EVENTS, OTHER};
enum WTDifficulty {UNDEFINED, ARCADE, REALISTIC, SIMULATOR};

const QString WTObjectif_str[2] = {"CAPTURE", "BATTLE"};
const QString WTGameMode_str[5] = {"PLANE_ONLY", "PLANE_TANK_MIXED", "PLANE_BOAT_MIXED", "EVENTS", "OTHER"};
const QString WTDifficulty_str[4] = {"UNDEFINED", "ARCADE", "REALISTIC", "SIMULATOR"};

/**
 * @brief WTMapCount total number of map
 */
const int WTMapCount = 107;

/**
 * @brief WTMapList list of all the game maps name + "unknown" case at index [0]
 */
const QString WTMapList[WTMapCount] = {
    "unknown",
    "air_afghan.bin",
    "air_ladoga.bin",
    "air_race_phiphi_islands.bin",
    "air_vietnam.bin",
    "arcade_africa_canyon.bin",
    "arcade_africa_seashore.bin",
    "arcade_alps.bin",
    "arcade_asia_4roads.bin",
    "arcade_canyon_snow.bin",
    "arcade_ireland.bin",
    "arcade_mediterranean.bin",
    "arcade_norway_fjords.bin",
    "arcade_norway_green.bin",
    "arcade_norway_plain.bin",
    "arcade_phiphi_crater.bin",
    "arcade_phiphi_crater_rocks.bin",
    "arcade_rice_terraces.bin",
    "arcade_snow_rocks.bin",
    "arcade_tabletop_mountain.bin",
    "arcade_zhang_park.bin",
    "avg_abandoned_factory.bin",
    "avg_africa_desert.bin",
    "avg_american_valley.bin",
    "avg_ardennes.bin",
    "avg_berlin.bin",
    "avg_eastern_europe.bin",
    "avg_egypt_sinai.bin",
    "avg_european_fortress.bin",
    "avg_finland.bin",
    "avg_football_field.bin",
    "avg_fulda.bin",
    "avg_guadalcanal.bin",
    "avg_hurtgen.bin",
    "avg_ireland.bin",
    "avg_japan.bin",
    "avg_karelia_forest_a.bin",
    "avg_karpaty_passage.bin",
    "avg_korea_lake.bin",
    "avg_krymsk.bin",
    "avg_kursk.bin",
    "avg_lazzaro_italy.bin",
    "avg_mozdok.bin",
    "avg_normandy.bin",
    "avg_poland.bin",
    "avg_port_novorossiysk.bin",
    "avg_rheinland.bin",
    "avg_sector_montmedy.bin",
    "avg_snow_alps.bin",
    "avg_stalingrad_factory.bin",
    "avg_syria.bin",
    "avg_training_ground.bin",
    "avg_tunisia_desert.bin",
    "avg_vietnam_hills.bin",
    "avg_volokolamsk.bin",
    "avn_africa_gulf.bin",
    "avn_alps_fjord.bin",
    "avn_arabian_north_coast.bin",
    "avn_blacksea_port.bin",
    "avn_england_shore.bin",
    "avn_fiji.bin",
	"avn_fuego_islands.bin",
    "avn_ice_field.bin",
    "avn_ice_land.bin",
    "avn_ice_port.bin",
    "avn_ireland_bay.bin",
    "avn_mediterranean_port.bin",
    "avn_north_sea.bin",
    "avn_norway_islands.bin",
    "avn_phang_nga_bay_islands.bin",
    "berlin.bin",
    "britain.bin",
    "bulge.bin",
    "caribbean_islands.bin",
    "dover_strait.bin",
    "guadalcanal.bin",
    "guam.bin",
    "hangar_airfield_2014.bin",
    "hangar_military_base.bin",
    "hangar_military_base_birthday.bin",
    "hangar_military_base_ny.bin",
    "honolulu.bin",
    "hurtgen.bin",
    "iwo_jima.bin",
    "khalkhin_gol.bin",
    "korea.bin",
    "korsun.bin",
    "krymsk.bin",
    "kursk.bin",
    "malta.bin",
    "midway.bin",
    "moscow.bin",
    "mozdok.bin",
    "norway.bin",
    "peleliu.bin",
    "port_moresby.bin",
    "ruhr.bin",
    "saipan.bin",
    "sector_montmedy.bin",
    "sicily.bin",
    "spain.bin",
    "stalingrad.bin",
    "stalingrad_w.bin",
    "tunisia.bin",
    "wake_island.bin",
    "water.bin",
    "zhengzhou.bin"
};

#endif // WTREPLAY_H
