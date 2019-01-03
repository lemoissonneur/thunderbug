#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QApplication>
#include <QPalette>
#include "wtreplay.h"

/**********************
	WTreplay class
**********************/

WTreplay::WTreplay()
{}

WTreplay::WTreplay(QString filepath)
{
    this->File = new QFile(filepath);

    if(!this->File->exists()) this->Name = "*NOT FOUND*";
    else this->Name = this->File->fileName();

    QFileInfo info(File->fileName());
    start = info.birthTime();
    finish = info.lastModified();

    if(File->open(QIODevice::ReadOnly))
        File->close();
    else if(!this->File->isReadable()) this->Name = "*UNREADABLE*" + this->File->fileName();
    else this->Name = "*UNOPENABLE*" + this->File->fileName();

    findmap();
}

WTreplay::~WTreplay()
{
    File->~QFile();
}

bool WTreplay::Analyse()
{
    bool res = false;

    return res;
}

QString WTreplay::description()
{
    // start and finish datetime
    QString desc = start.toString("ddMMM")+" from "+start.toString("HH:mm")+" to "+finish.toString("HH:mm");

    // add map name (.bin choped)
    desc+=" | "+WTMapList[Mapid].chopped(4);

    // add extra from filename if not basic
    QRegExp basicname("#\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}\\.\\d{2}\\.\\d{2}\\.wrpl");
    QString replayfilename = QFileInfo(*File).fileName();
    if(!replayfilename.contains(basicname))
    {
        desc+= "("+replayfilename.remove( QRegExp("\\d{4}\\.\\d{2}\\.\\d{2} \\d{2}\\.\\d{2}\\.\\d{2}") )+")";
    }

    return desc;
}

bool WTreplay::findmap()
{
    Mapid =0;
    uint i =0;

    if(File->open(QIODevice::ReadOnly))
    {
        QTextStream in (File);
        QString line = in.readLine();
        File->close();

        do
        {
            i++;

            if (line.contains(WTMapList[i]))
            {
                Mapid = i;
                //qDebug() << "map found : "+WTMapList[i];
                return true;
            }

        } while (i<WTMapCount);

    }

    qDebug() << "WARNING : map not found in" << File->fileName();

    return false;
}





/**********************
	WTreplaylistModel class
**********************/

WTreplaylistModel::WTreplaylistModel(QObject *parent): QAbstractListModel(parent)
{}

int WTreplaylistModel::rowCount(const QModelIndex & /* parent */) const
{
    return replaycount;
}

QVariant WTreplaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= replaylist.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        return replaylist.at(index.row())->description();
    } else if (role == Qt::BackgroundRole) {
        return qApp->palette().base();
        /* alternate backgroud
        int batch = (index.row() / 100) % 2;
        if (batch == 0)
            return qApp->palette().base();
        else
            return qApp->palette().alternateBase();
            */
    }
    return QVariant();
}

WTreplay* WTreplaylistModel::get_replay(const QModelIndex &index)
{
    return replaylist.at(index.row());
}

WTreplay* WTreplaylistModel::get_replay(int index)
{
    return replaylist.at(index);
}

void WTreplaylistModel::delete_replay(const QModelIndex &index)
{
    replaylist.removeAt(index.row());
    replaycount--;
    emit layoutChanged();
}

void WTreplaylistModel::delete_replay(int index)
{
    replaylist.removeAt(index);
    replaycount--;
    emit layoutChanged();
}

void WTreplaylistModel::addreplay(QString filename)
{
    replaylist.append(new WTreplay(filename));
    replaycount++;
    emit layoutChanged();
}

void WTreplaylistModel::addreplay(WTreplay* replay)
{
    replaylist.append(replay);
    replaycount++;
    emit layoutChanged();
}

void WTreplaylistModel::reset(void)
{
    beginResetModel();
    qDeleteAll(replaylist.begin(), replaylist.end());
    replaylist.clear();
    replaycount = 0;
    endResetModel();
    emit layoutChanged();
}
