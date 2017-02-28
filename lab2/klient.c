#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#define WIADOMOSC 512

//Nawiazywanie polaczenia z serwerem
void polacz()
{	
	while (open("lockfile",O_CREAT|O_EXCL, 0711)==-1)
    {
       printf("Serwer zajety, prosze czekac\n\n");	
       sleep(3);
    }
}

//Procedura wysylania tresci wiadomosci do serwera (bufor dane.txt)
void wyslij()
{
	int dane;
	char bufor[WIADOMOSC] ={0x00,};
	char znak[1] = {0x00,};
	int counter = 0;
	
	char uzytkownik[100] = {0x00,};
	getlogin_r(uzytkownik, 100);
	char output[1024] = {0x00,};
	
	while((dane = open("dane.txt", O_RDWR|O_CREAT|O_APPEND, 0711)) < 0)
	{
		
	}
	
	if(dane != 0)
	{
		printf("\n%s:\n", uzytkownik);
		while(1)
		{
			read(0,znak,1);
			if(znak[0] == 27)
            {
				counter = sprintf(output, "%s:\n%s" , uzytkownik, bufor);
				write(dane, output, counter);
				break;
			}
				bufor[counter] = znak[0];
				counter++;
				
		}
	}
}

//Odbieranie wiadomosci zwrotnej od serwera (bufor wyniki.txt)
void odbierz()
{
	int wyniki;
	char bufor[WIADOMOSC] ={0x00,};

	while((wyniki = open("wyniki.txt", O_RDWR)) < 0)
	{
	}
		
	if(wyniki != 0)
	{
		while(read(wyniki,bufor,512) < 1)
		{
		}
		printf("WIADOMOSC Z SERWERA:\n%s", bufor);
		close(wyniki);
	}
}

//Oczyszczanie buforow wyniki i dane.txt (faktyczne kasowanie plikow tekstowych)
void rozlacz()
{
	unlink("wyniki.txt");
	
}

//Glowna funkcja programu - pobieranie nazwy uzytkownika i wywolywanie procedur wysylki i odbioru wiadomosci wraz z laczenie i rozlaczaniem z serwerem  
int main(int argc, char *argv[]) 
{
	char uzytkownik[100] = {0x00,};
	getlogin_r(uzytkownik, 100);
	
	/*
	char sciezka_programu[200];
	getcwd(sciezka_programu, 200);
	printf("pwd -> %s\n", sciezka_programu);	
	*/
		
	printf("Witaj w komunikatorze tekstowym - KLIENT\n");
	printf("(Zalogowany jako:%s)\n" , uzytkownik);

		polacz();
		
		wyslij();
		
		odbierz();

		rozlacz();

	getchar();
    return 0;
}
		
