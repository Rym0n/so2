#include <stdio.h>
#include "server.h"
#include "client.h"
int main() {
    printf("Write:\n1. CLIENT \n2.SERVER");
    int info;
    scanf("%d",&info);
    if(info == 1){
        clientMaintain();
    }
    if(info== 2){
        serverMaintain();
    }

    return 0;
}
