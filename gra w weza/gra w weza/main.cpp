#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include"winbgi2.h"
#define D 400
#define _CRT_SECURE_NO_WARNINGS

void rysuj(int *x, int *y, int jx, int jy);
void ruch(int *x, int *y, int k, int n);
void jablko(int &jx, int &jy, int *x, int *y);
void highscore();
bool skucha(int *x, int *y);
int p = 1, kolor = 2, v = 10;

int main()
{
	int *x, *y;
	x = (int *)malloc(p*sizeof(int));
	y = (int *)malloc(p*sizeof(int));
	x[0] = D / 2; y[0] = D / 2 + 100;
	int k=0, k1=0, jx, jy;

	srand(time(NULL));
	graphics(D, D+100);
	jablko(jx, jy, x, y);
	rysuj(x,y,jx,jy);
	for (;;)
	{
		if (kbhit() == 1)
		{
			k1 = getch();
			if (((k1 == 'w')&&(k != 's')) || ((k1 == 's')&&(k != 'w')) || ((k1 == 'a')&&(k != 'd')) || ((k1 == 'd')&&(k != 'a')))  k = k1; 
		}
		ruch(x, y, k, p-1);
		if ((jx - x[0])*(jx - x[0]) + (jy - y[0])*(jy - y[0]) < 200) 
		{
			jablko(jx, jy, x ,y);
			p++;
			x = (int*)realloc(x, p*sizeof(int)+sizeof(*x));
			y = (int*)realloc(y, p*sizeof(int)+sizeof(*y));
		}
		if (mousedown() == 1)
		{
			if ((mouseclickx() > 100) && (mouseclickx() < 120) && (mouseclicky() > 45) && (mouseclicky() < 65))
			{ 
				if(whichmousebutton() == 0 ) kolor++; if (kolor > 15) kolor = 0; 
				if (whichmousebutton() == 1) kolor--; if (kolor < 0) kolor = 15;
			}
			if ((mouseclickx() > D - 50) && (mouseclickx() < D - 30) && (mouseclicky() > 35) && (mouseclicky() < 55)&&(v<20)) v++;
			if ((mouseclickx() > D - 50) && (mouseclickx() < D - 30) && (mouseclicky() > 55) && (mouseclicky() < 75)&&(v>5)) v--;
		}
		if (skucha(x, y) == true) break;
		rysuj(x, y, jx, jy);
		animate(10);
	}
	settextstyle(0, 0, 3);
	outtextxy(D/2 - 125, D/2+50, "GAME OVER");
	animate(10);
	highscore();
	free(x); free(y);
	wait();
	return 0;
}

void rysuj(int *x, int *y, int jx, int jy)
{
	cleardevice();
	system("cls");
	line(0, 100, D, 100);
	line(0, 0, D, 0);
	line(0, 0, 0, D + 100);
	line(0, D + 100, D, D + 100);
	line(D, 0, D, D + 100);

	char pkt[12] = "punkty: ";
	if (p-1 < 10)  pkt[8] = p + 47; 
	else if ((p-1 >= 10) && (p-1 < 100)) { pkt[8] = (p-1) / 10+48; pkt[9] = (p-1) - (p-1) / 10*10 + 48; }
	else if (p-1 >= 100) { pkt[8] = (p-1) / 100 + 48; pkt[9] = ((p-1) - (p-1)/100*100)/10 + 48; pkt[10] = (p-1) - (p-1)/100*100 - ((p-1) - (p-1)/100*100)/10*10 + 48;}
	outtextxy(D / 2 - 55, 50, pkt);

	setfillstyle(1, kolor);
	outtextxy(30, 50, "kolor:");
	rectangle(100, 45, 120, 65);
	for (int i = 0; i < p; i++) { rectangle(x[i] - 10, y[i] - 10, x[i] + 10, y[i] + 10); }

	setfillstyle(1, RED);
	fillellipse(jx, jy, 10, 10);
	outtextxy(D - 150, 50, "predkosc: ");

	setfillstyle(0, BLACK);
	rectangle(D - 50, 35, D - 30, 75);
	line(D - 50, 55, D - 30, 55);
	line(D - 45, 65, D - 35, 65);
	line(D - 45, 45, D - 35, 45);
	line(D - 40, 40, D - 40, 50);
}

void ruch(int *x, int *y, int k, int n)
{
	if (n>0)
	{
		x[n] = x[n - 1];
		y[n] = y[n - 1];
		ruch(x, y, k, n - 1);
	}
	else
	{
		switch (k)
		{
		case 'w':
			y[0] -= v;
			break;
		case 's':
			y[0] += v;
			break;
		case 'a':
			x[0] -= v;
			break;
		case 'd':
			x[0] += v;
			break;
		default:
			break;
		}
	}
}

void jablko(int &jx, int &jy, int *x, int *y)
{
	bool q;
	do
	{
		q = true;
		jx = rand() % (D - 30) + 15;
		jy = rand() % (D - 30) + 115;
		for (int i = 0; i < p; i++) if ((jx>x[i] - 20) && (jx<x[i] + 20) && (jy>y[i] - 20) && (jy < y[i] + 20)) q = false;
	} while (q == false);
}

bool skucha(int *x, int *y)
{
	if ((x[0]<10) || (x[0]>D-10) || (y[0]<110) || (y[0]>D+90)) return true;
	if (p>1) for (int i = 1; i < p; i++)  if ((x[0]>x[i] - 5) && (x[0]<x[i] + 5) && (y[0]>y[i] - 5) && (y[0] < y[i] + 5)) return true; 
	return false;
}

void highscore()
{
	char imie[11][20];
	int punkty[11];
	punkty[10] = p - 1;
	FILE *f;

	system("cls");
	printf("koniec gry! twoj wynik: %d\n", p-1);
	printf("podaj swoje imie: ");
	scanf("%19s", imie[10]);

	f = fopen("highscore.txt", "r");
	if (f == NULL) { printf("blad otwarcia pliku"); exit(0); }
	for (int i = 0; i < 10; i++) fscanf(f, "%s %d", imie[i], &punkty[i]);
	fclose(f);

	f = fopen("highscore.txt", "w");
	printf("    HIGHSCORE    \n");
	printf("-----------------\n");
	for (int i = 0; i < 10; i++)
	{
		if (punkty[10]>punkty[i]) 
		{
			printf("%s %d\n", imie[10], punkty[10]); 
			fprintf(f, "%s %d\n", imie[10], punkty[10]);
			for (int j = i; j < 9; j++) { fprintf(f, "%s %d\n", imie[j], punkty[j]); printf("%s %d\n", imie[j], punkty[j]);}
			break;
		}
		else { fprintf(f, "%s %d\n", imie[i], punkty[i]); printf("%s %d\n", imie[i], punkty[i]); }
	}
	fclose(f);
}