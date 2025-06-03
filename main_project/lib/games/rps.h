#ifndef RPS_H
#define RPS_H

#include "games.h"

#define RPS_OPTIONS 3

// determines winner of rock-paper-scissors
// first value represents player 1 (line),
// second value represents player 2 (column)
static const uint8_t rps_winner[RPS_OPTIONS][RPS_OPTIONS] PROGMEM = { 
    { 0, 2, 1 },
    { 1, 0, 2 },
    { 2, 1, 0 }
};

// index beats vector value
static const uint8_t beats[RPS_OPTIONS] PROGMEM = { 2, 0, 1 };

const char *choice_name(uint8_t choice); 
uint8_t bot_turn(char diff, uint8_t last_bot_choice, uint8_t last_player_choice);
void rps_pvbot(char diff);
void rps_pvp();
void rps(char mode);

#endif // RPS_H