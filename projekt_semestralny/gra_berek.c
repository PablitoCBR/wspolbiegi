#include <X11/Xlib.h>
#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>


const int SEM_ID = 7256;

//XLIB configuration
Display *mydisplay;
Window mywindow;
XSetWindowAttributes mywindowattributes;
XGCValues mygcvalues;
GC mygc;
Visual *myvisual;
int mydepth;
int myscreen;
Colormap mycolormap;
XColor mycolor, mycolor0, mycolor1, mycolor2, mycolor3, mycolor4, mycolor5, dummy;
XEvent myevent;
char *myname;

int pamiec;
char *adres;

typedef enum { false, true } bool;


//Struktura pionka gracza (wspolrzedne x,y, rozmiar i ID gracza oraz status (aktywny gracz , berek, uciekajacy)
typedef struct circle {
	int x;
	int y;
	int size;
	int ID;
	bool active_player;
	bool berek;
}circle;


//Numer biezacego gracza, tym ktorym sterujemy
int id;

//Funkcja przydzielajaca ID graczom. ID==1 wtedy zostaje berkiem (zawsze pierwszy gracz) 
void firts_player_status(circle *circles)
{
	int i; 
	
	for(i = 1; i < 6; i++)
	{
		circles[i].ID = i;
		circles[i].active_player = false;
		circles[i].berek = false;
		
		if(circles[i].ID == 1) 
		{
			circles[i].berek = true;
		}
	}
}

//Funkcja dodajaca nowego gracza do gry (zmiana statusu active_player)
int new_player(circle *circles) {
	
	int i;
	
	for(i=1; i<6; i++) 
	{
		if(circles[i].active_player == false) 
		{
			circles[i].active_player = true;
			break;
		}
	}
	return i;
}

//Funkcja wyswietlajaca graczy (kolorowanie graczy)
void overview_game(circle *circles, int id) {
	
	int i;
	char napis[1];  //Numer przy graczu (jego ID)
	
	if(id==6) {
		printf("Nie mozesz dolaczyc do gry! Max liczba graczy to: %d\n", id-1);
		}
		
	else {
	
	for(i=1; i<6; i++) 
	{
		if(circles[i].active_player == true) 
		{
			//Wyswietlanie nr gracza (id) oraz wspolrzednych x , y w terminalu przy kazdym ruchu
			printf("GRACZ: %d\n", id);
			printf("Twoje aktualne polozenie: x=%d y=%d\n", circles[i].x, circles[i].y);    
			
			if(circles[i].berek == true) //Jesli berek to czerwony
			{ 
				XSetForeground(mydisplay,mygc,mycolor1.pixel);
				sprintf(napis, "%d", circles[i].ID);
				XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis));	 
			}
			
			//Inni gracze (pozostale kolory)
				
				if(circles[i].ID == 1 && circles[i].berek == false) {
					XSetForeground(mydisplay,mygc,mycolor0.pixel);
					sprintf(napis, "%d", circles[i].ID);
				    XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis)); 
					
				}
				if(circles[i].ID == 2 && circles[i].berek == false) {
					XSetForeground(mydisplay,mygc,mycolor2.pixel);
					sprintf(napis, "%d", circles[i].ID);
				    XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis));
					
				}
				if(circles[i].ID == 3 && circles[i].berek == false) {
					XSetForeground(mydisplay,mygc,mycolor3.pixel);
					sprintf(napis, "%d", circles[i].ID);
				    XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis));
					
				}
				if(circles[i].ID == 4 && circles[i].berek == false) {
					XSetForeground(mydisplay,mygc,mycolor4.pixel);
					sprintf(napis, "%d", circles[i].ID);
				    XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis));
					
				}
				if(circles[i].ID == 5 && circles[i].berek == false) {
					XSetForeground(mydisplay,mygc,mycolor5.pixel);
					sprintf(napis, "%d", circles[i].ID);
				    XDrawString(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, napis, strlen(napis));
					
				}
			
				XFillArc(mydisplay, mywindow, mygc, circles[i].x, circles[i].y, circles[i].size, circles[i].size, 0, 360*64);
				XFlush(mydisplay);
			
			}
		}
	}
}
//Ilosc "aktywnych" graczy
int number_of_players(circle *circles) {
	
	int i, number_players=0; 
	
	for(i=1; i<6; i++) 
	{
		if(circles[i].active_player == true) 
		{
			number_players++;
		}
	} 
	return number_players;
}

//Funkcja spradzajaca czy berek zlapal innego gracza
void check_to_catch(circle *circles, int id) {
	
	int i;
	
	for(i=1; i<6; i++) 
	{
	   if(circles[id].active_player == true && i != id && number_of_players(circles)>1 && circles[id].berek == true) //Jesli graczy jest wiecej niz 1 i nasz gracz to berek (tylko w takim wypadku nastapic ma zmiana)
	   {
			if(circles[id].x >= circles[i].x-circles[i].size && circles[id].x <= circles[i].x+circles[i].size && circles[id].y >= circles[i].y-circles[i].size && circles[id].y <= circles[i].y+circles[i].size) 
			{
				usleep(7000);
				
				circles[i].berek = true;
				circles[id].berek = false;
			}
	   }
	}
}

//Funkcja obslugujaca rozgrywke, sterowanie, wyjscie z programu
int game(circle *circles, int id) {
   
   int xw, yw;
   while (1)
   {
	   XSetForeground(mydisplay, mygc, mycolor.pixel);
	   XFillRectangle(mydisplay, mywindow, mygc, 0, 0, 600, 600);
	   overview_game(circles, id);
	   usleep(5000);
	   if(XPending(mydisplay) > 0) 
		{
		  XNextEvent(mydisplay,&myevent);
		  switch (myevent.type)
		  {
			 case Expose:
				  
				  //PUNKT STARTOWY
				  circles[id].x = 250;
				  circles[id].y = 250;
				  circles[id].size = 60;
				  
				  break;
			  
			   
			 case ButtonPress:
				
				xw=myevent.xbutton.x;
              
				yw=myevent.xbutton.y;
              
				check_to_catch(circles, id);
				XSetForeground(mydisplay,mygc,mycolor.pixel);
				XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
  
				if(myevent.xbutton.x == circles[id].x+circles[id].size && myevent.xbutton.y == circles[id].y+circles[id].size) { 
					circles[id].x = xw;
					circles[id].y = yw;
				}
             
				XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
				XFlush(mydisplay);
            
				break;

			  case MotionNotify:
         
				xw=myevent.xmotion.x;
              
				yw=myevent.xmotion.y;
                
				//BLOKADA RUCHU POZA PLANSZA PRZY STEROWANIU MYSZA - NIEDOPRACOWANE
				//if(circles[id].y >= 5 && circles[id].x <= 535 && circles[id].x >= 5 && circles[id].y <= 535) {
					
					circles[id].x=xw;
					circles[id].y=yw;
				//}
			  
				/*else {
				  
					if(circles[id].x >= 5) {
						circles[id].x += 1;
					}
					if(circles[id].x <= 535) {
						circles[id].x -= 1;
					}
					if(circles[id].y <= 535) {
						circles[id].y -= 1;
					}
					if(circles[id].y >= 5) {
						circles[id].y += 1;
					}	
				}*/
             
				check_to_catch(circles, id);
				XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
				XFlush(mydisplay);

				break;
			  
			  
				//OBSLUGA KLAWISZY
			    case KeyPress:
				
				//WYJSCIE Z PROGRAMU (KLAWISZ ESC)
				if(myevent.xkey.keycode == 0x09) 
				{
					  XCloseDisplay(mydisplay);
					  shmdt(adres);
					  shmctl(pamiec, IPC_RMID, NULL);
					  exit(0);
				}
				
				//STEROWANIE ZA POMOCA KLAWIATURY
				  
				  //STRZALKA W GORE
				  if(myevent.xkey.keycode == 0x6f) {
					  check_to_catch(circles, id);
					  XSetForeground(mydisplay,mygc,mycolor.pixel);
					  XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					  
					if(circles[id].y >= 5) circles[id].y -= 10;
					XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					XFlush(mydisplay);
				  }
				  
				  //STRZALKA W PRAWO
				  if(myevent.xkey.keycode == 0x72) {
					  check_to_catch(circles, id);
					  XSetForeground(mydisplay,mygc,mycolor.pixel);
					  XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					  
					if(circles[id].x <= 535) circles[id].x += 10;
					XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					XFlush(mydisplay);
				  }
				  
				  //STRZALKA W LEWO
				  if(myevent.xkey.keycode == 0x71) {
					  check_to_catch(circles, id);
					  XSetForeground(mydisplay,mygc,mycolor.pixel);
					  XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
						
					if(circles[id].x >= 5) circles[id].x -= 10;
					XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					XFlush(mydisplay);
				  }
				  
				  //STRZALKA W DOL
				  if(myevent.xkey.keycode == 0x74) {
					  check_to_catch(circles, id);
					  XSetForeground(mydisplay,mygc,mycolor.pixel);
					  XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					  
					if(circles[id].y <= 535) circles[id].y += 10;
					XFillArc(mydisplay, mywindow, mygc, circles[id].x, circles[id].y, circles[id].size, circles[id].size, 0, 360*64);
					XFlush(mydisplay);
				  }
				break;
		}
	}
  }
}

//Glowna funkcja main
int main(int argc, char **argv) {

	circle *adres;

	id = 1;
	pamiec = shmget(SEM_ID, 1024, 0777|IPC_CREAT|IPC_EXCL);

	if(pamiec == -1) 
	{
		id++;
	}

	pamiec = shmget(SEM_ID, 1024, 0777|IPC_CREAT);

	adres = shmat(pamiec, 0, 0);

	if(id == 1) 
	{
		firts_player_status(adres);
	}
	
	id = new_player(adres);
	
    //XLIB setup
    mydisplay = XOpenDisplay("");
    myscreen = DefaultScreen(mydisplay);
    myvisual = DefaultVisual(mydisplay,myscreen);
    mydepth = DefaultDepth(mydisplay,myscreen);
    mywindowattributes.background_pixel = XWhitePixel(mydisplay,myscreen);
    mywindowattributes.override_redirect = False;
    mywindowattributes.backing_store = Always;
    mywindowattributes.bit_gravity = NorthWestGravity;
    mywindow = XCreateWindow(mydisplay,XRootWindow(mydisplay,myscreen),
                            100,100,600,600,10,mydepth,InputOutput,
                            myvisual,CWBackingStore|CWBackingPlanes|CWBitGravity|
                            CWBackPixel|CWOverrideRedirect,&mywindowattributes);
    XSelectInput(mydisplay,mywindow,ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonMotionMask);
    mycolormap = DefaultColormap(mydisplay,myscreen);
         XAllocNamedColor(mydisplay,mycolormap,"white",&mycolor,&dummy);   //tlo
         XAllocNamedColor(mydisplay,mycolormap,"red",&mycolor1,&dummy);    //berek
         XAllocNamedColor(mydisplay,mycolormap,"orange",&mycolor0,&dummy); //1 gracz (kolor dla 1 gracza gdy nie jest berkiem)
         XAllocNamedColor(mydisplay,mycolormap,"yellow",&mycolor2,&dummy); //2 gracz 
		 XAllocNamedColor(mydisplay,mycolormap,"green",&mycolor3,&dummy);  //3 gracz
		 XAllocNamedColor(mydisplay,mycolormap,"blue",&mycolor4,&dummy);   //4 gracz
		 XAllocNamedColor(mydisplay,mycolormap,"black",&mycolor5,&dummy);  //5 gracz
		 
         myname="Gra BEREK";
    XStoreName(mydisplay,mywindow,myname);
    XMapWindow(mydisplay,mywindow);
    mygc = DefaultGC(mydisplay,myscreen);
   
    game(adres, id);
	

	return 0;
}
