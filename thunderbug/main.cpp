#include "mainwindow.h"
#include "wtreplay.h"
#include "thunderbug.h"
#include <QApplication>
#include <QtDebug>

MainWindow* w;
thunderbug* thbg;

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} | %{function} [%{file}(%{line})] | %{message}");

    QApplication a(argc, argv);

    thbg = new thunderbug();
    w = new MainWindow();

    if(!thbg->find_gamedir()) // exit if we cant find the game directory
		return 0;
	
	thbg->get_clog_list();
	thbg->get_llog_list();
	thbg->get_replay_list();

	w->update_dxdiag_info();

	w->show();

	return a.exec();
}
