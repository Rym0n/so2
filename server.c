#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include<fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include "server.h"
#include <time.h>

char MapModel[25][52] = {
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "X   X       X    ####       X         X       X   X",
        "X X XXX XXX XXXXXXXXXXX XXX X XXXXXXX XXX XXXXX   X",
        "X X   X X X           X X X   X     X     X   X   X",
        "X XXX X X XXX###XXXXX X X XXXXX XXXXXXXXXXXXX XXX X",
        "X X X   X           X X X ##  X       X       X X X",
        "X X XXXXX XXX XXXXXXX X X X XXX XXX XXX XXX X X X X",
        "X X         X X       X X X     X   X   X X X   X X",
        "X X XXXXXXX XXX XXXXXXX XXXXX XXX XXX XXX X XXX X X",
        "X X X     X   X   X     X   X   X         X X X X X",
        "X XXX XXX XXX XXX XXX XXX X XXX XXXXXXXXXXX X X X X",
        "X X   X       X X   X  A  X X   X X       X X   X X",
        "X X XXXXXX#XX X XXX XXX XXX XXX X X XXXXX X X XXX X",
        "X X    #X   X X   X   X   X   X   X X     X X X   X",
        "X X X ##X XXX XXX XXX XXXXXXX XXX X XXX XXX X X XXX",
        "X X X## X    #  X   X X  ###  X   X   X     X X X X",
        "X X X#  XXXXXXX X X X X XX#XXXX XXXXX XXXXXXX X X X",
        "X X X#      X   X X X   X     X   X X       ##X   X",
        "X XXXXXXXXX X XXX XXXXXXX XXXXXXX X XXXXX X ##XXX X",
        "X X#      X X     X     X       X   X   X X ##  X X",
        "X X XXXXX X XXXXXXX X XXX XXXXX XXX X X XXX#XXXXX X",
        "X###X     X         X  ###X## X     X X   X###### X",
        "X XXX XXXXXXXXXXXXXXXXXXXXX#X XXXXXXX XXX X#    # X",
        "X   X                 ######X##         X    ##   X",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
char playerMapModel[25][52] = {0};
WINDOW *debug;

void printMap(WINDOW *map) {         // Biore map i przechodze
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 52; x++) {
            mvwprintw(map, y, x, "%c", MapModel[y][x]);
        }
    }
}

void printPlayers(WINDOW *map) {         // Biore map i przechodze
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 52; x++) {
            if (playerMapModel[y][x] != 0)
                mvwprintw(map, y, x, "%c", playerMapModel[y][x]);
        }
    }
}

void printPlayerInfo(PlayerInfo info) {


    //AD 1
    // ZROBIC PRIONTOWANIE I PLAYER 2 przesuniecie o 2

}

void printInfo(WINDOW *playerinfo, InfoMap server) {

    mvwprintw(playerinfo, 1, 28, "Server's PID: %d", server.pitNum);
    mvwprintw(playerinfo, 2, 28, "Campsite X/Y: %d/%d", server.campsiteX, server.campsiteY);
    mvwprintw(playerinfo, 3, 28, "Round number: %d", server.roundNum);

    mvwprintw(playerinfo, 5, 28, "Parameter:   ");
    mvwprintw(playerinfo, 6, 28, "PID:          ");
    mvwprintw(playerinfo, 7, 28, "Type:         ");
    mvwprintw(playerinfo, 8, 28, "Curr X/Y:     ");
    mvwprintw(playerinfo, 9, 28, "Deaths:       ");

    mvwprintw(playerinfo, 11, 28, "Coins ");
    mvwprintw(playerinfo, 12, 32, "carried ");
    mvwprintw(playerinfo, 13, 32, "brought ");

    mvwprintw(playerinfo, 16, 28, "Legend: ");
    mvwprintw(playerinfo, 17, 29, "1234 - players");
    mvwprintw(playerinfo, 18, 29, "X    - wall");
    mvwprintw(playerinfo, 19, 29, "#    - bushes (slow down)");
    mvwprintw(playerinfo, 20, 29, "*    - wild beast");
    mvwprintw(playerinfo, 21, 29, "c    - one coin");
    mvwprintw(playerinfo, 22, 29, "t    - treasure (10 coins)");
    mvwprintw(playerinfo, 22, 29, "T    - large treasure (50 coins)");
    mvwprintw(playerinfo, 22, 29, "A    - campsite");
    mvwprintw(playerinfo, 22, 29, "D    - dropped treasure");

    int x = strlen("Parameter:   ");
    if (server.player1 != NULL) {
        mvwprintw(playerinfo, 5, 28 + x, "Player%d", 1);
        mvwprintw(playerinfo, 6, 28 + x, "%d", server.player1->pid_t);
        mvwprintw(playerinfo, 7, 28 + x, "HUMAN");
        mvwprintw(playerinfo, 8, 28 + x, "%d/%d", server.player1->current_x, server.player1->current_y);
        mvwprintw(playerinfo, 9, 28 + x, "%d", server.player1->death_number);
        mvwprintw(playerinfo, 12, 28 + x, "%d", server.player1->current_coin);
        mvwprintw(playerinfo, 13, 28 + x, "%d", server.player1->bank_coin);
    }
    int y = strlen("Player1  ");
    if (server.player2 != NULL) {
        mvwprintw(playerinfo, 5, 28 + x + y, "Player%d", 2);
        mvwprintw(playerinfo, 6, 28 + x + y, "%d", server.player2->pid_t);
        mvwprintw(playerinfo, 7, 28 + x + y, "HUMAN");
        mvwprintw(playerinfo, 8, 28 + x + y, "%d/%d", server.player2->current_x, server.player2->current_y);
        mvwprintw(playerinfo, 9, 28 + x + y, "%d", server.player2->death_number);
        mvwprintw(playerinfo, 12, 28 + x + y, "%d", server.player2->current_coin);
        mvwprintw(playerinfo, 13, 28 + x + y, "%d", server.player2->bank_coin);
    }
}

void addRound(InfoMap *server) {
    server->roundNum += 1;
    if (server->player1 != NULL) {
        server->player1->round_number = server->roundNum;
    }
    if (server->player2 != NULL) {
        server->player2->round_number = server->roundNum;
    }
}

int map_random_location_player(char map[25][52], int *x, int *y) {
    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);
    int random_x, random_y;
    while (1) {
        random_x = rand() % ((52 - 1) + 1);
        random_y = rand() % ((25 - 1) + 1);

        if (map[random_y][random_x] == ' ')
            break;
    }
    *x = random_x;
    *y = random_y;
//    *x = 14;
//    *y = 5;
    return 0;
}

int map_copy_to_player(char map[25][52], int x, int y, char dest[5][5]) {
    for (int i = 0; i < 5; ++i) {
        int src_y = y + i;

        for (int j = 0; j < 5; ++j) {
            int src_x = x + j;

            if (src_y < 0 || src_x < 0 || src_y >= 25 || src_x >= 52) {
                dest[i][j] = ' ';
                continue;
            }

            char temp = map[src_y][src_x];

            if (temp != '\0')
                dest[i][j] = temp;
        }
    }
}

void addMapPlayer(InfoMap *server) {
    if (server->player1 != NULL) {
        int x = (int) server->player1->current_x;
        int y = (int) server->player1->current_y;
        map_copy_to_player(MapModel, x - 2, y - 2, server->player1->player_map);
        map_copy_to_player(playerMapModel, x - 2, y - 2, server->player1->player_map);

    }
    if (server->player2 != NULL) {
        int x = (int) server->player2->current_x - (5 / 2);
        int y = (int) server->player2->current_y - (5 / 2);
        map_copy_to_player(MapModel, x, y, server->player2->player_map);
        map_copy_to_player(playerMapModel, x, y, server->player2->player_map);
    }
}

void updatePositionPlayer(InfoMap *server) {
    if (server->player1 != NULL) {
        playerMapModel[server->player1->current_y][server->player1->current_x] = '1';
    }
    if (server->player2 != NULL) {
        playerMapModel[server->player2->current_y][server->player2->current_x] = '2';
    }
}

void newPositionPlayer(PlayerInfo *info) {
    playerMapModel[info->current_y][info->current_x] = (char) (info->playerNumber + '0');
}


void clearPositionPlayer(InfoMap *server) {
    if (server->player1 != NULL) {
        playerMapModel[server->player1->current_y][server->player1->current_x] = 0;
    }
    if (server->player2 != NULL) {
        playerMapModel[server->player2->current_y][server->player2->current_x] = 0;
    }
}

void deletePositionPlayer(PlayerInfo *info) {
    playerMapModel[info->current_y][info->current_x] = 0;
}

void *maintainPlayer(void *arg) {//potrzymanie polaczenia z graczem
    PlayerInfo *player = (PlayerInfo *) arg;

    int pd = 0;
    if (player->playerNumber == 1) {
        pd = shm_open("Player1_Connection", O_CREAT | O_RDWR, 0600);
    } else if (player->playerNumber == 2) {
        pd = shm_open("Player2_Connection", O_CREAT | O_RDWR, 0600);
    }

    ftruncate(pd, sizeof(PlayerInfo));
    PlayerInfo *map = mmap(NULL, sizeof(PlayerInfo), PROT_WRITE | PROT_READ, MAP_SHARED, pd, 0);


}

void *playerMovementManager(void *arg) {
    PlayerInfo *player = (PlayerInfo *) arg;
    while (1) {
        sem_post(&player->movementSem2);
        sem_wait(&player->movementSem);
        int playerX = player->current_x;
        int playerY = player->current_y;

        if (player->movementKeyBlind == 'w') {
            playerY += 1;
        }
        if (player->movementKeyBlind == 'a') {
            playerX -= 1;
        }
        if (player->movementKeyBlind == 's') {
            playerY -= 1;
        }
        if (player->movementKeyBlind == 'd') {

            playerX += 1;
        }
        if (MapModel[playerY][playerX] == 'X') {
            continue;
        } else {
            deletePositionPlayer(player);
            player->current_y = playerY;
            player->current_x = playerX;
            newPositionPlayer(player);
        }
    }
}


void *addPlayer(void *arg) { //u clienta to samo zeby sherowac memory
    int fd = shm_open("joinPlayerSHM", O_RDWR | O_CREAT, 0600);
    InfoMap *server = (InfoMap *) arg;

    if (fd == -1) {
        mvwprintw(debug, 0, 0, "%s", "shm exception");
        wrefresh(debug);
    }

    ftruncate(fd, sizeof(struct joinInfo));
    struct joinInfo *PlayerJoin = mmap(NULL, sizeof(struct joinInfo), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    sem_init(&PlayerJoin->joinCommunication, 1, 0); //semafor nienazwany, 0 to oznacza ile osoob moze zrobic sem w8
    sem_init(&PlayerJoin->openRequest, 1, 0);
    sem_init(&PlayerJoin->readyRequest, 1, 0);

    while (1) {
        sem_post(&PlayerJoin->openRequest); //info o wolnym slocie
        sem_wait(&PlayerJoin->joinCommunication); //czekamy na gracza i czekanie na dane player join az wypelni

        int playerNumber = PlayerJoin->playerNumber;

        int pd = 0;
        if (playerNumber == 1) {
            pd = shm_open("Player1_Connection", O_CREAT | O_RDWR, 0600);
            ftruncate(pd, sizeof(PlayerInfo));
            server->player1 = mmap(NULL, sizeof(PlayerInfo), PROT_WRITE | PROT_READ, MAP_SHARED, pd,
                                   0); //tworzymy dla niego shared memory

            server->player1->playerNumber = 1;
            server->player1->pid_t = PlayerJoin->playerPid;
            int locationX, locationY;
            map_random_location_player(MapModel, &locationX, &locationY);
            server->player1->start_x = locationX;
            server->player1->start_y = locationY;
            server->player1->current_x = locationX;
            server->player1->current_y = locationY;

            pthread_t threadMovePlayer1;
            pthread_create(&threadMovePlayer1, NULL, playerMovementManager, &server->player1);

        } else if (playerNumber == 2) {
            pd = shm_open("Player2_Connection", O_CREAT | O_RDWR, 0600);
            ftruncate(pd, sizeof(PlayerInfo));
            server->player2 = mmap(NULL, sizeof(PlayerInfo), PROT_WRITE | PROT_READ, MAP_SHARED, pd, 0);

            server->player2->playerNumber = 2;
            server->player2->pid_t = PlayerJoin->playerPid;

            int locationX, locationY;
            map_random_location_player(MapModel, &locationX, &locationY);
            server->player2->start_x = locationX;
            server->player2->start_y = locationY;
            server->player2->current_x = locationX;
            server->player2->current_y = locationY;

            pthread_t threadMovePlayer2;
            pthread_create(&threadMovePlayer2, NULL, playerMovementManager, &server->player2);
        }

        updatePositionPlayer(server);
        addMapPlayer(server);

        sem_post(&PlayerJoin->readyRequest);//dajemy znac clientowi
    }


    sem_wait(&PlayerJoin->joinCommunication);

    mvwprintw(debug, 0, 0, "%d", PlayerJoin->playerNumber);
    printf("%d", PlayerJoin->playerNumber);

    wrefresh(debug);
}

void *keyThread(void *arg) {
    while (1) {
        int x, y;
        char temp = getchar();
        if (temp == 'c') {
            map_random_location_player(MapModel, &x, &y);
            MapModel[y][x] = 'c';
        }
        if (temp == 't') {
            map_random_location_player(MapModel, &x, &y);
            MapModel[y][x] = 't';
        }
        if (temp == 'T') {
            map_random_location_player(MapModel, &x, &y);
            MapModel[y][x] = 'T';
        }

    }

}


void serverMaintain() {

    InfoMap server = {0};
    server.campsiteY = 11;
    server.campsiteX = 23;
    server.player1 = NULL;
    server.player2 = NULL;

    sem_t *openServer = sem_open("openServer", O_CREAT | O_RDWR, 0600,
                                 2); //semafor do dania znac ze server jest gotowy (semafor nazwany )

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    WINDOW *map = newwin(100, 100, 0, 0);
    WINDOW *playerInfo = newwin(100, 100, 0, 28);
    //debug = newwin(50,50,0,0);

//    pthread_attr_t threadAtributeJoin; //atrybut do watku
//    pthread_attr_init(&threadAtributeJoin);
    pthread_t threadJoin;
    pthread_create(&threadJoin, NULL, addPlayer, &server);

    pthread_t threadKey;
    pthread_create(&threadKey, NULL, keyThread, NULL);

    //Licznik Rund
    clock_t clock1 = clock();

    while (1) {
        double time = (((double) clock()) / CLOCKS_PER_SEC) - (((double) clock1) / CLOCKS_PER_SEC);
        if (time > 1.0) {
            printMap(map);
            printPlayers(map);
            wrefresh(map);
            printInfo(playerInfo, server);
            wrefresh(playerInfo);
            addRound(&server);
            addMapPlayer(&server);
            clock1 = clock();
        }

        //jak chce wyczyscic to uzywam werase
    }

}//zawsze usuwaj root/dev/shm

