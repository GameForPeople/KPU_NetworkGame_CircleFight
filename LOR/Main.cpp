//----------------------
// 게임공학과 13학번(2013182027) 원성연
// STL 과제 - 2 Replay (FHW)
// 2017-06-08
//----------------------


#include <windows.h> 
#include <atlimage.h>
#include <time.h>
#include <math.h>
#include <MMSystem.h>
#include <iostream>

#include "ReplayFunction.h"

using namespace std;
//#include"Sound.h"
#pragma comment(lib, "winmm.lib" )
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define HELLO_LIMIT 5 // 돌을 몇개를 최소한 갖고 있어야 보스를 잡으러 갈 수 있는지의 여부를 설정하는 것1 시간 조절


#pragma region [STL EDIT - Replay 각 기능을 위한 전역변수]

CRep CReplay;
bool isReplay = true;		// F 플레이 T 리플레이
bool isShot = false;        // data shot 전역화
bool bufferShot = false;	// 중복 shot 방지

CImage speedTimeUI;			// 2배속 ui
bool isSpeedTime = 0;		// 0 1배속 1 2배속

CImage timeBarUI;			// 클릭에 따른 시간 위치 조절 UI
CImage timechangeLoadingUI; // 위 기능 로딩 UI
int hopeFrame = 0;			// 사용자가 원하는 프레임 
bool isTimeChange = false;	// F 안하는중 T 하는중

CImage stepUI;				// 1단계씩 실행 UI
CImage goUI;				// 위 기능 없애기!
int isStepByStep = 0;	// 특별히 INT! 경우의 수가 3,, 0 off 1 firstOn 2 on

#pragma endregion

#pragma region [SoundManger]
System*			pSystem;

//	사운드의 개수만큼 필요
Sound*			pSound[8];

//	동시에 출력될 사운드 만큼 필요
Channel*		pChannel[1];

#pragma endregion

#pragma region [WindowCreate]
HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "STL Homework 2 Replay - project_FHW [ 원성연 ]";

int i, j;	//?이친구들은 무얼위해존재하는것인가

static int GAME_MODE;					//게임의 모드 설정!!  0 -> 1 -> 2 -> 3

LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);
#pragma endregion

#pragma region [ClassGroup]

RECT rect;
struct Bos {
	int count;
	int count_atk;
	int count_atked;
	int count_atkmod;
	int mod;
	int speed;
	int posX;
	int posY;
	int HP;
	bool run;
	bool atked;
	bool atk;
	bool atkmod;
};
struct Ch {
	int x;
	int y;
	int count;
	int mod;
	int actmod;
	int atkmod;
	int speed;
	int select;
	int score;
	int HP;
	bool atk;
	bool walk;
};
struct actStruct {
	bool mod;
	int count;
	int speed;
	int y;
	bool button;
};
struct Atk {
	int x;
	int y;
	int xPlus;
	int yPlus;
	int count;
	int mod;
	bool atk;
};
struct Node {
	Atk arrow;
	Node *next;
};
Node *Head = new Node;


CImage Finn, FeelChart, Boss, BossL, Bullet[16], BossStage, Arrow[8], FinnAtk, HELLOWORLD, FinnDead, HPbar, WARP[11];

Ch main, Fruits, dog, woman, tree;
Bos B, bullet[8];

#pragma endregion

int Map;
int start = 0;
int  count_intro = 0;
int HELLO_count = 1;
bool HELLO[5];

//				GAME_MODE == 2  // 함수 편집으로 인한 변 수선언 위치 변경 -> 코드가 많이 더러워보임
/*
CImage END[56];
int END_onoff;
int END_TIMER;					//화면을 바꿔주는 인자값
int END_BUFFER;				    //시간을 체크하는 인자값

CImage FunShop[3];              //상점 회상 컷
int FunShop_count;				//상점 회상 컷 관련 변수
int FunShop_onoff;				//상점 회상 컷 관련 온 오프

CImage FunFish[18];              //낚시 회상 컷
int FunFish_count;				//낚시 회상 컷 관련 변수
int FunFish_onoff;				//낚시 회상 컷 관련 온 오프

CImage dream[3][10];
int dream_onoff;
int dream_count;
*/

bool KeyBuffer[256];

void Destroy_WARP()
{
	int i;
	for (i = 0; i < 11; i++)
	{
		WARP[i].Destroy();
	}
}
void Load_CImage() {
	Finn.Load("Resource/Finn.png");
	HELLOWORLD.Load("Resource/HELLOWORLD.png");
}
void Load_CImage_B() {
	Boss.Load("Resource/Dragon.png");
	BossL.Load("Resource/DragonL.png");
	HPbar.Load("Resource/HPbar.png");

	Bullet[0].Load("Resource/총알/0.png"); Bullet[1].Load("Resource/총알/1.png"); Bullet[2].Load("Resource/총알/2.png"); Bullet[3].Load("Resource/총알/3.png");
	Bullet[4].Load("Resource/총알/4.png"); Bullet[5].Load("Resource/총알/5.png"); Bullet[6].Load("Resource/총알/6.png"); Bullet[7].Load("Resource/총알/7.png");
	Bullet[8].Load("Resource/총알/8.png"); Bullet[9].Load("Resource/총알/9.png"); Bullet[10].Load("Resource/총알/10.png"); Bullet[11].Load("Resource/총알/11.png");
	Bullet[12].Load("Resource/총알/12.png"); Bullet[13].Load("Resource/총알/13.png"); Bullet[14].Load("Resource/총알/14.png"); Bullet[15].Load("Resource/총알/15.png");

	BossStage.Load("Resource/BossMap.jpg");

	Arrow[0].Load("Resource/화살/0.png"); Arrow[1].Load("Resource/화살/1.png"); Arrow[2].Load("Resource/화살/2.png"); Arrow[3].Load("Resource/화살/3.png");
	Arrow[4].Load("Resource/화살/4.png"); Arrow[5].Load("Resource/화살/5.png"); Arrow[6].Load("Resource/화살/6_2.png"); Arrow[7].Load("Resource/화살/7.png");

	FinnAtk.Load("Resource/atk.png");
	FinnDead.Load("Resource/dead.png");

	speedTimeUI.Load("Resource/speedTime.png");
	timeBarUI.Load("Resource/timeBar.png");
	timechangeLoadingUI.Load("Resource/timeChangeLoding.png");
	stepUI.Load("Resource/stepByStepUI.png");
	goUI.Load("Resource/goUI.png");
}

void CDestroy() {
	Finn.Destroy();

	FeelChart.Destroy();
	HELLOWORLD.Destroy();
	Boss.Destroy();
	BossL.Destroy();

	Bullet[0].Destroy(); Bullet[1].Destroy(); Bullet[2].Destroy(); Bullet[3].Destroy();
	Bullet[4].Destroy(); Bullet[5].Destroy(); Bullet[6].Destroy(); Bullet[7].Destroy();
	Bullet[8].Destroy(); Bullet[9].Destroy(); Bullet[10].Destroy(); Bullet[11].Destroy();
	Bullet[12].Destroy(); Bullet[13].Destroy(); Bullet[14].Destroy(); Bullet[15].Destroy();

	BossStage.Destroy();

	Arrow[0].Destroy(); Arrow[1].Destroy(); Arrow[2].Destroy(); Arrow[3].Destroy();
	Arrow[4].Destroy(); Arrow[5].Destroy(); Arrow[6].Destroy(); Arrow[7].Destroy();

	FinnAtk.Destroy();
	FinnDead.Destroy();

	HPbar.Destroy();

	speedTimeUI.Destroy();

	timeBarUI.Destroy();
	timechangeLoadingUI.Destroy();
	stepUI.Destroy();
	goUI.Destroy();
}

void DrawBoss(HDC Memdc) {
	if (B.atkmod) {
		if ((B.count_atk / 3) == 0)
			Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
				5, Boss.GetHeight() / 6 * 0, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		else
			Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
				Boss.GetWidth()* (B.count_atk / 3) / 30, Boss.GetHeight() / 6 * 0, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
	}
	else {
		if (B.mod == 0) {
			if (B.atked)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count_atked / 3 + 10), Boss.GetHeight() / 6 * 1, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6 + 20), Boss.GetHeight() / 6 * 1, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6), Boss.GetHeight() / 6 * 1, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 1) {
			if (B.atked)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count_atked / 3 + 10), BossL.GetHeight() / 6 * 3, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6), BossL.GetHeight() / 6 * 3, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6 + 20), BossL.GetHeight() / 6 * 3, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 2) {
			if (B.atked)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count_atked / 3 + 10), Boss.GetHeight() / 6 * 5, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6 + 20), Boss.GetHeight() / 6 * 5, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6), Boss.GetHeight() / 6 * 5, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 3) {
			if (B.atked)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count_atked / 3 + 10), Boss.GetHeight() / 6 * 3, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6 + 20), Boss.GetHeight() / 6 * 3, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6), Boss.GetHeight() / 6 * 3, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 4) {
			if (B.atked)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count_atked / 3 + 10), BossL.GetHeight() / 6 * 2, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6), BossL.GetHeight() / 6 * 2, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6 + 20), BossL.GetHeight() / 6 * 2, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 5) {
			if (B.atked)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count_atked / 3 + 10), BossL.GetHeight() / 6 * 4, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6), BossL.GetHeight() / 6 * 4, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
			else
				BossL.TransparentBlt(Memdc, B.posX, B.posY, BossL.GetWidth() / 30 * 2, BossL.GetHeight() / 6 * 2,
					BossL.GetWidth() / 30 * (B.count / 6 + 20), BossL.GetHeight() / 6 * 4, BossL.GetWidth() / 30, BossL.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 6) {
			if (B.atked)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count_atked / 3 + 10), Boss.GetHeight() / 6 * 2, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6 + 20), Boss.GetHeight() / 6 * 2, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6), Boss.GetHeight() / 6 * 2, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		}
		else if (B.mod == 7) {
			if (B.atked)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count_atked / 3 + 10), Boss.GetHeight() / 6 * 4, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else if (B.run)
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6 + 20), Boss.GetHeight() / 6 * 4, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
			else
				Boss.TransparentBlt(Memdc, B.posX, B.posY, Boss.GetWidth() / 30 * 2, Boss.GetHeight() / 6 * 2,
					Boss.GetWidth() / 30 * (B.count / 6), Boss.GetHeight() / 6 * 4, Boss.GetWidth() / 30, Boss.GetHeight() / 6, RGB(0, 255, 252));
		}
	}
}
void atk_plus(double x, double y, int mod) {
	Node *temp = new Node;
	temp->arrow.x = rect.right / 2 + 25;
	temp->arrow.y = rect.bottom / 2 + Finn.GetHeight() / 44;
	temp->arrow.xPlus = x;
	temp->arrow.yPlus = y;
	temp->arrow.mod = mod;
	temp->next = Head->next;
	Head->next = temp;
}
void arrowMove() {
	Node *curr = Head->next, *prev = Head;
	while (curr != nullptr) {
		curr->arrow.x = curr->arrow.x + curr->arrow.xPlus;
		curr->arrow.y = curr->arrow.y + curr->arrow.yPlus;
		if (curr->arrow.x<0 || curr->arrow.x>rect.right || curr->arrow.y<0 || curr->arrow.y>rect.bottom)
		{
			prev->next = curr->next;
			delete curr;
			curr = prev->next;
		}
		else if (curr->arrow.x > B.posX + Boss.GetWidth() / 60 && curr->arrow.x<B.posX + Boss.GetWidth() / 30 + Boss.GetWidth() / 60
			&& curr->arrow.y> B.posY + Boss.GetHeight() / 12 && curr->arrow.y < B.posY + Boss.GetHeight() / 6 + Boss.GetHeight() / 12) {
			prev->next = curr->next;
			delete curr;
			curr = prev->next;
			B.atked = true;
			B.speed = 5;

			if(!isReplay)
				B.HP--;
			
			if (B.HP == 0) {
				B.count = 0;
				PlaySound("Resource\\sound\\DragonDie.wav", NULL, SND_ASYNC);
				start = 2;
			}
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
}
void arrowKeyMove(int mod) {}
void Simul_Input() {
		if (Map == 2) {
			if (!start == 0) {
				if (KeyBuffer[VK_LEFT]) {
					main.walk = true;
					if (main.x > 215) {
						if (!((main.x > 435 && main.y > 450 && main.x <= 535 && main.y < 540) || (main.x > 995 && main.y > 850 && main.x <= 1095 && main.y < 940) || (main.x > 1115 && main.y > 490 && main.x <= 1255 && main.y < 580)
							|| (main.x > 1075 && main.y > 410 && main.x <= 1135 && main.y < 500) || (main.x > 1035 && main.y > 330 && main.x <= 1095 && main.y < 420)
							)) {
							main.x -= main.speed;
							B.posX += main.speed + 5;
							//B.posX += (B.speed+2);
							for (int i = 0; i < 8; ++i) {
								if (bullet[i].mod == 1)
									bullet[i].posX += main.speed + B.speed + 2;
							}
							arrowKeyMove(0);
						}
					}
					main.mod = 1;
				}
				if (KeyBuffer[VK_RIGHT]) {
					main.walk = true;
					if (main.x < 1235) {
						if (!((main.x >= 435 && main.y > 450 && main.x < 535 && main.y < 540) || (main.x >= 995 && main.y > 850 && main.x < 1095 && main.y < 940) || (main.x >= 1115 && main.y > 490 && main.x < 1255 && main.y < 580)
							|| (main.x >= 1075 && main.y > 410 && main.x < 1135 && main.y < 500) || (main.x >= 1035 && main.y > 330 && main.x < 1095 && main.y < 420)
							)) {
							main.x += main.speed;
							B.posX -= main.speed + 5;
							//	B.posX -= (B.speed + 2);
							for (int i = 0; i < 8; ++i) {
								if (bullet[i].mod == 1)
									bullet[i].posX -= main.speed + B.speed + 2;
							}
							arrowKeyMove(1);
						}
					}
					main.mod = 3;
				}
				if (KeyBuffer[VK_UP]) {
					main.walk = true;
					if (main.y > 340) {
						if (!((main.x > 435 && main.y > 450 && main.x < 535 && main.y <= 540) || (main.x > 995 && main.y > 850 && main.x < 1095 && main.y <= 940) || (main.x > 1115 && main.y > 490 && main.x < 1255 && main.y <= 580)
							|| (main.x > 1075 && main.y > 410 && main.x < 1135 && main.y <= 500) || (main.x > 1035 && main.y > 330 && main.x < 1095 && main.y <= 420)
							)) {
							main.y -= main.speed;
							B.posY += main.speed + 5;
							//B.posY += (B.speed + 2);
							for (int i = 0; i < 8; ++i) {
								if (bullet[i].mod == 1)
									bullet[i].posY += main.speed + B.speed + 2;
							}
							arrowKeyMove(2);
						}
					}
					main.mod = 2;
				}
				if (KeyBuffer[VK_DOWN]) {
					main.walk = true;
					if (main.y < 1050) {
						if (!((main.x > 435 && main.y >= 450 && main.x < 535 && main.y < 540) || (main.x > 995 && main.y >= 850 && main.x < 1095 && main.y < 940) || (main.x > 1115 && main.y >= 490 && main.x < 1255 && main.y < 580)
							|| (main.x > 1075 && main.y >= 410 && main.x < 1135 && main.y < 500) || (main.x > 1035 && main.y >= 330 && main.x < 1095 && main.y < 420)
							)) {
							main.y += main.speed;
							B.posY -= main.speed + 5;
							//B.posY -= (B.speed + 2);
							for (int i = 0; i < 8; ++i) {
								if (bullet[i].mod == 1)
									bullet[i].posY -= main.speed + B.speed + 2;
							}
							arrowKeyMove(3);
						}
					}
					main.mod = 0;
				}
			}
		}
}

void drawFinn(HDC Memdc) {
	if (main.actmod == 0) {
		if (!main.atk) {
			if (main.mod == 0) {
				Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
					0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 1), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
			}
			if (main.mod == 1) {
				Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
					0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 3), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
			}
			if (main.mod == 2) {
				Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
					0, Finn.GetHeight() / 44 * (main.count / 3 * 4), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
			}
			if (main.mod == 3) {
				Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
					0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 2), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
			}
		}
		else {
			if (main.atkmod == 0)
				FinnAtk.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, FinnAtk.GetHeight() / 4 * 2,
					0, FinnAtk.GetHeight() / 4 * 1, 25, FinnAtk.GetHeight() / 4, RGB(255, 0, 255));
			else if (main.atkmod == 1)
				FinnAtk.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, FinnAtk.GetHeight() / 4 * 2,
					0, FinnAtk.GetHeight() / 4 * 3, 25, FinnAtk.GetHeight() / 4, RGB(255, 0, 255));
			else if (main.atkmod == 2)
				FinnAtk.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, FinnAtk.GetHeight() / 4 * 2,
					0, FinnAtk.GetHeight() / 4 * 0, 25, FinnAtk.GetHeight() / 4, RGB(255, 0, 255));
			else if (main.atkmod == 3)
				FinnAtk.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, FinnAtk.GetHeight() / 4 * 2,
					0, FinnAtk.GetHeight() / 4 * 2, 25, FinnAtk.GetHeight() / 4, RGB(255, 0, 255));
		}
	}	
	else if (main.actmod == 3) {
		if (main.mod == 0) {
			Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
				0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 1 + 40), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
		}
		if (main.mod == 1) {
			Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
				0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 3 + 40), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
		}
		if (main.mod == 2) {
			Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
				0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 0 + 40), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
		}
		if (main.mod == 3) {
			Finn.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, Finn.GetHeight() / 44 * 2,
				0, Finn.GetHeight() / 44 * (main.count / 3 * 4 + 2 + 40), 25, Finn.GetHeight() / 44, RGB(255, 0, 255));
		}
	}
	else if (main.actmod == 4)
		FinnDead.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 50, FinnDead.GetHeight() / 4 * 2,
			0, FinnDead.GetHeight() / 4 * (main.count / 3), FinnDead.GetWidth(), FinnDead.GetHeight() / 4, RGB(255, 0, 255));
}
void intro(HDC Memdc) {
	CImage Intro[5];
	Intro[0].Load("Resource/인트로/0.png"); Intro[1].Load("Resource/인트로/1.png"); Intro[2].Load("Resource/인트로/2.png");
	Intro[3].Load("Resource/인트로/3.png"); Intro[4].Load("Resource/인트로/4.png");
	if (count_intro / 10 == 4)
		Intro[count_intro / 10].TransparentBlt(Memdc, 400 + 130, 50, Intro[count_intro / 10].GetWidth() * 2, Intro[count_intro / 10].GetHeight() * 2,
			0, 0, Intro[count_intro / 10].GetWidth(), Intro[count_intro / 10].GetHeight(), RGB(0, 0, 0));
	else
		Intro[count_intro / 10].TransparentBlt(Memdc, 485 + 130, 200, Intro[count_intro / 10].GetWidth() * 2, Intro[count_intro / 10].GetHeight() * 2,
			0, 0, Intro[count_intro / 10].GetWidth(), Intro[count_intro / 10].GetHeight(), RGB(0, 0, 0));
	Intro[0].Destroy(); Intro[1].Destroy(); Intro[2].Destroy(); Intro[3].Destroy(); Intro[4].Destroy();
}
void deadBoss(HDC Memdc) {
	CImage Dead[12];
	Dead[0].Load("Resource/Dead/0.png"); Dead[1].Load("Resource/Dead/1.png"); Dead[2].Load("Resource/Dead/2.png"); Dead[3].Load("Resource/Dead/3.png");
	Dead[4].Load("Resource/Dead/4.png"); Dead[5].Load("Resource/Dead/5.png"); Dead[6].Load("Resource/Dead/6.png"); Dead[7].Load("Resource/Dead/7.png");
	Dead[8].Load("Resource/Dead/8.png"); Dead[9].Load("Resource/Dead/9.png"); Dead[10].Load("Resource/Dead/10.png"); Dead[11].Load("Resource/Dead/11.png");
	if (B.count / 6 == 0)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 1)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 10 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 2)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 25 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 3)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 36 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 4)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 16 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 5)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 1 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 6)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY - 5 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 7)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY - 6 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 8)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 15 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 9)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 68 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 10)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 119 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	else if (B.count / 6 == 11)
		Dead[B.count / 6].TransparentBlt(Memdc, B.posX, B.posY + 159 * 2, Dead[B.count / 6].GetWidth() * 2, Dead[B.count / 6].GetHeight() * 2,
			0, 0, Dead[B.count / 6].GetWidth(), Dead[B.count / 6].GetHeight(), RGB(0, 0, 0));
	for (int i = 0; i < 12; ++i) {
		Dead[i].Destroy();
	}
}
void BossTimer() {
	if (start == 0) {
		count_intro++;
		if (count_intro == 50)start = 1;
	}
	else if (start == 2) {
		B.count++;
		//if (B.count == 50) {
		//GAME_MODE = 2; CDestroy(); Load_cImage_C();
		//}
		if (B.count == 72) {

			// 책갈피!! 게임이 끝날떄 체크해줘야할 곳입니다.!

		//skip_GAMEMODE_1:
			//GAME_MODE = 2; CDestroy();
			//Load_cImage_C();
		}
	}
	else {
		B.count_atkmod++;
		if (B.count_atkmod == 100)
		{
			B.count_atkmod = 0;

			if(!isTimeChange)
				PlaySound("Resource\\sound\\드래곤버스터.wav", NULL, SND_ASYNC);

			B.atk = false;
			for (int i = 0; i < 8; ++i) {
				bullet[i].mod = 1;
				bullet[i].posX = B.posX + Boss.GetWidth() / 30;
				bullet[i].posY = B.posY + Boss.GetHeight() / 6;
			}

			B.atkmod = true;

		}
		if (B.atkmod) {
			B.count_atk++;
			if (B.count_atk == 15)
				B.atk = true;
			if (B.count_atk == 30) {
				B.count_atk = 0; B.atkmod = false; B.count = 0;
			}
		}
		else {
			if (B.atked == false) {
				B.count++;
				if (B.count == 30)B.count = 0;
			}
			else {
				B.count_atked++;
				if (B.count_atked == 30) {
					B.count_atked = 0; B.atked = false; B.count = 0; B.speed = 10;
				}
			}
		}
		if (!B.atkmod) {
			if (!(rect.right / 2 + 25 + 100 >= B.posX + Boss.GetWidth() / 30 && rect.right / 2 + 25 - 100 <= B.posX + Boss.GetWidth() / 30
				&& rect.bottom / 2 + Finn.GetHeight() / 44 + 100 >= B.posY + Boss.GetHeight() / 6 && rect.bottom / 2 + Finn.GetHeight() / 44 - 100 <= B.posY + Boss.GetHeight() / 6)) {
				B.run = true;
			}
			if (B.run == true) {
				double x, y, cross;
				float Degree;
				x = rect.right / 2 + 25 - (B.posX + Boss.GetWidth() / 30);
				y = rect.bottom / 2 + Finn.GetHeight() / 44 - (B.posY + Boss.GetHeight() / 6);
				cross = sqrt(x*x + y*y);
				if (cross == 0)
					cross = 275;

				Degree = atan2f((float)y, (float)x) * 180 / 3.1415f;

				if (Degree > 15 && Degree < 75) {
					B.mod = 6;
				}
				else if (Degree < 105 && Degree>75) {
					B.mod = 0;
				}
				else if (Degree > 105 && Degree < 165) {
					B.mod = 4;
				}
				else if (Degree < -165 || Degree>165) {
					B.mod = 1;
				}
				else if (Degree > -165 && Degree < -105) {
					B.mod = 5;
				}
				else if (Degree > -105 && Degree < -75) {
					B.mod = 2;
				}
				else if (Degree <-15 && Degree>-75) {
					B.mod = 7;
				}
				else
					B.mod = 3;
				B.posX += B.speed * x / cross;
				B.posY += B.speed * y / cross;
				if ((rect.right / 2 + 25 + 100 >= B.posX + Boss.GetWidth() / 30 && rect.right / 2 + 25 - 100 <= B.posX + Boss.GetWidth() / 30
					&& rect.bottom / 2 + Finn.GetHeight() / 44 + 100 >= B.posY + Boss.GetHeight() / 6 && rect.bottom / 2 + Finn.GetHeight() / 44 - 100 <= B.posY + Boss.GetHeight() / 6)) {
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
					B.run = false;
				}
			}
		}
		if (B.atk == true) {
			int count_bullet = 0;
			if (bullet[0].mod == 1) {
				bullet[0].posY += 30;
				if (bullet[0].posX - 20 + 22 > rect.right / 2 - 20 && bullet[0].posX + 20 + 22 < rect.right / 2 + 50 + 20
					&& bullet[0].posY - 20 + 27 > rect.bottom / 2 - 20 && bullet[0].posY + 20 + 27 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[0].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				else if (bullet[0].posY > rect.bottom) {
					bullet[0].mod = 0;
					count_bullet++;
				}
				bullet[0].count++;
				if (bullet[0].count == 2)bullet[0].count = 0;
			}
			if (bullet[1].mod == 1) {
				bullet[1].posY += 20;
				bullet[1].posX -= 20;
				bullet[1].count++;
				if (bullet[1].count == 2)bullet[1].count = 0;
				if (bullet[1].posX - 20 + 20 > rect.right / 2 - 20 && bullet[1].posX + 20 + 20 < rect.right / 2 + 50 + 20
					&& bullet[1].posY - 20 + 25 > rect.bottom / 2 - 20 && bullet[1].posY + 20 + 25 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[1].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[1].posY > rect.bottom&&bullet[1].posX < 0) {
					bullet[1].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[2].mod == 1) {
				bullet[2].posX -= 30;
				bullet[2].count++;
				if (bullet[2].count == 2)bullet[2].count = 0;
				if (bullet[2].posX - 20 + 20 > rect.right / 2 - 20 && bullet[2].posX + 20 + 20 < rect.right / 2 + 50 + 20
					&& bullet[2].posY - 20 + 20 > rect.bottom / 2 - 20 && bullet[2].posY + 20 + 20 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[2].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[2].posX < 0) {
					bullet[2].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[3].mod == 1) {
				bullet[3].posY -= 20;
				bullet[3].posX -= 20;
				bullet[3].count++;
				if (bullet[3].count == 2)bullet[3].count = 0;
				if (bullet[3].posX - 20 + 20 > rect.right / 2 - 20 && bullet[3].posX + 20 + 20 < rect.right / 2 + 50 + 20
					&& bullet[3].posY - 20 + 30 > rect.bottom / 2 - 20 && bullet[3].posY + 20 + 30 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[3].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[3].posY < 0 && bullet[3].posX < 0) {
					bullet[3].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[4].mod == 1) {
				bullet[4].posY -= 30;

				bullet[4].count++;
				if (bullet[4].count == 2)bullet[4].count = 0;
				if (bullet[4].posX - 20 + 30 > rect.right / 2 - 20 && bullet[4].posX + 20 + 30 < rect.right / 2 + 50 + 20
					&& bullet[4].posY - 20 + 40 > rect.bottom / 2 - 20 && bullet[4].posY + 20 + 40 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[4].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[4].posY < 0) {
					bullet[4].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[5].mod == 1) {
				bullet[5].posY -= 20;
				bullet[5].posX += 20;
				bullet[5].count++;
				if (bullet[5].count == 2)bullet[5].count = 0;
				if (bullet[5].posX - 20 + 65 > rect.right / 2 - 20 && bullet[5].posX + 20 + 65 < rect.right / 2 + 50 + 20
					&& bullet[5].posY - 20 + 30 > rect.bottom / 2 - 20 && bullet[5].posY + 20 + 30 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[5].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[5].posY <0 && bullet[5].posX> rect.right) {
					bullet[5].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[6].mod == 1) {

				bullet[6].posX += 30;
				bullet[6].count++;
				if (bullet[6].count == 2)bullet[6].count = 0;
				if (bullet[6].posX - 20 + 70 > rect.right / 2 - 20 && bullet[6].posX + 20 + 70 < rect.right / 2 + 50 + 20
					&& bullet[6].posY - 20 + 20 > rect.bottom / 2 - 20 && bullet[6].posY + 20 + 20 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[6].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[6].posX > rect.right) {
					bullet[6].mod = 0;
					count_bullet++;
				}
			}
			if (bullet[7].mod == 1) {
				bullet[7].posY += 20;
				bullet[7].posX += 20;
				bullet[7].count++;
				if (bullet[7].count == 2)bullet[7].count = 0;
				if (bullet[7].posX - 20 + 50 > rect.right / 2 - 20 && bullet[7].posX + 20 + 50 < rect.right / 2 + 50 + 20
					&& bullet[7].posY - 20 + 25 > rect.bottom / 2 - 20 && bullet[7].posY + 20 + 25 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 20) {
					bullet[7].mod = 0;
					HELLO_count--;
					main.actmod = 3;
					main.count = 0;
					if (HELLO_count == 0) {
						main.actmod = 4;
					}
				}
				if (bullet[7].posY > rect.bottom&&bullet[7].posX > rect.right) {
					bullet[7].mod = 0;
					count_bullet++;
				}
			}
			if (count_bullet == 8)
				B.atk = false;
		}
	}
}
void mainAttak(int mx, int my) {
	double x, y, cross;
	float Degree;
	main.atk = true;
	main.count = 0;
	x = mx - (rect.right / 2 + 25);
	y = my - (rect.bottom / 2 + Finn.GetHeight() / 44);
	cross = sqrt(x*x + y*y);
	if (cross == 0)
		cross = 275;
	Degree = atan2f((float)y, (float)x) * 180 / 3.1415f;
	if (Degree > 15 && Degree < 75) {
		atk_plus(50 * x / cross, 50 * y / cross, 7);
		main.atkmod = 3;
	}
	else if (Degree < 105 && Degree>75) {
		atk_plus(50 * x / cross, 50 * y / cross, 0);
		main.atkmod = 0;
	}
	else if (Degree > 105 && Degree < 165) {
		atk_plus(50 * x / cross, 50 * y / cross, 4);
		main.atkmod = 1;
	}
	else if (Degree < -165 || Degree>165) {
		atk_plus(50 * x / cross, 50 * y / cross, 1);
		main.atkmod = 1;
	}
	else if (Degree > -165 && Degree < -105) {
		atk_plus(50 * x / cross, 50 * y / cross, 5);
		main.atkmod = 1;
	}
	else if (Degree > -105 && Degree < -75) {
		atk_plus(50 * x / cross, 50 * y / cross, 2);
		main.atkmod = 2;
	}
	else if (Degree <-15 && Degree>-75) {
		atk_plus(50 * x / cross, 50 * y / cross, 6);
		main.atkmod = 3;
	}
	else
	{
		atk_plus(50 * x / cross, 50 * y / cross, 3);
		main.atkmod = 3;
	}

}
void drawArrow(HDC Memdc) {
	Node *curr = Head->next;
	while (curr != nullptr) {
		//Ellipse(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, curr->arrow.x + 10, curr->arrow.y + 10);
		if (curr->arrow.mod == 0)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 20, 50,
				0, Arrow[curr->arrow.mod].GetHeight() / 2 * 1, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight() / 2, RGB(255, 0, 255));
		else if (curr->arrow.mod == 1)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				Arrow[curr->arrow.mod].GetWidth() / 2 * 0, 0, Arrow[curr->arrow.mod].GetWidth() / 2, Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		else if (curr->arrow.mod == 2)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 20, 50,
				0, Arrow[curr->arrow.mod].GetHeight() / 2 * 0, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight() / 2, RGB(255, 0, 255));
		else if (curr->arrow.mod == 3)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				Arrow[curr->arrow.mod].GetWidth() / 2 * 1, 0, Arrow[curr->arrow.mod].GetWidth() / 2, Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		else if (curr->arrow.mod == 4)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				0, 0, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		else if (curr->arrow.mod == 5)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				0, 0, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		else if (curr->arrow.mod == 6)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				0, 0, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		else if (curr->arrow.mod == 7)
			Arrow[curr->arrow.mod].TransparentBlt(Memdc, curr->arrow.x - 10, curr->arrow.y - 10, 50, 20,
				0, 0, Arrow[curr->arrow.mod].GetWidth(), Arrow[curr->arrow.mod].GetHeight(), RGB(255, 0, 255));
		curr = curr->next;
	}
}
void drawBullet(HDC Memdc) {
	for (int i = 0; i < 8; ++i) {
		if (bullet[i].mod == 1) {
			if (bullet[i].count == 0)
				Bullet[i * 2].TransparentBlt(Memdc, bullet[i].posX, bullet[i].posY, Bullet[i * 2].GetWidth() * 2, Bullet[i * 2].GetHeight() * 2,
					0, 0, Bullet[i * 2].GetWidth(), Bullet[i * 2].GetHeight(), RGB(0, 0, 0));
			else
				Bullet[i * 2 + 1].TransparentBlt(Memdc, bullet[i].posX, bullet[i].posY, Bullet[i * 2 + 1].GetWidth() * 2, Bullet[i * 2 + 1].GetHeight() * 2,
					0, 0, Bullet[i * 2 + 1].GetWidth(), Bullet[i * 2 + 1].GetHeight(), RGB(0, 0, 0));
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG Message;
	WNDCLASSEX WndClass;
	hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW || WS_BORDER,
		100, 30,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned)time(NULL));

	HDC hdc, Memdc;
	HBITMAP hBitmap, OldBitmap;
	PAINTSTRUCT ps;
	static int mx, my;
	char temp3[20];
	static int count, fishcount;
	HFONT hFont, saveFont;
	HBRUSH Brush, oldBrush;
	HPEN MyPen, OldPen, RedPen;
	//TCHAR *str = TEXT("원성연");

	//				GAME_MODE == 0
	static int WARP_count;
	static int WARP_onoff;
	static int WARP_timer;
	static int WARP_number;

	/*
	static CImage ZERO[30];					//첫 씬
	static int ZERO_onoff;
	static int ZERO_TIMER;					//화면을 바꿔주는 인자값
	static int ZERO_BUFFER;					//시간을 체크하는 인자값

	static CImage FIRST[30];				//두번째 씬
	static int FIRST_onoff;
	static int FIRST_TIMER;					//화면을 바꿔주는 인자값
	static int FIRST_BUFFER;				//시간을 체크하는 인자값

	*/
	static CImage TITLE[3];
	static int TITLE_onoff;
	static int TITLE_TIMER;					//화면을 바꿔주는 인자값
	static int TITLE_BUFFER;				//시간을 체크하는 인자값

											//				GAME_MODE == 1

	static bool	bPlay = true;				// 플레이 관련 변수 건들지 말기 
	static bool	IsBGPlaying = false;		// BGM 관련 재생 변수
											//static bool	IsEffectPlaying = false;
											//
											/*
											static int jump_function_onoff;
											static int jump_timer;
											static int jump_count;
											*/

	switch (iMessage) {
	case WM_CREATE: {
		GetClientRect(hwnd, &rect);
		SetTimer(hwnd, 1, 40, NULL);
		///////////////////////////////////////////////////////////////////////
		System_Create(&pSystem);
		//	사운드 시스템 기본 세팅
		pSystem->init(1, FMOD_INIT_NORMAL, NULL);
		//	음악파일 생성
		pSystem->createSound(
			"Resource//sound//WaltzBGM.mp3",	//	사운드파일경로
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,	//	사운드파일옵션
			NULL,			//	확장데이터(쓰지않음)
			&pSound[0]);	//	저장할 사운드 포인터

		pSystem->createSound(
			"Resource//sound//Bgm.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[1]);

		pSystem->createSound(
			"Resource//sound//mainBGM.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[2]);

		pSystem->createSound(
			"Resource//sound//noSOUND.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[3]);

		pSystem->createSound(
			"Resource//sound//endingBGM.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[4]);

		pSystem->createSound(
			"Resource//sound//CutBGM.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[5]);

		pSystem->createSound(
			"Resource//sound//CutBGM_2.mp3",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[6]);

		pSystem->createSound(
			"Resource//sound//Logo.wav",
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,
			NULL,
			&pSound[7]);

		pChannel[0]->isPlaying(&IsBGPlaying);

		//pChannel[0]->isPlaying(&IsEffectPlaying);

		//	pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[0]);

		//////////////////////////////////////////////////////////////////////
		GAME_MODE = 0;

		//////////////////////////////////////////////////////////////////////////////////// GAME_MODE == 0
		{
			////////////////워프로고작업추가1!
			WARP_count = 0;
			WARP_timer = 0;
			WARP_onoff = 1;
			WARP_number = 1;

			TITLE_TIMER = 2;
			TITLE_BUFFER = 0;

			WARP[0].Load("Resource/storyimage/워프_0.jpg");
			WARP[1].Load("Resource/storyimage/워프_1.jpg");
			WARP[2].Load("Resource/storyimage/워프_2.jpg");
			WARP[3].Load("Resource/storyimage/워프_3.jpg");
			WARP[4].Load("Resource/storyimage/워프_4.jpg");
			WARP[5].Load("Resource/storyimage/워프_5.jpg");
			WARP[6].Load("Resource/storyimage/워프_6.jpg");
			WARP[7].Load("Resource/storyimage/워프_7.jpg");
			WARP[8].Load("Resource/storyimage/워프_8.jpg");
			WARP[9].Load("Resource/storyimage/워프_9.jpg");
			WARP[10].Load("Resource/storyimage/워프_10.jpg");

			TITLE[0].Load("Resource/storyimage/title.jpg");
			TITLE[1].Load("Resource/storyimage/title_1.jpg");
			TITLE[2].Load("Resource/storyimage/title_loading.jpg");

		}
		//////////////////////////////////////////////////////////////////////////////////// GAME_MODE == 1
		{
			{
				//main.x = 2270;
				//main.y = 2270;
				main.x = 700;
				main.y = 800;
				main.count = 0;
				main.mod = 0;
				main.speed = 10;
				main.atk = false;

				Load_CImage();
				Load_CImage_B();
				B.mod = 0;
				B.count = 0;
				B.posX = 400;
				B.posY = -250;
				B.speed = 10;
				B.count_atk = 0;
				B.atk = false;
				B.HP = 50;
				for (int i = 0; i < 5; ++i) {
					HELLO[i] = false;
				}
				Head->next = nullptr;

				Map = 2;
			}
		}
		break;
	}
	case WM_PAINT:
	{
		//if (!isTimeChange) {
			// 더블 버퍼링*********************

			HDC mainHDC = BeginPaint(hwnd, &ps);
#pragma region [Doubble buffer]
			HBITMAP GLay = CreateCompatibleBitmap(mainHDC, SCREEN_WIDTH, SCREEN_HEIGHT);
			HDC hdc = CreateCompatibleDC(mainHDC);
			SelectObject(hdc, GLay);
			Rectangle(hdc, -10, -10, SCREEN_WIDTH + 20, SCREEN_HEIGHT + 20);
#pragma endregion

			SetStretchBltMode(mainHDC, COLORONCOLOR);
			SetStretchBltMode(hdc, COLORONCOLOR);
			//*********************************
			if (GAME_MODE == 0)
			{
				if (WARP_onoff)
					WARP[WARP_count].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT, SRCCOPY);
				/*
				else if (ZERO_onoff)
				ZERO[ZERO_TIMER].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT, SRCCOPY);
				else if (FIRST_onoff)
				FIRST[FIRST_TIMER].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT, SRCCOPY);
				*/
				else if (TITLE_onoff)
					TITLE[TITLE_TIMER].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
				/*
				else if (STORY_onoff)
				STORY[STORY_TIMER].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
				*/
			}
			else if (GAME_MODE == 1)
			{
				Memdc = CreateCompatibleDC(hdc);
				GetClientRect(hwnd, &rect);
				hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
				OldBitmap = (HBITMAP)SelectObject(Memdc, hBitmap);
				FillRect(Memdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));


				if (Map == 2) {
					BossStage.StretchBlt(Memdc, 0, 0, rect.right, rect.bottom, main.x, main.y, rect.right * 2 / 3, rect.bottom * 2 / 3, SRCCOPY);
					//Rectangle(Memdc, B.posX + Boss.GetWidth() / 60, B.posY + Boss.GetHeight() / 12, B.posX + Boss.GetWidth() / 30 + Boss.GetWidth() / 60, B.posY + Boss.GetHeight() / 6 + Boss.GetHeight() / 12);

					if (start == 0)
						intro(Memdc);
					else if (start == 2)
						deadBoss(Memdc);
					else
						DrawBoss(Memdc);

					drawArrow(Memdc);
					Brush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(Memdc, Brush);

					RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
					OldPen = (HPEN)SelectObject(Memdc, RedPen);

					for (i = 0; i < B.HP; i++)
					{
						Rectangle(Memdc, 360 + 700 / (50) * i, 615, 360 + 700 / (50) * (i + 1), 645);
						//Rectangle(Memdc, 360 + i * 7 , 615, 360 + (i + 1) * 7 , 645);
					}

					HPbar.TransparentBlt(Memdc, 240, 580, 840, 100, RGB(0, 0, 0));
					
					RedPen = CreatePen(PS_SOLID, 1, RGB(242, 221, 222));
					OldPen = (HPEN)SelectObject(Memdc, RedPen);

					if (GAME_MODE == 1 && isReplay) {
						for (i = 0; i < CReplay.getNowFrame() ; i++)
						{
							Rectangle(Memdc, 610 + (float)600 / CReplay.getFrameCount() * i, 38, 610 + (float)600 / CReplay.getFrameCount() * (i + 1), 95);
						}
						if(isStepByStep)
							goUI.TransparentBlt(Memdc, 240, 30, 80, 80, RGB(255, 255, 255));

						stepUI.TransparentBlt(Memdc, 135, 30, 80, 80, RGB(255, 255, 255));
						speedTimeUI.TransparentBlt(Memdc, 30, 30, 80, 80, RGB(255, 255, 255));

						timeBarUI.TransparentBlt(Memdc, 600, 25, 600, 85, RGB(255, 255, 255));
					}

					SelectObject(Memdc, oldBrush);
					DeleteObject(Brush);

					SelectObject(Memdc, OldPen);
					DeleteObject(RedPen);

					if (B.HP > 0)
					{
						if (B.atk)drawBullet(Memdc);
					}
				}

				//HELLOWORLD.TransparentBlt(Memdc, 0, 0, 50 * HELLO_count, 50,
				//	HELLOWORLD.GetWidth() * 0, 0, HELLOWORLD.GetWidth() / 10 * HELLO_count, HELLOWORLD.GetHeight(), RGB(255, 255, 255));

				drawFinn(Memdc);

				if (isTimeChange) {
					timechangeLoadingUI.AlphaBlend(Memdc, 0, 0, 1280, 720, 0, 0, 1280, 720, 80, AC_SRC_OVER);
					timechangeLoadingUI.TransparentBlt(Memdc, 0, 0, 1280, 720, RGB(0,0, 0));
					//HPbar.TransparentBlt(Memdc, 240, 580, 840, 100, RGB(0, 0, 0));
				}


				BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Memdc, 0, 0, SRCCOPY);
				hBitmap = (HBITMAP)SelectObject(Memdc, OldBitmap);
				DeleteDC(Memdc);
				/*ReleaseDC(hwnd, hdc);*/
				DeleteObject(hBitmap);
			}
			//EndPaint(hwnd, &ps);


			SetStretchBltMode(mainHDC, COLORONCOLOR);
			SetStretchBltMode(hdc, COLORONCOLOR);
			//-----------------------------------
			/*
			SelectObject(hdc, oldBrush);
			DeleteObject(Brush);


			SelectObject(hdc, OldPen);
			DeleteObject(MyPen);
			DeleteObject(RedPen);
			*/
			//*********************************
#pragma region [Doubble buffer]
			BitBlt(mainHDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

			//DeleteDC(Memdc);
			DeleteDC(hdc);
			DeleteObject(GLay);
			EndPaint(hwnd, &ps);
#pragma endregion
			//*********************************
		//}
		
	/*	
#pragma region [NotNeeD]
		if (isTimeChange) {
			HDC mainHDC = BeginPaint(hwnd, &ps);
#pragma region [Doubble buffer]
			HBITMAP GLay = CreateCompatibleBitmap(mainHDC, SCREEN_WIDTH, SCREEN_HEIGHT);
			HDC hdc = CreateCompatibleDC(mainHDC);
			SelectObject(hdc, GLay);
			Rectangle(hdc, -10, -10, SCREEN_WIDTH + 20, SCREEN_HEIGHT + 20);
#pragma endregion

			SetStretchBltMode(mainHDC, COLORONCOLOR);
			SetStretchBltMode(hdc, COLORONCOLOR);

			Memdc = CreateCompatibleDC(hdc);
			GetClientRect(hwnd, &rect);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
			OldBitmap = (HBITMAP)SelectObject(Memdc, hBitmap);



			BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Memdc, 0, 0, SRCCOPY);
			hBitmap = (HBITMAP)SelectObject(Memdc, OldBitmap);
			DeleteDC(Memdc);
			DeleteObject(hBitmap);
		
			SetStretchBltMode(mainHDC, COLORONCOLOR);
			SetStretchBltMode(hdc, COLORONCOLOR);

#pragma region [Doubble buffer]
			BitBlt(mainHDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

			//DeleteDC(Memdc);
			DeleteDC(hdc);
			DeleteObject(GLay);
			EndPaint(hwnd, &ps);
#pragma endregion
		}
		#pragma endregion
	*/	
		break;
	}
	case WM_TIMER:
	{
		if (GAME_MODE == 0)
		{
			if (WARP_onoff) //매직넘버 사용
			{
				WARP_timer++;

				if (WARP_count == 0 && WARP_number == 1 && WARP_timer == 1)
				{
					//pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[7], false, &pChannel[0]);
				}

				if (WARP_count == 0 && WARP_number == 1 && WARP_timer == 10)
				{
					WARP_count = WARP_count + WARP_number;
					WARP_timer = 0;
				}
				else if (WARP_number == 1 && WARP_count > 0 && WARP_count < 10 && WARP_timer == 1)
				{
					WARP_count = WARP_count + WARP_number;
					WARP_timer = 0;
				}
				else if (WARP_number == -1 && WARP_count < 10 && WARP_timer == 1)
				{
					WARP_count = WARP_count + WARP_number;
					WARP_timer = 0;
				}
				else if (WARP_count == 10 && WARP_timer == 40)
				{
					WARP_number = -1;
					WARP_count = 9;
					WARP_timer = 0;
				}

				if (WARP_number == -1 && WARP_count == -1)
				{
					WARP_onoff = 0;
					TITLE_onoff = 1;
				}
			}
			else if (TITLE_onoff)
			{
				if (TITLE_BUFFER == 0) {
					TITLE_TIMER = 2;
					pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[0]);
				}

				TITLE_BUFFER++;

				if (TITLE_BUFFER == 60)
				{
					if (isReplay)
						TITLE_TIMER = 1;
					else
						TITLE_TIMER = 0;

						TITLE_BUFFER = 61;
				}
			}		
			// 책갈피 -> 메뉴에서 넘어 갈떄! 
			//GAME_MODE = 1;
			//pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[2], false, &pChannel[0]);
			//
			//TITLE[0].Destroy();
			//TITLE[1].Destroy();
			//Destroy_WARP();
		}
		else if (GAME_MODE == 1)
		{
			if (isReplay) {

				KeyBuffer[VK_UP] = CReplay.getReplayData().getInputUp();
				KeyBuffer[VK_DOWN] = CReplay.getReplayData().getInputDown();
				KeyBuffer[VK_RIGHT] = CReplay.getReplayData().getInputRight();
				KeyBuffer[VK_LEFT] = CReplay.getReplayData().getInputLeft();

				if (CReplay.getReplayData().getDir() < 4) {
					if(!KeyBuffer[VK_UP] && !KeyBuffer[VK_DOWN] && !KeyBuffer[VK_LEFT] && !KeyBuffer[VK_RIGHT])
						main.walk = false;

					main.mod = CReplay.getReplayData().getDir();
				}
				else {
					main.atk = true;
					main.atkmod = CReplay.getReplayData().getDir() - 4;
					mainAttak(CReplay.getReplayData().getMouseX(), CReplay.getReplayData().getMouseY());
				}

				//B.posX += ( 700 - main.x );
				//B.posY -= ( 800 -  main.y );
				B.HP = CReplay.getReplayData().getBossHP();

				if(CReplay.getFrameCount() - 1 != CReplay.getNowFrame())
					CReplay.PlusNowframe();
			}

			Simul_Input();

			if (isTimeChange) {
				if (CReplay.getNowFrame() == hopeFrame) {
					KillTimer(hwnd, 1);

					if(isSpeedTime)
						SetTimer(hwnd, 1, 20, NULL);
					else if(!isSpeedTime)
						SetTimer(hwnd, 1, 40, NULL);

					isTimeChange = false;
				}
			}
			//STL
			if (!isReplay) {
				if (!main.atk)
					//CReplay.SaveFrame(main.x, main.y, mx, my, main.mod);
					CReplay.SaveFrame(KeyBuffer[VK_UP], KeyBuffer[VK_DOWN], KeyBuffer[VK_LEFT], KeyBuffer[VK_RIGHT], mx, my, main.mod, B.HP);
				else if (!bufferShot) {
					CReplay.SaveFrame(KeyBuffer[VK_UP], KeyBuffer[VK_DOWN], KeyBuffer[VK_LEFT], KeyBuffer[VK_RIGHT], mx, my, main.atkmod + 4, B.HP);
					bufferShot = true;
				}
				else if (bufferShot) {
					CReplay.SaveFrame(KeyBuffer[VK_UP], KeyBuffer[VK_DOWN], KeyBuffer[VK_LEFT], KeyBuffer[VK_RIGHT], mx, my, main.mod, B.HP);
					bufferShot = false;
				}

			}

			// 나 저장하고!! 초기화야!!!
			if (HELLO_count <= 0 || B.count == 72) {
				
				if (!isReplay && CReplay.getClose()) {
					isReplay = false;
					CReplay.EndSave();
				}

				// 초기화를 진행합니다.
				// 전역부의 변수들의 초기화 입니다.
				isReplay = false;		// 플레이어가 리플레이를 원할 경우 이 변수를 통해 마우스, 키보드 입력 권한을 제어! F -> T 될때, 파일 읽어야함!
				isShot = false;
				bufferShot = false;
				start = 0;
				count_intro = 0;
				HELLO_count = 1;
				GAME_MODE = 0;

				// 타이틀 타이머 변수들의 초기화입니다.
				TITLE_TIMER = 2;
				TITLE_BUFFER = 0;
				TITLE_onoff = 1;

				// 전투 씬 관련 변수들의 초기화입니다.
				main.x = 700;
				main.y = 800;
				main.count = 0;
				main.mod = 0;
				main.speed = 10;
				main.atk = false;
				B.mod = 0;
				B.count = 0;
				B.posX = 400;
				B.posY = -250;
				B.speed = 10;
				B.count_atk = 0;
				B.atk = false;
				B.HP =50;
				B.atked = false;
				B.atkmod = false;
				B.count_atked = 0;
				B.count_atkmod = 0;
	
				for (int i = 0; i < 5; ++i) {
					HELLO[i] = false;
				}
				Head->next = nullptr;

				Map = 2;

				// 리플레이 데이터 초기화입니다.
				CReplay.InitData();

				// 조작데이터 초기화입니다.
				for (int i = 0; i < 256; i++)
					KeyBuffer[i] = false;
			}

			if (main.actmod == 0) {
				if (main.atk == false) {
					if (main.walk) {
						main.count++;
						if (main.count == 24)main.count = 0;
					}
				}
				else
				{
					main.count++;
					if (main.count == 3) { main.count = 0; main.atk = false; }
				}
				if (Map == 2) {
					arrowMove();
					BossTimer();
				}
			}
		}

		isShot = false;

		if (isStepByStep == 2) {
			KillTimer(hwnd, 1);
			isStepByStep = 1;
		}

		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
			if (GAME_MODE == 1)
			{
				if (!isReplay) {
					{
						if (wParam == 'P' || wParam == 'p') {
							if (HELLO_count < 10)
							{
								HELLO_count++;
								PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
							}
						}
						else if (wParam == VK_SPACE) {
						}
					}
					KeyBuffer[wParam] = true;

					/*
					if (wParam == VK_UP)
						bufferUp = true;
					if (wParam == VK_DOWN)
						bufferDown = true;
					if (wParam == VK_LEFT)
						bufferLeft = true;
					if (wParam == VK_RIGHT)
						bufferRight = true;*/
				}
			}
			else if (GAME_MODE == 0)
			{
				if (wParam == VK_UP || wParam == VK_DOWN) {
					if (!isReplay) {
						isReplay = true;
						TITLE_TIMER = 1;
						PlaySound("Resource\\sound\\push.wav", NULL, SND_ASYNC);	//Push Start Button - "any key setting"

					}
					else if (isReplay) {
						isReplay = false;
						TITLE_TIMER = 0;
						PlaySound("Resource\\sound\\push.wav", NULL, SND_ASYNC);	//Push Start Button - "any key setting"

					}
				}
				else if (TITLE_BUFFER > 60)	// 노래 조금 들려줘야지!!
					{

					if (isReplay) {
						CReplay.LoadReplayData();
					}

					KillTimer(hwnd, 1);
					SetTimer(hwnd, 1, 40, NULL);

					isSpeedTime = false;

						TITLE_onoff = 0;	// 
											//책갈피 게임 시작				
						Map = 2;
						GAME_MODE = 1;
						main.actmod = 0;

						//HELLO[0] = true;HELLO[1] = true;HELLO[2] = true;HELLO[3] = true;HELLO[4] = true;

						//FMOD_Channel_Stop(channel);  -> FMOD:: Channel 관련 수정작업 필요
						pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[0], false, &pChannel[0]);	//NoSOund
						PlaySound("Resource\\sound\\push.wav", NULL, SND_ASYNC);	//Push Start Button - "any key setting"
					}				
			}
			//InvalidateRgn(hwnd, NULL, false);
			else if (isReplay) {}
		break;
	case WM_KEYUP:
		if (!isReplay) {
			if (GAME_MODE == 1)
			{
				KeyBuffer[wParam] = false;

				/*
				if (wParam == VK_UP)
					bufferUp = false;
				if (wParam == VK_DOWN)
					bufferDown = false;
				if (wParam == VK_LEFT)
					bufferLeft = false;
				if (wParam == VK_RIGHT)
					bufferRight = false;
					*/

				InvalidateRgn(hwnd, NULL, false); //case 마지막으로 옮김
			}

			if (!KeyBuffer[VK_UP] && !KeyBuffer[VK_DOWN] && !KeyBuffer[VK_LEFT] && !KeyBuffer[VK_RIGHT])
				main.walk = false;
			else main.walk = true;
		}
		else if (isReplay) {	}
		break;
	case WM_LBUTTONDOWN:
		if (!isReplay) {
			if (Map == 2 && GAME_MODE == 1 && start != 0 && B.count != 72) {
				my = HIWORD(lParam);
				mx = LOWORD(lParam);
				isShot = true;
				mainAttak(mx, my);
				InvalidateRect(hwnd, NULL, false); //case 마지막으로 옮김!
			}
		}
		else if(isReplay) {
			int mY = HIWORD(lParam);
			int mX = LOWORD(lParam);

			if (30 <= mX && mX <= 110 && 30 <= mY && mY <= 110) {
					KillTimer(hwnd, 1);

					if (isSpeedTime)
						SetTimer(hwnd, 1, 40, NULL);
					else if (!isSpeedTime)
						SetTimer(hwnd, 1, 20, NULL);

					isSpeedTime = !isSpeedTime;
			}
			else if (130 <= mX && mX <= 215 && 30 <= mY && mY <= 110) {
				
				if (isStepByStep == 0)
					isStepByStep = 2;
				else if (isStepByStep == 1) {

					if (isSpeedTime)
						SetTimer(hwnd, 1, 40, NULL);
					else if (!isSpeedTime)
						SetTimer(hwnd, 1, 20, NULL);

					isStepByStep = 2;
				}
			}
			else if (235 <= mX && mX <= 320 && 30 <= mY && mY <= 110) {

				if (isStepByStep)
					isStepByStep = 0;

				if (isSpeedTime)
					SetTimer(hwnd, 1, 40, NULL);
				else if (!isSpeedTime)
					SetTimer(hwnd, 1, 20, NULL);

			}
			else if (600 <= mX && mX <= 1200 && 25 <= mY && mY <= 110) {			
				hopeFrame = CReplay.getFrameCount() * ((mX / 6) - 100) / 100;
				isTimeChange = true;
				KillTimer(hwnd, 1);
				SetTimer(hwnd, 1, 0.1 , NULL);

				if (hopeFrame < CReplay.getNowFrame()) {
					CReplay.setNowFrame(0);

					//---
					isShot = false;
					bufferShot = false;
					start = 0;
					count_intro = 0;
					//---
					main.x = 700;
					main.y = 800;
					main.count = 0;
					main.mod = 0;
					main.speed = 10;
					main.atk = false;
					B.mod = 0;
					B.count = 0;
					B.posX = 400;
					B.posY = -250;
					B.speed = 10;
					B.count_atk = 0;
					B.atk = false;
					B.HP = 50;
					B.atked = false;
					B.atkmod = false;
					B.count_atked = 0;
					B.count_atkmod = 0;

					for (int i = 0; i < 5; ++i) {
						HELLO[i] = false;
					}
					Head->next = nullptr;

					Map = 2;
					//---
					for (int i = 0; i < 256; i++)
						KeyBuffer[i] = false;
					//---
				}
				else if (hopeFrame >= CReplay.getNowFrame()) {
				}
			}
		}
		break;
	case WM_RBUTTONDOWN:
		if (GAME_MODE == 0) {
			if (!isReplay) {
				isReplay = true;
				TITLE_TIMER = 1;
			}
			else if (isReplay) {
				isReplay = false;
				TITLE_TIMER = 0;
			}

			InvalidateRect(hwnd, NULL, false); //case 마지막으로 옮김!
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}
