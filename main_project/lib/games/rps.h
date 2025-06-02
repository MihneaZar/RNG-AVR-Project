#ifndef RPS_H
#define RPS_H

#include "games.h"

#define RPS_OPTIONS 3

uint8_t bot_turn(char diff, uint8_t last_bot_choice, uint8_t last_player_choice);
void rps_pvbot(char diff);
void rps_pvp();
void rps(char mode);

#endif // RPS_H