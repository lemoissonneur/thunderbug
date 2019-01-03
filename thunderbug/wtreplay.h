#ifndef WTREPLAY_H
#define WTREPLAY_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QAbstractListModel>

class WTreplay
{
public:
	/* WTreplay()
		desc:	create empty replay
		in	:	-
		out	:	-
	*/
    WTreplay();
	
	/* WTreplay(QString filepath)
		desc:	create replay and extract info from the given file ( bool Analyse() is called )
		in	:	QString filepath	:	path of the file
		out	:	-
	*/
    WTreplay(QString filepath);
	
	/* ~WTreplay()
		desc:	destroy replay
		in	:	-
		out	:	-
	*/
    ~WTreplay();

	/*	bool Analyse()
		desc:	extract map name, game mode, difficulty, etc ...  from the file
		in	:	-
		out	:	false if failed to open the file or file is not set
	*/
    bool Analyse();
	
	/*	QString description()
		desc:	return a summary of the replay info we extracted from the file
		in	:	-
		out	:	QString		: empty if file is not set or Analyse() failed
	*/
    QString description();

	//
	QString Name;		// replay name (file name + extra info)
    QFile* File;		// replay file
    
    QDateTime start;	// start time of the game (file creation time)
    QDateTime finish;	// finish time of the game (file last edit time)

    uint Mapid;			// index in the WTMapList array

    QString ServerReplayLink;	//
	
private:

	/*	bool findmap()
		desc:	extract map name from the replay file
		in	:	-
		out	:	bool : false if map not found
	*/
	bool findmap();

};










class WTreplaylistModel :public QAbstractListModel
{

public:
    WTreplaylistModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    WTreplay* get_replay(const QModelIndex &index);
    WTreplay* get_replay(int index);
    void delete_replay(const QModelIndex &index);
    void delete_replay(int index);
    void addreplay(QString filename);
    void addreplay(WTreplay* replay);
	void reset(void);

private:
    QList<WTreplay*> replaylist;
    int replaycount = 0;
};









/*
	
*/
enum WTObjectif {CAP, BATTLE};
enum WTGameMode {PLANE_ONLY, TANK_ONLY, BOATS_ONLY, PLANE_TANK_MIXED, PLANE_BOAT_MIXED};
enum WTDifficulty {UNDEFINED, ARCADE, REALISTIC, SIMULATOR};

const QString WTObjectif_str[2] = {"CAPTURE", "BATTLE"};
const QString WTGameMode_str[5] = {"PLANE_ONLY", "PLANE_TANK_MIXED", "PLANE_BOAT_MIXED", "EVENTS", "OTHER"};
const QString WTDifficulty_str[3] = {"ARCADE", "REALISTIC", "SIMULATOR"};

const int WTMapCount = 106;
const QString WTMapList[WTMapCount] = {	// list of all the game maps name + "unknown" case at index [0]
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
