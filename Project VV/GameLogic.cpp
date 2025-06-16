#include "GameLogic.h"

GameLogic::GameLogic()
{
	terminal_open();

    terminal_set("font: fonts/UbuntuMono-Regular.ttf, size=9, codepage=437");
    terminal_set("window.size=191x56");
    terminal_set("window.title='Toxic & Ventures'");
    terminal_set("window.icon='icons/icon.ico'");
    terminal_set("input.encoding = utf-8");
    terminal_set("window.encoding=437");
    terminal_set("window.resizeable=false");
    SetFontSize(false);
    //terminal_set("window.cursor.visible=false");
    terminal_set("window.redraw_on_focus=true");

    DrawFrameFromFile("ramka.txt", 0, 0);
    LoadScreen();

    currentEntity = new Enemy("mimik");

    Player* Gplayer = new Player(1);
    Gplayer->setName(L"Клоун");
    Gplayer->setMaxEmotion(6);
    Gplayer->setModD20(1);
    int emot[6] = { 10,2,2,2,2,2 };
    Gplayer->setEmotions(emot, emot);

    //Тут ещё будет логика меню и настроек

	while (currentEntity) {
        nextEntity = currentEntity->start(Gplayer);
        delete currentEntity;
        currentEntity = nextEntity;
	}
}
