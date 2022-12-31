#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h>		
#pragma comment ( lib, "Winmm.lib" )


#define NUM		13		
#define PI      3.1415926548


struct FIRE
{
	int r;					
	int max_r;				
	int x, y;			
	int cen_x, cen_y;		
	int width, height;		
	int xy[240][240];		

	bool show;			
	bool draw;			
	DWORD t1, t2, dt;		
}Fire[NUM];


struct JET
{
	int x, y;				
	int hx, hy;			
	int height;				
	bool shoot;			

	DWORD t1, t2, dt;		
	IMAGE img[2];			
	byte n : 1;				
}Jet[NUM];




void welcome();
void Init(int);		// 初始化烟花
void Load();		// 加载烟花图片
void Shoot();		// 发射烟花
void Chose(DWORD&);		// 筛选烟花
void Style(DWORD&);		// 发射样式
void Show(DWORD*);		// 绽放烟花


void main()
{
	initgraph(1200, 800);
	srand(time(0));

	mciSendString("open ./file/studio.mp3 alias bk", 0, 0, 0);
	mciSendString("play bk repeat", 0, 0, 0);

	welcome();

	DWORD t1 = timeGetTime();			
	DWORD st1 = timeGetTime();			
	DWORD* pMem = GetImageBuffer();		

	for (int i = 0; i < NUM; i++)	
	{
		Init(i);
	}
	Load();							
	BeginBatchDraw();					

	while (!kbhit())
	{
		Sleep(10);

		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % 1200;
				int py1 = rand() % 800;

				if (py1 < 799)			
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK;	
			}
		}
		Chose(t1);			
		Shoot();			
		Show(pMem);			
		Style(st1);			
		FlushBatchDraw();	 
	}
}


void welcome()
{
	//setfillstyle(0);
	setcolor(YELLOW);
	
	for (int i = 0; i < 50; i++)
	{
		int x = 600 + int(180 * sin(PI * 2 * i / 60));
		int y = 200 + int(180 * cos(PI * 2 * i / 60));
		cleardevice();
		settextstyle(i, 0, "楷体");
		outtextxy(x-80, y, "2023");
		outtextxy(x-10, y+100, "祝各位新年快乐");
		Sleep(25);
	}

	Sleep(2000);
	cleardevice();
	settextstyle(25, 0, "楷体");
	outtextxy(400, 270, "     爆竹声中一岁除,");
	outtextxy(400, 320, "     春风送暖入屠苏。");
	outtextxy(400, 370, "     千门万户曈曈日,");
	outtextxy(400, 420, "     总把新桃换旧符。");
	outtextxy(400, 470, "              ----(宋)王安石《元日》");
	Sleep(2000);
	cleardevice();
	settextstyle(25, 0, "楷体");
	outtextxy(400, 170, "       让烦恼结束,");
	outtextxy(400, 220, "       让快乐起步,");
	outtextxy(400, 270, "       让悠闲瞩目。");
	outtextxy(400, 320, "         新年到,");
	outtextxy(400, 370, "   借新升的太阳把你祝福,");
	outtextxy(400, 420, "  愿你新年吉祥,幸福每一步。");
	outtextxy(400, 470, " 陪好运走一路,把健康当归宿。");
	Sleep(2000);
	cleardevice();
}
void Init(int i)
{
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
	int x[13] = { 120, 120, 110, 117, 110, 93, 102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85, 118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };


	Fire[i].x = 0;			
	Fire[i].y = 0;
	Fire[i].width = 240;			
	Fire[i].height = 240;	
	Fire[i].max_r = r[i];		
	Fire[i].cen_x = x[i];			
	Fire[i].cen_y = y[i];
	Fire[i].show = false;			
	Fire[i].dt = 5;				
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;				
	
	Jet[i].x = -240;				
	Jet[i].y = -240;
	Jet[i].hx = -240;				
	Jet[i].hy = -240;
	Jet[i].height = 0;				
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;		
	Jet[i].n = 0;				
	Jet[i].shoot = false;			
}

void Load()
{
	IMAGE fm, gm;
	loadimage(&fm, "./file/flower.jpg", 3120, 240);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		SetWorkingImage(&gm);

		for (int a = 0; a < 240; a++)
		for (int b = 0; b < 240; b++)
			Fire[i].xy[a][b] = getpixel(a, b);
	}

	IMAGE sm;
	loadimage(&sm, "./file/shoot.jpg", 200, 50);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&sm);
		int n = rand() % 5;
		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);			
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);		
	}
	SetWorkingImage();		
}

void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 20;
		if (n < 13 && Jet[n].shoot == false && Fire[n].show == false)
		{
			Jet[n].x = rand() % 1200;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n], SRCINVERT);

			/**** 播放每个烟花弹的声音 *****/
			/*char c1[50], c2[30], c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", n);
			sprintf(c2, "play s%d", n);
			sprintf(c3, "close n%d", n);
			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);*/
		}
		t1 = t2;
	}
}


void Shoot()
{
	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();
		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);
			if (Jet[i].y > Jet[i].hy)
			{Jet[i].n++;et[i].y -= 5;}
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			if (Jet[i].y <= Jet[i].hy)
			{
				char c1[50], c2[30], c3[30];
				sprintf(c1, "open ./file/bomb.wav alias n%d", i);
				sprintf(c2, "play n%d", i);
				sprintf(c3, "close s%d", i);
				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);
				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	
				Fire[i].x = Jet[i].hx + 10;										
				Fire[i].y = Jet[i].hy;											
				Fire[i].show = true;					
				Jet[i].shoot = false;					

			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}



void Style(DWORD& st1)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 >20000)		
	{
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < NUM; i++){
			//cleardevice();
			/**** 规律分布烟花弹 ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	
			
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;

			/**** 播放发射声音 ***/
			/*char c1[50], c2[30], c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", i);
			sprintf(c2, "play s%d", i);
			sprintf(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);*/
		}
		st1 = st2;
	}
}



void Show(DWORD* pMem)
{
	
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };
	for (int i = 0; i < NUM; i++)
	{
		Fire[i].t2 = timeGetTime();
		if (Fire[i].t2 - Fire[i].t1 > Fire[i].dt && Fire[i].show == true)
		{
			if (Fire[i].r < Fire[i].max_r)
			{
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r / 10];
				Fire[i].draw = true;
			}

			if (Fire[i].r >= Fire[i].max_r - 1)
			{
				Fire[i].draw = false;
				Init(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}
		if (Fire[i].draw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));				
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));
				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height)	
				{
					int b = Fire[i].xy[x1][y1] & 0xff;
					int g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (Fire[i].xy[x1][y1] >> 16);
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));
					
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800)
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x1][y1]);	
				}
			}
			Fire[i].draw = false;
		}
	}
}
