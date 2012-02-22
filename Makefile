MEDIA = /media/PANDORA/psp/game/CAT_Homebrews\ perso/
TARGET = PSP_Bot

OBJS = main.o level.o controls.o parsing.o language.o\
       disp.o disp_ingame.o disp_ui.o\
       game.o game_ingame.o game_interpreter.o\
  ./libraries/intraFont.o ./libraries/libccc.o\
  ./libraries/glib2d.o
LIBS = -lpng -lz -lpspgum -lpspgu -lpsprtc -lm

CFLAGS = -O3 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP_Bot 1.2
PSP_EBOOT_ICON = icon0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o
