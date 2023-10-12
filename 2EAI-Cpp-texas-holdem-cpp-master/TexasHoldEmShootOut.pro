TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += FULL_SOURCE #\
#	FULL_PRINT \
#	DEBUG_PRINT

SOURCES += main.cpp \
	deck.cpp \
	game.cpp \
	hand.cpp \
	poker_rank.cpp \
	table.cpp \
	tournament.cpp

HEADERS += \
	card.h \
	deck.h \
	game.h \
	hand.h \
	player.h \
	playerid.h \
	poker_rank.h \
	table.h \
	tournament.h \
	vader.h \
	noob.h \
	shootout.h

include(2016/2016.pri)
include(2017/2017.pri)
include(2018/2018.pri)
include(2019/2019.pri)
include(2020/2020.pri)
#include(2020/all_files.pri)
