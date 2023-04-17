#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include<fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include "client.h"
#include "server.h"


void printInfoPlayer(WINDOW *playerinfo, PlayerInfo playerInfoModel);
void clientMaintain() {

    int playerNumber = 0;

    sem_t *openServer = sem_open("openServer", O_EXCL | O_RDWR, 0600,
                                 0); //semafor do dania znac ze server jest gotowy (semafor nazwany )
    if (openServer == NULL) {
        printf("Incorrect open server exception");
        exit(1);
    }

    int sem_value;
    sem_getvalue(openServer, &sem_value);//liczba wolnych slotow

    if (sem_value > 0) {
        int fd = shm_open("joinPlayerSHM", O_RDWR | O_CREAT, 0600);
        if (fd == -1) {
            printf("Incorrect joinPlayerSHM Client");
        }
        ftruncate(fd, sizeof(struct joinInfo)); //zeby plik byl tego samego rozmiaru co struktura
        struct joinInfo *PlayerJoin = mmap(NULL, sizeof(struct joinInfo), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

        sem_wait(&PlayerJoin->openRequest);

        //wypelnienie wartosci z petli server o playerze
        if (sem_value == 2)
            PlayerJoin->playerNumber = playerNumber = 1;
        else if (sem_value == 1)
            PlayerJoin->playerNumber = playerNumber = 2;

        PlayerJoin->playerPid = getpid();

        sem_post(&PlayerJoin->joinCommunication);
        sem_wait(&PlayerJoin->readyRequest);

//        if (PlayerJoin.accepted)
        sem_wait(openServer);//odejmujemy semafor o jeden aby liczba graczy sie zgadzala

        munmap(PlayerJoin, sizeof(struct joinInfo));//odmapowanie struktury
        close(fd);

    } else {
        printf("No slots available");
        exit(1);
    }

    int entity_fd = -1;
    if (playerNumber == 1) {
        entity_fd = shm_open("Player1_Connection", O_CREAT | O_RDWR, 0600);
    }
    else if (playerNumber == 2) {
        entity_fd = shm_open("Player2_Connection", O_CREAT | O_RDWR, 0600);
    }

    ftruncate(entity_fd, sizeof(PlayerInfo)); //zeby plik byl tego samego rozmiaru co struktura
    PlayerInfo *playerInfo = mmap(NULL, sizeof(PlayerInfo), PROT_WRITE | PROT_READ, MAP_SHARED, entity_fd, 0);

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    
    WINDOW *playerModel = newwin(100, 100, 0, 0);

    pthread_t threadMovePlayer;
    pthread_create(&threadMovePlayer, NULL, playerMovement, playerInfo);

    while (1) {//PD zrobic mape

        printInfoPlayer(playerModel, *playerInfo);
        wrefresh(playerModel);

        // wyslac klawisz
    }

//    munmap(PlayerJoin, sizeof(struct joinInfo));//odmapowanie struktury
//    close(fd);
}

void printInfoPlayer(WINDOW *playerModel, PlayerInfo playerInfoModel) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            mvwprintw(playerModel, y, x, "%c",playerInfoModel.player_map[y][x]);
        }
    }
    mvwprintw(playerModel, 1, 28, "Server's PID: %d", playerInfoModel.pid_server);
    mvwprintw(playerModel, 2, 28, "Campsite X/Y: unknown");
    mvwprintw(playerModel, 3, 28, "Round number: %d", playerInfoModel.round_number); //nw co wpisac

    mvwprintw(playerModel, 5, 28, "Player:   ");
    mvwprintw(playerModel, 6, 29, "Number:     %d",playerInfoModel.playerNumber);
    mvwprintw(playerModel, 7, 29, "Type:       HUMAN");
    mvwprintw(playerModel, 8, 29, "Curr X/Y:   %d/%d",playerInfoModel.current_x,playerInfoModel.current_y);
    mvwprintw(playerModel, 9, 29, "Deaths:     %d",playerInfoModel.death_number);

    mvwprintw(playerModel, 11, 29,"Coins found:   %d",playerInfoModel.current_coin);
    mvwprintw(playerModel, 12, 29,"Coins brought: %d",playerInfoModel.bank_coin);

    mvwprintw(playerModel, 16, 28, "Legend: ");
    mvwprintw(playerModel, 17, 29, "1234 - players");
    mvwprintw(playerModel, 18, 29, "X    - wall");
    mvwprintw(playerModel, 19, 29, "#    - bushes (slow down)");
    mvwprintw(playerModel, 20, 29, "*    - wild beast");
    mvwprintw(playerModel, 21, 29, "c    - one coin");
    mvwprintw(playerModel, 22, 29, "t    - treasure (10 coins)");
    mvwprintw(playerModel, 22, 29, "T    - large treasure (50 coins)");
    mvwprintw(playerModel, 22, 29, "A    - campsite");
    mvwprintw(playerModel, 22, 29, "D    - dropped treasure");
}

void *playerMovement(void *arg){
    PlayerInfo *player = (PlayerInfo *) arg;
    while(1){
        sem_wait(&player->movementSem2);
        char temp = getchar();

        WINDOW *debug = newwin(10, 10, 15, 15);
        mvwprintw(debug, 0, 0, "Button pressed: %c", temp);
        wrefresh(debug);

        player->movementKeyBlind = temp;
        sem_post(&player->movementSem);
    }
}


