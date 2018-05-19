#include <allegro.h>
#include<allegro/alcompat.h>
#include <cstdlib>
#include<iostream>
#include<sstream>

#define MAXFILAS 20 // eje y
#define MAXCOLS 33  //eje x
using namespace std;

struct nodo {
    char item;
    struct nodo *sgte;
};

typedef nodo *Puntero;

Puntero p=NULL;//puntero de pila

int puntaje=0;
int bonusRojo=0;
int bonusAzul=0;
int anteriorpx, anteriorpy;
int px=30*14, py=30*17;
int dir=4;//indica la direccion
int vidas=3;
int s=0; //contador para la pila
int d=570;
char w;
/*   Declaracion de BITMAP */
BITMAP *buffer;
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;
BITMAP *muertebmp;
BITMAP *diamante1;
BITMAP *diamante2;
BITMAP *fondo1;
BITMAP *fondo2;
BITMAP *fondo3;
BITMAP *cursor;
BITMAP *win;
BITMAP *loser;
/* ---- declaracion del audio ----*/
MIDI *musica1;


/*----Funcion Apilar valor----*/
void push( char valor )
{
     Puntero aux;
     aux = new(struct nodo);
     aux->item = valor;
     cout<<endl<<aux->item;
     aux->sgte = p ;
     p = aux ;
}
/*---Funcion que es llamada cuando Pacman pierde una vida  ----*/
int pop(  )
{   char it;

     Puntero aux;
     if(p!=NULL){
     aux = p ;
     it = aux->item;
     cout<<endl<<"desapilado:"<<it<<endl;
     p = aux->sgte ;
     delete(aux);
     } else cout<<endl<<"no hay elementos en la pila"<<endl;


}

/*Funcion que muestra el elemento apilado en pantalla*/
void mostrar_pila(  )
{   int t=0;
     Puntero aux;
     aux = p;     // apunta al inicio de la lista

     while( aux !=NULL )
     {      if(aux->item=='L')//diamante azul
            draw_sprite(buffer,diamante1,30*30,d-t*30);//resta el tamaño de pantalla-30 para posicionar un diamante

            if(aux->item=='M')//diamante Rojo
            draw_sprite(buffer,diamante2,30*30,d-t*30);

            aux = aux->sgte;
            t++;
     }
}

/*---- Funcion que desapila y destruye la pila -----*/
void destruir_pila( )
{
     Puntero aux;

     while( p != NULL)
     {
           aux = p;
           p = aux->sgte;
           if(aux->item=='L')
                bonusAzul=bonusAzul+100;
           if(aux->item=='M')
                bonusRojo=bonusRojo+200;
           delete(aux);

     }
}

/*----- Declaracion del Arreglo del mapa -----*/
char mapa[MAXFILAS][MAXCOLS]={
	 "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",
	 "XOOOO|OOOOO XXXXX OOOOO|OOOMX  ",
	 "XOXXX XXXXXLXXXXXOXXXXXOXXXOX  ",
	 "XOXXX XXXXX XXXXXOXXXXXOXXXOX  ",
	 "XOOLOOOOOO |OOOOOO|OOOOMOOOOX  ",
	 "XOXXX XX|XXXXXXXXXX |XX XXXOX  ",
	 "XOOOO|XXOOOOOXXX O OOXX|OOOOX  ",
	 "XOXXX XXXXXXLXXX XXXXXX XXXOX  ",
	 "XLXXX XX OOM OOOO O  XX XXX X  ",
	 "OOOOOOXX|XXXXXXXXXXX|XXOOOOOO  ",
	 "X XXX XX XXXXXXXXXXXOXX XXX X  ",
	 "XOXXX|XXOOOOOOMOOOOOOXX XXX X  ",
	 "XOXXX XXXXXX XXX XXXXXX XXX X  ",
	 "XOOOO XXOOOOOXXXOOOOOXX OOOOX  ",
	 "XMXXXOXXOXXXXXXXXXXX XX XXX X  ",
	 "XOXXX  O O |OOOOOO|OOOOOXXXLX  ",
	 "XOXXX XXXX0XXXXXXXXOXXXLXXXOX  ",
	 "XOXXXOXXXX  O OO O OXXX XXXOX  ",
	 "XOOLOO O O XXXXXXXX OOOOOO OX  ",
	 "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",
};
/*-------------Funcion para Dibujar bloques, monedas, diamantes  ---------*/
void dibujar_mapa()
{
	int row, col;
	for (row = 0 ; row<MAXFILAS ; row++)
	{
		for (col = 0 ; col<MAXCOLS ; col++)
		{
			if(mapa[row][col] == 'X')
			{
				draw_sprite(buffer, roca, col*30, row*30);
			}
			else if(mapa[row][col] == 'O')
            {
                draw_sprite(buffer, comida, col*30, row*30);
                if(py/30 == row && px/30 == col)/*Si pacman come una moneda, desaparcerla*/
                  {
                      mapa[row][col] = ' ';
                      puntaje=puntaje+10;

                  }
            }
            else if(mapa[row][col]=='L')/*  pacman come un diamante azul*/
            {
                draw_sprite(buffer,diamante1,col*30,row*30);
                if(py/30 == row && px/30 == col)
                  {
                      mapa[row][col] = ' ';
                      push('L');

                    }
            }

            if(mapa[row][col]=='M')/*  pacman come un diamante rojo*/
            {
                draw_sprite(buffer,diamante2,col*30,row*30);
                if(py/30 == row && px/30 == col)
                  {
                      mapa[row][col] = ' ';
                      push('M');

                    }
            }
            char c1[10];
            textout_ex(buffer, font, "Vidas:", 885, 30,  0xfff000, -1);
            textout_ex(buffer, font, itoa(vidas,c1,10), 895, 60,  0xfff000, -1);//muestra las vidas en pantalla
            mostrar_pila();//dibujar diamantes apilados en pantalla
		}
	}
}
/*----- Funcion que dibuja todo lo de buffer en pantalla principal -------*/
void pantalla()
{
	blit(buffer, screen, 0, 0, 0, 0,940, 600);
}

/*------- Muestra en pantalla a pacman -------*/
void dibujar_pacman()
{
    blit(pacbmp,pacman,dir*33,0,0,0,33,33);// imprimir imagen de pacman
    draw_sprite(buffer, pacman, px,py);
}
/*----------Funcion que determina si las monedas se acabaron------*/
bool win_juego(){
    int row, col;

	for (row = 0 ; row<MAXFILAS ; row++){
		for (col = 0 ; col<MAXCOLS ; col++){
			if(mapa[row][col] == 'O')  return true;
		}
	}
	return false;
}

/*------- Funcion que activa el fin del juego si es que las vidas llegan a cero ------*/
bool lose_juego(){
    if(vidas==0) return true;
    return false;
}

/*---------------------Implementacion de la clase fantasma-----------*/
class fantasma
{
    BITMAP *enemigobmp;
    BITMAP *enemigo;

    int fdir ;
    int _x, _y;

public:
       fantasma(int x, int y); //construcctor de parametros
       void dibujar_fantasma() const;
       void mover_fantasma();
       void choque_pacman();
};
/*---------- Constructor de la clase ------------*/
fantasma::fantasma(int x, int y)
{
    _x = x;
    _y = y;
    fdir = rand()%4;
    enemigobmp = load_bitmap("images/enemigo.bmp",NULL);
    enemigo = create_bitmap(30,30);
}


void fantasma::dibujar_fantasma()const
{
     blit(enemigobmp,enemigo ,0,0,0,0,30,30);
     draw_sprite(buffer, enemigo, _x,_y);
}

/*---------- Funcion que determina si hay colision entre pacman y un fantasma ------------*/
void fantasma::choque_pacman()
{
    if(py== _y && px == _x || _y ==anteriorpy && _x==anteriorpx)
    {

        for(int j=0;j<=5;j++)
        {
            clear(pacman);
            clear(buffer);
            dibujar_mapa();

            blit(muertebmp, pacman,j*33,0,0,0,33,33);
            draw_sprite(buffer,pacman,px,py);

            pantalla();
            rest(80);

        }
        vidas--;
        cout<<endl<<"vidas"<<vidas<<endl;
        pop();//desapilar TOS
        px =30*14;
        py =30*17;
        dir =4;
    }
}

/*-------------- Funcion para mover un fantasma --------------*/
void fantasma::mover_fantasma()
{
    dibujar_fantasma();
    choque_pacman();
    if(mapa[_y/30][_x/30] == '|'){
        fdir = rand()%4;
        }
    if(fdir == 0)
    {
       if(mapa[_y/30][(_x-30)/30]!= 'X') _x-=30;
            else fdir = rand()%4;
    }
    if(fdir == 1)
    {
       if(mapa[_y/30][(_x+30)/30]!= 'X') _x+=30;
            else fdir = rand()%4;
    }
    if(fdir == 2)
    {
       if(mapa[(_y-30)/30][(_x)/30]!= 'X') _y-=30;
            else fdir = rand()%4;
    }
    if(fdir == 3)
    {
       if(mapa[(_y+30)/30][(_x)/30]!= 'X') _y+=30;
            else fdir = rand()%4;
    if(_x <= -30) _x=870;
            else if(_x >= 870) _x = -30;
}
}


/*--------------------------Funcion Principal --------------------------*/
int main()
{
    bool fin_menu;
    bool lose;
    bool salir;
    char c1[10],c2[10];

	allegro_init();
	install_keyboard();
    install_mouse();

	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 940, 600, 0, 0);

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
        {   allegro_message("Error: Inicializando sistema de sonido\n%s\n", allegro_error);
            return 1;
        }
    set_volume(70, 70); //Control del volumen


    /*----- Cargamos todos las imagenes a su respectivo BITMAP---*/
	fondo1 = load_bitmap("menu/FONDO1.bmp", NULL);
	fondo2 = load_bitmap("menu/FONDO2.bmp", NULL);
	fondo3 = load_bitmap("menu/FONDO3.bmp", NULL);
	cursor = load_bitmap("menu/cursor.bmp", NULL);

    roca = load_bitmap ("images/roca.bmp", NULL);
	pacbmp = load_bitmap("images/pacman.bmp", NULL);
    comida = load_bitmap("images/Comida.bmp", NULL);
    muertebmp= load_bitmap ("images/muerte.bmp", NULL);
    diamante1=load_bitmap("images/diamante1.bmp",NULL);
    diamante2=load_bitmap("images/diamante2.bmp",NULL);

    win=load_bitmap("images/WIN.bmp",NULL);
    loser=load_bitmap("images/LOSE.bmp",NULL);

    musica1=load_midi("sounds/mario.mid");



    /* ------- Implementacion del Menu -----*/
	buffer =create_bitmap(800,530);
    fin_menu=false;

     while(!fin_menu){
        if(mouse_x>211&&mouse_x<553&&mouse_y>319&&mouse_y<374)
        {
            blit(fondo2,buffer,0,0,60,0,800,530);
            if(mouse_b&1){
                fin_menu=true;
            }
        } else if(mouse_x>311&&mouse_x<404&&mouse_y>418&&mouse_y<465){
                  blit(fondo3,buffer,0,0,60,0,800,530);
                  if(mouse_b&1){
                    fin_menu=true;
                    return 0;
                  }

    }else blit(fondo1,buffer,0,0,60,0,800,530);

    masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);
    blit(buffer,screen,0,0,60,0,880,600);

    }
    /* ------- Fin de Implementacion del Menu -----*/

    clear(buffer);/*  Liberamos los registros cargados*/
    clear(cursor);
    clear(fondo1);
    clear(fondo2);
    clear(fondo3);

    /*-- Creamos los enemigos en distintas posiciones ---*/
    fantasma A(30*2,30*3);
    fantasma B(30*15,30*15);
    fantasma C(30*3,30*3);
    fantasma D(30*18,30*18);
    fantasma E(30*9,30*9);

    /*  Implementamos la pantalla del juego  */
    buffer =create_bitmap(940,600);
	pacman = create_bitmap(33,33);
    play_midi(musica1,1);//iniciamos musica de fondo
	while(win_juego()==true&&lose_juego()==false )
	{
        if(key[KEY_ESC]) return 0;

        /*---- Funcion para manejar el teclado ----*/
	    anteriorpx = px;
	    anteriorpy = py;
	    if(key[KEY_RIGHT]) dir=1;
	    else if(key[KEY_LEFT]) dir=0;
	    else if(key[KEY_UP]) dir=2;
	    else if(key[KEY_DOWN]) dir=3;

	    if(dir == 0)
        {
            if(mapa[py/30][(px-30)/30]!='X')
                px -= 30;
            else dir = 4;
        }
	    if(dir == 1)
	    {
            if(mapa[py/30][(px+30)/30]!='X')
               px += 30;
            else dir = 4;
	    }
	    if(dir == 2)
	    {
            if(mapa[(py-30)/30][(px)/30]!='X')
               py -= 30;
            else dir = 4;
	    }
	    if(dir == 3)
	    {
            if(mapa[(py+30)/30][(px)/30]!='X')
               py += 30;
            else dir = 4;
	    }
	    if(px <= -30) px = 870;
	     else if (px>=870) px = -30;

        /*---- Fin de Funcion para manejar el teclado ----*/

	    clear(buffer);//limpiamos pantalla

		dibujar_mapa();
		dibujar_pacman();
        A.mover_fantasma();
        B.mover_fantasma();
        C.mover_fantasma();
        D.mover_fantasma();
        E.mover_fantasma();
		pantalla();
		rest(70);

		clear (pacman);
		blit(pacbmp,pacman,4*33,0,0,0,33,33);// imprimir imagen de pacman
        draw_sprite(buffer, pacman, px,py);
        pantalla();
        rest(90);

	}		/*---- Fin de la implentacion del juego ----*/

	/*-------- Limpiamos todos los buffers ------*/
	clear(buffer);
	clear(pacman);
	clear(roca);
	clear(pacbmp);
	clear(comida);
	clear(muertebmp);
	clear(diamante1);
	clear(diamante2);
	clear(screen);


    destruir_pila();// calculamos el bonus obtenido

    salir=false;
    int Total=puntaje+bonusAzul+bonusRojo;
    while(!salir){
    /*-----  Mostramos pantalla cuando perdemos  ------*/
    if(lose_juego()==true){

        blit(loser,buffer,0,0,60,0,880,600);
        textout(buffer,font,"Tu Puntaje es:",220,350,pallete_color[14]);
        textout_ex(buffer, font, itoa(puntaje,c1,10), 240, 380,  0xfff000, -1);
        textout(buffer,font,"Bonus Rojo(200) :      Bonus  Azul(100):",220,410,pallete_color[14]);
        textout_ex(buffer, font, itoa(bonusRojo,c2,10), 300, 430,  0xfff000, -1);
        textout_ex(buffer, font, itoa(bonusAzul,c2,10), 450, 430,  0xfff000, -1);
        textout(buffer,font,"Total de Puntos :",220,460,pallete_color[14]);
        textout_ex(buffer, font, itoa(Total,c2,10), 400, 460,  0xfff000, -1);
        textout(buffer,font,"Presione ESC o ENTER para salir",220,490,pallete_color[14]);
        blit(buffer,screen,0,0,0,0,880,600);
        midi_pause();
        if(key[KEY_ESC]||key[KEY_ENTER]){
            salir=true;
        }
    }
    /*----- Mostramos pantalla cuando ganamos ------*/
    if(win_juego()==false){
        blit(win,buffer,0,0,60,0,880,600);
        textout(buffer,font,"Tu Puntaje es:",220,350,pallete_color[14]);
        textout_ex(buffer, font, itoa(puntaje,c1,10), 240, 380,  0xfff000, -1);
        textout(buffer,font,"Bonus Rojo(200) :      Bonus  Azul(100):",220,410,pallete_color[14]);
        textout_ex(buffer, font, itoa(bonusRojo,c2,10), 300, 430,  0xfff000, -1);
        textout_ex(buffer, font, itoa(bonusAzul,c2,10), 450, 430,  0xfff000, -1);
        textout(buffer,font,"Total de Puntos :",220,460,pallete_color[14]);
        textout_ex(buffer, font, itoa(Total,c2,10), 400, 460,  0xfff000, -1);
        textout(buffer,font,"Presione ESC o ENTER para salir",220,490,pallete_color[14]);
        blit(buffer,screen,0,0,0,0,880,600);
        midi_pause();
        if(key[KEY_ESC]||key[KEY_ENTER]){
            salir=true;
        }
    }
    }
    /*--- Destruimos todos los BITMAP usados en el juego para liberar memoria de tarjeta grafica----*/
    destroy_midi(musica1);
    destroy_bitmap(buffer);
    destroy_bitmap(win);
    destroy_bitmap(loser);
    destroy_bitmap(pacbmp);
    destroy_bitmap(pacman);
    destroy_bitmap(comida);
    destroy_bitmap(roca);
    destroy_bitmap(muertebmp);
    destroy_bitmap(diamante1);
    destroy_bitmap(diamante2);
}
END_OF_MAIN();
