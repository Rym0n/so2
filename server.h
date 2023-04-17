//
// Created by root on 11/29/22.
//

#ifndef SO2_SERVER_H
#define SO2_SERVER_H
#include <ncurses.h>
#include <semaphore.h>

typedef struct {
    int playerNumber;
    int pid_t;

    int current_x;
    int current_y;

    int start_x;
    int start_y;
    int death_number;
    int current_coin;
    int bank_coin;
    char movementKeyBlind;

    int round_number;
    int pid_server;
    char player_map [5][5];

    sem_t movementSem;
    sem_t movementSem2;
}PlayerInfo;

typedef struct{
    pid_t pitNum;

    PlayerInfo *player1;
    PlayerInfo *player2;

    int campsiteX;
    int campsiteY;
    int roundNum;

}InfoMap;

struct joinInfo{
    int playerNumber;
    int playerPid;

    sem_t joinCommunication;
    sem_t openRequest;
    sem_t readyRequest;
};

void printMap(WINDOW *map);
void *addPlayer(void* arg);
void serverMaintain();

#endif //SO2_SERVER_H
