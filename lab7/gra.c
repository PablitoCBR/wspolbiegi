#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>

#define klucz 5535

int* planszaW;

int pamiec, semafory;
int wynik = -2;


void eraseTable(){
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			*(planszaW + i * 3 + j) = 0;
		}
	}
}

void drawTable(){
	system("clear");
	int i, j;
	printf(" 1 2 3\n");
	for(i = 0; i < 3; i++){
		printf("%c", 'A' + i);
		for(j = 0; j < 3; j++){
			printf("|");
			int val = *(planszaW + i * 3 + j);
			switch(val){
				case -1:
					printf("X");
					break;
				case 1:
					printf("O");
					break;
				default:
					printf(" ");
					break;
			}
		}
		printf("|\n");
	}
}

int isGameEnd(){
	int win[8];
	int i, j, n;
	for(n = 0; n < 8; n++){
		win[n] = 0;
	}
	n = 0;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			int val = *(planszaW + i * 3 + j);
			win[0 + i] += val;
			win[3 + j] += val;
			if(i == j){
				win[6] += val;
			}else if(2-i == j){
				win[7] += val;
			}
			if(val != 0){
				n++;
			}
		}
	}
	for(n = 0; n < 8; n++){
		if(win[n] == 3){
			return 1;
		}else if(win[n] == -3){
			return -1;
		}
	}
	if(n == 9){
		return 0;
	}
	return -2;
}

void koniec(int player, int gameResult){
	if(player != 0){
		if(player == gameResult){
			printf("Wygrales\n");
		}else if(gameResult == 0){
			printf("Remis");
		}else{
			printf("Przegrales\n");
		}
	}
	semctl(semafory, 0, IPC_RMID, 0);
	shmdt(planszaW);
	shmctl(pamiec, IPC_RMID, 0);
	exit(0);
}

void zapisz(int n){
	drawTable();
	printf("Twoj ruch: ");
	char x;
	int y;
	int ok = 0;
	do {
		int ix;
		scanf("%c%d", &x, &y);
		if(x == '\n'){
		}else if(x < 'A' || x > 'C' || y < 1 || y > 3){
			printf("Bledny ruch, podaj poprawny ruch [%d][%d]: ", (int)x, y);
		}else{
			int ix = (int)x - (int)'A';
			y--;
			int val = *(planszaW + ix * 3 + y);
			if(val != -1 && val != 1){
				*(planszaW + ix * 3 + y) = n;
				ok = 1;
			}else{
				printf("Bledny ruch, podaj poprawny ruch: ");
			}
		}
	} while(ok == 0);
	drawTable();
}

void intHandler(int dummy) {
	koniec(0, 0);
}

int main(){
	printf("Witaj");
	signal(SIGINT, intHandler);

	struct sembuf operA1 = {0, -1, 0}, operA2 = {1, 1, 0}, *oper1, *oper2;
	struct sembuf operB1 = {1, -1, 0}, operB2 = {0, 1, 0};

	pamiec = shmget(klucz, 256, 0777 | IPC_CREAT);
	planszaW = shmat(pamiec, 0, 0);

	int player;

	if((semafory = semget(klucz, 2, 0777 | IPC_CREAT | IPC_EXCL)) != -1){
		printf(", grasz jako X, zaczynasz\n");
		player = -1;

		oper1 = &operA1;
		oper2 = &operA2;

		semctl(semafory, 0, SETVAL, 1);
		semctl(semafory, 1, SETVAL, 0);
	}else{
		semafory = semget(klucz, 2, 0777 | IPC_CREAT);
		
		printf(", grasz jako O, zaczekaj na ruch pierwszego gracza\n");
		player = 1;
		
		oper1 = &operB1;
		oper2 = &operB2;
	}

	while(1){
		semop(semafory, oper1, 1);
		wynik = isGameEnd();
		if(wynik != -2){
			drawTable();
			koniec(player, wynik);
			break;
		}
		zapisz(player);
		wynik = isGameEnd();
		if(wynik != -2){
			drawTable();
			koniec(player, wynik);
			break;
		}
		semop(semafory, oper2, 1);
	}

	return 0;
}
