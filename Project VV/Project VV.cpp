#include "BearLibTerminal.h"
#include "Entitys.h"

int main() {
    Player player(1);
    player.save();
    player.PrintParam();
    OpenTerminal();
    return 0;
}


