///////////////////////////////////////////////////////////////////////////////////////
// 안녕하세요!! First Hello World 소스 코드 세상에 오신 여러분을 환영합니다.         //
// 저는 First Hello World project에서 Design & Art 를 담당한 13학번 원성연입니다.    //
// 코드 같은 경우 최초에 같은 프로젝트 팀원인 15학번 강동균이 최초 작성하였으며      //
// 동균이가 군입대를 한 후에 과제전을 목표로 제가 코드를 추가하고 수정하게되었습니다.//
// 이 과정에서 코드가 많이 더러워지고 보기 끔찍하게 바뀌는 일이 발생했습니다.        //
// 보시는 분의 정신건강을 위해 코드를 안 보시는 걸 추천드립니다.                     //
//																					 //
// 마지막으로 동균아 건강히 제대해!!!												 //
//																					 //
// 행복한 하두 되세요!																 //
//																					 //
// 2016년 8월 30일 원성연				 010-9769-0703								 //
///////////////////////////////////////////////////////////////////////////////////////

#include <windows.h> 
#include <atlimage.h>
#include <time.h>
#include <math.h>
#include <MMSystem.h>
#include <iostream>

using namespace std;
//#include"Sound.h"
#pragma comment(lib, "winmm.lib" )
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define HELLO_LIMIT 5 // 돌을 몇개를 최소한 갖고 있어야 보스를 잡으러 갈 수 있는지의 여부를 설정하는 것1 시간 조절

System*			pSystem;

//	사운드의 개수만큼 필요
Sound*			pSound[8];

//	동시에 출력될 사운드 만큼 필요
Channel*		pChannel[1];

HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "STL Homework2 Replay - First Hello World";

int i, j;
static int GAME_MODE;					//게임의 모드 설정!!  0 -> 1 -> 2 -> 3

LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);

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

//CSoundManager* sound;

CImage bk, Finn, talk, talkMask, fishing, fishGage[2], fishPoint[2], Dog, Timber, maze, blind, Tree, TimberGage, TimberPoint, FeelChart,
Boss, BossL, Bullet[16], BossStage, Arrow[8], FinnAtk, HELLOWORLD, fruits[3], FinnDead, VillageChief[9], ShopKeeper[3], HouseWife[4], Kid[3], Fisherman[3], TImberman[3]
, Soldier[4], People[14], Captin, Qmark, HPbar, parable[14], WARP[11], bird[9], bat[3], jump[2];

Ch main, Fruits, dog, woman, tree;
Bos B, bullet[8];
int Map;
int start = 0;
int  count_intro = 0;
int HELLO_count = 0;
bool HELLO[5];
bool Talk;

int Skip_Gamemode_1 = 0;

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
actStruct fish, timber, chief, shop, house, kid, fisher, timberman, soldier, people, captin;

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
	maze.Load("Resource/미로.png");
	blind.Load("Resource/blind3.jpg");
	bk.Load("Resource/월드맵.jpg");
	Finn.Load("Resource/Finn.png");
	talkMask.Load("Resource/말풍선마스크.jpg");
	fishing.Load("Resource/행동/낚시.png");
	fishGage[0].Load("Resource/행동/낚시게이지마스크.jpg");
	fishGage[1].Load("Resource/행동/낚시게이지.jpg");
	fishPoint[0].Load("Resource/행동/낚시포인트마스크.jpg");
	fishPoint[1].Load("Resource/행동/낚시포인트.jpg");
	Dog.Load("Resource/NPC/개.png");
	Tree.Load("Resource/행동/나무.jpg");
	Timber.Load("Resource/행동/벌목.png");
	TimberGage.Load("Resource/행동/벌목게이지.jpg");
	TimberPoint.Load("Resource/행동/벌목포인트.jpg");
	FeelChart.Load("Resource/NPC/느낌표.png");
	Qmark.Load("Resource/NPC/물음표.png");
	HELLOWORLD.Load("Resource/HELLOWORLD.png");
	fruits[0].Load("Resource/행동/감자.jpg");
	fruits[1].Load("Resource/행동/사과.jpg");
	fruits[2].Load("Resource/행동/옥수수.jpg");

	VillageChief[0].Load("Resource/대사/촌장0.jpg"); VillageChief[1].Load("Resource/대사/촌장1.jpg"); VillageChief[2].Load("Resource/대사/촌장2.jpg");
	VillageChief[3].Load("Resource/대사/촌장3.jpg"); VillageChief[4].Load("Resource/대사/촌장4.jpg"); VillageChief[5].Load("Resource/대사/촌장5.jpg");
	VillageChief[6].Load("Resource/대사/촌장6.jpg"); VillageChief[7].Load("Resource/대사/촌장7.jpg"); VillageChief[8].Load("Resource/대사/촌장8.jpg");

	ShopKeeper[0].Load("Resource/대사/가게주인1.jpg"); ShopKeeper[1].Load("Resource/대사/가게주인2.jpg"); ShopKeeper[2].Load("Resource/대사/가게주인3.jpg");

	HouseWife[0].Load("Resource/대사/전업주부1.jpg"); HouseWife[1].Load("Resource/대사/전업주부2.jpg");
	HouseWife[2].Load("Resource/대사/전업주부3.jpg"); HouseWife[3].Load("Resource/대사/전업주부4.jpg");

	Kid[0].Load("Resource/대사/꼬마아이0.jpg"); Kid[1].Load("Resource/대사/꼬마아이1.jpg"); Kid[2].Load("Resource/대사/꼬마아이2.jpg");

	Fisherman[0].Load("Resource/대사/낚시꾼0.jpg"); Fisherman[1].Load("Resource/대사/낚시꾼1.jpg"); Fisherman[2].Load("Resource/대사/낚시꾼2.jpg");

	TImberman[0].Load("Resource/대사/나무꾼0.jpg"); TImberman[1].Load("Resource/대사/나무꾼1.jpg"); TImberman[2].Load("Resource/대사/나무꾼2.jpg");

	Soldier[0].Load("Resource/대사/병사0.jpg"); Soldier[1].Load("Resource/대사/병사1.jpg"); Soldier[2].Load("Resource/대사/병사2.jpg"); Soldier[3].Load("Resource/대사/병사3.jpg");

	People[0].Load("Resource/대사/마을사람0.jpg"); People[1].Load("Resource/대사/마을사람1.jpg"); People[2].Load("Resource/대사/마을사람2.jpg"); People[3].Load("Resource/대사/마을사람3.jpg");
	People[4].Load("Resource/대사/마을사람4.jpg"); People[5].Load("Resource/대사/마을사람5.jpg"); People[6].Load("Resource/대사/마을사람6.jpg"); People[7].Load("Resource/대사/마을사람7.jpg");
	People[8].Load("Resource/대사/마을사람8.jpg"); People[9].Load("Resource/대사/마을사람9.jpg"); People[10].Load("Resource/대사/마을사람10.jpg"); People[11].Load("Resource/대사/마을사람11.jpg");
	People[12].Load("Resource/대사/마을사람12.jpg"); People[13].Load("Resource/대사/마을사람13.jpg");

	bird[0].Load("Resource/새/bird_0.png");
	bird[1].Load("Resource/새/bird_1.png");
	bird[2].Load("Resource/새/bird_2.png");
	bird[3].Load("Resource/새/bird_3.png");
	bird[4].Load("Resource/새/bird_4.png");
	bird[5].Load("Resource/새/bird_5.png");
	bird[6].Load("Resource/새/bird_6.png");
	bird[7].Load("Resource/새/bird_7.png");
	bird[8].Load("Resource/새/bird_8.png");

	bat[0].Load("Resource/박쥐/bat_1.png");
	bat[1].Load("Resource/박쥐/bat_1.png");
	bat[2].Load("Resource/박쥐/bat_1.png");

	Captin.Load("Resource/대사/선장.jpg");
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
}

/*
void Load_cImage_C()
{
	dream[1][0].Load("Resource/storyimage/dream_1_0.jpg");
	dream[1][1].Load("Resource/storyimage/dream_1_1.jpg");
	dream[1][2].Load("Resource/storyimage/dream_1_2.jpg");
	dream[1][3].Load("Resource/storyimage/dream_1_3.jpg");
	dream[1][4].Load("Resource/storyimage/dream_1_4.jpg");
	dream[1][5].Load("Resource/storyimage/dream_1_5.jpg");
	dream[1][6].Load("Resource/storyimage/dream_1_6.jpg");
	dream[1][7].Load("Resource/storyimage/dream_1_7.jpg");
	dream[1][8].Load("Resource/storyimage/dream_1_8.jpg");
	dream[1][9].Load("Resource/storyimage/dream_1_9.jpg");

	dream[2][0].Load("Resource/storyimage/dream_2_0.jpg");
	dream[2][1].Load("Resource/storyimage/dream_2_1.jpg");
	dream[2][2].Load("Resource/storyimage/dream_2_2.jpg");
	dream[2][3].Load("Resource/storyimage/dream_2_3.jpg");
	dream[2][4].Load("Resource/storyimage/dream_2_4.jpg");
	dream[2][5].Load("Resource/storyimage/dream_2_5.jpg");
	dream[2][6].Load("Resource/storyimage/dream_2_6.jpg");
	dream[2][7].Load("Resource/storyimage/dream_2_7.jpg");
	dream[2][8].Load("Resource/storyimage/dream_2_8.jpg");
	dream[2][9].Load("Resource/storyimage/dream_2_9.jpg");

	END[0].Load("Resource/storyimage/story_2_0.jpg");
	END[1].Load("Resource/storyimage/story_2_1.jpg");
	END[2].Load("Resource/storyimage/story_2_2.jpg");
	END[3].Load("Resource/storyimage/story_2_3.jpg");
	END[4].Load("Resource/storyimage/story_2_4.jpg");
	END[5].Load("Resource/storyimage/story_2_5.jpg");
	END[6].Load("Resource/storyimage/story_2_6.jpg");
	END[7].Load("Resource/storyimage/story_2_7.jpg");
	END[8].Load("Resource/storyimage/story_2_8.jpg");
	END[9].Load("Resource/storyimage/story_2_9.jpg");
	END[10].Load("Resource/storyimage/story_2_10.jpg");
	END[11].Load("Resource/storyimage/story_2_11.jpg");
	END[12].Load("Resource/storyimage/story_2_12.jpg");
	END[13].Load("Resource/storyimage/story_2_13.jpg");
	END[14].Load("Resource/storyimage/story_2_14.jpg");
	END[15].Load("Resource/storyimage/story_2_15.jpg");
	END[16].Load("Resource/storyimage/story_2_16.jpg");
	END[17].Load("Resource/storyimage/story_2_17.jpg");
	END[18].Load("Resource/storyimage/story_2_18.jpg");
	END[19].Load("Resource/storyimage/story_2_19.jpg");
	END[20].Load("Resource/storyimage/story_2_20.jpg");
	END[21].Load("Resource/storyimage/story_2_21.jpg");
	END[22].Load("Resource/storyimage/story_2_22.jpg");
	END[23].Load("Resource/storyimage/story_2_23.jpg");
	END[24].Load("Resource/storyimage/story_2_24.jpg");
	END[25].Load("Resource/storyimage/story_2_25.jpg");
	END[26].Load("Resource/storyimage/story_2_26.jpg");
	END[27].Load("Resource/storyimage/story_2_27.jpg");
	END[28].Load("Resource/storyimage/story_2_28.jpg");
	END[29].Load("Resource/storyimage/story_2_29.jpg");
	END[30].Load("Resource/storyimage/story_2_30.jpg");
	END[31].Load("Resource/storyimage/story_2_31.jpg");
	END[32].Load("Resource/storyimage/story_2_32.jpg");
	END[33].Load("Resource/storyimage/story_2_33.jpg");
	END[34].Load("Resource/storyimage/story_2_34.jpg");
	END[35].Load("Resource/storyimage/story_2_35.jpg");
	END[36].Load("Resource/storyimage/story_2_36.jpg");
	END[37].Load("Resource/storyimage/story_2_37.jpg");
	END[38].Load("Resource/storyimage/story_2_38.jpg");
	END[39].Load("Resource/storyimage/story_2_39.jpg");
	END[40].Load("Resource/storyimage/story_2_40.jpg");
	END[41].Load("Resource/storyimage/story_2_41.jpg");
	END[42].Load("Resource/storyimage/story_2_42.jpg");
	END[43].Load("Resource/storyimage/story_2_43.jpg");
	END[44].Load("Resource/storyimage/story_2_44.jpg");
	END[45].Load("Resource/storyimage/story_2_45.jpg");
	END[46].Load("Resource/storyimage/story_2_46.jpg");
	END[47].Load("Resource/storyimage/story_2_47.jpg");
	END[48].Load("Resource/storyimage/story_2_48.jpg");
	END[49].Load("Resource/storyimage/story_2_49.jpg");
	END[50].Load("Resource/storyimage/story_2_50.jpg");
	END[51].Load("Resource/storyimage/story_2_51.jpg");
	END[52].Load("Resource/storyimage/story_2_52.jpg");
	END[53].Load("Resource/storyimage/story_2_53.jpg");
	END[54].Load("Resource/storyimage/story_2_54.jpg");
	END[55].Load("Resource/storyimage/story_2_55.jpg");


	FunShop[0].Load("Resource/storyimage/FunShop_1.jpg");
	FunShop[1].Load("Resource/storyimage/FunShop_2.jpg");
	FunShop[2].Load("Resource/storyimage/FunShop_3.jpg");

	FunFish[0].Load("Resource/storyimage/흑백낚시_1.jpg");
	FunFish[1].Load("Resource/storyimage/흑백낚시_2.jpg");
	FunFish[2].Load("Resource/storyimage/흑백낚시_3.jpg");
	FunFish[3].Load("Resource/storyimage/흑백낚시_4.jpg");
	FunFish[4].Load("Resource/storyimage/흑백낚시_5.jpg");
	FunFish[5].Load("Resource/storyimage/흑백낚시_6.jpg");
	FunFish[6].Load("Resource/storyimage/흑백낚시_7.jpg");
	FunFish[7].Load("Resource/storyimage/흑백낚시_8.jpg");
	FunFish[8].Load("Resource/storyimage/흑백낚시_9.jpg");
	FunFish[9].Load("Resource/storyimage/흑백낚시_10.jpg");
	FunFish[10].Load("Resource/storyimage/흑백낚시_11.jpg");
	FunFish[11].Load("Resource/storyimage/흑백낚시_12.jpg");
	FunFish[12].Load("Resource/storyimage/흑백낚시_13.jpg");
	FunFish[13].Load("Resource/storyimage/흑백낚시_14.jpg");
	FunFish[14].Load("Resource/storyimage/흑백낚시_15.jpg");
	FunFish[15].Load("Resource/storyimage/흑백낚시_16.jpg");
	FunFish[16].Load("Resource/storyimage/흑백낚시_17.jpg");
	FunFish[17].Load("Resource/storyimage/흑백낚시_18.jpg");

	parable[0].Load("Resource/storyimage/parable_1.jpg");
	parable[1].Load("Resource/storyimage/parable_0.jpg");
	parable[2].Load("Resource/storyimage/parable_2.jpg");
	parable[3].Load("Resource/storyimage/parable_0.jpg");
	parable[4].Load("Resource/storyimage/parable_3.jpg");
	parable[5].Load("Resource/storyimage/parable_0.jpg");
	parable[6].Load("Resource/storyimage/parable_4.jpg");
	parable[7].Load("Resource/storyimage/parable_0.jpg");
	parable[8].Load("Resource/storyimage/parable_5.jpg");
	parable[9].Load("Resource/storyimage/parable_0.jpg");
	parable[10].Load("Resource/storyimage/parable_6.jpg");
	parable[11].Load("Resource/storyimage/parable_0.jpg");
	parable[12].Load("Resource/storyimage/parable_7.jpg");
	parable[13].Load("Resource/storyimage/parable_0.jpg");
}
*/

void CDestroy() {

	maze.Destroy();
	blind.Destroy();
	bk.Destroy();
	Finn.Destroy();
	talkMask.Destroy();
	fishing.Destroy();
	fishGage[0].Destroy();
	fishGage[1].Destroy();
	fishPoint[0].Destroy();
	fishPoint[1].Destroy();
	Dog.Destroy();
	Tree.Destroy();
	Timber.Destroy();
	TimberGage.Destroy();
	TimberPoint.Destroy();
	FeelChart.Destroy();
	HELLOWORLD.Destroy();
	fruits[0].Destroy();
	fruits[1].Destroy();
	fruits[2].Destroy();

	VillageChief[0].Destroy(); VillageChief[1].Destroy(); VillageChief[2].Destroy();
	VillageChief[3].Destroy(); VillageChief[4].Destroy(); VillageChief[5].Destroy();
	VillageChief[6].Destroy(); VillageChief[7].Destroy(); VillageChief[8].Destroy();

	ShopKeeper[0].Destroy(); ShopKeeper[1].Destroy(); ShopKeeper[2].Destroy();

	HouseWife[0].Destroy(); HouseWife[1].Destroy();
	HouseWife[2].Destroy(); HouseWife[3].Destroy();

	Kid[0].Destroy(); Kid[1].Destroy(); Kid[2].Destroy();

	Fisherman[0].Destroy(); Fisherman[1].Destroy(); Fisherman[2].Destroy();

	TImberman[0].Destroy(); TImberman[1].Destroy(); TImberman[2].Destroy();

	Soldier[0].Destroy(); Soldier[1].Destroy(); Soldier[2].Destroy(); Soldier[3].Destroy();

	People[0].Destroy(); People[1].Destroy(); People[2].Destroy(); People[3].Destroy();
	People[4].Destroy(); People[5].Destroy(); People[6].Destroy(); People[7].Destroy();
	People[8].Destroy(); People[9].Destroy(); People[10].Destroy(); People[11].Destroy();
	People[12].Destroy(); People[13].Destroy();


	Captin.Destroy();
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

	Qmark.Destroy();
	HPbar.Destroy();

	bird[0].Destroy();
	bird[1].Destroy();
	bird[2].Destroy();
	bird[3].Destroy();
	bird[4].Destroy();
	bird[5].Destroy();
	bird[6].Destroy();
	bird[7].Destroy();
	bird[8].Destroy();

	bat[0].Destroy();
	bat[1].Destroy();
	bat[2].Destroy();

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
	if (!Talk) {
		/*
		if (Map == 0) {
			dog.speed = 10;
			woman.speed = 10;
			Fruits.speed = 10;
			tree.speed = 10;
			if (main.actmod == 0) {
				if (KeyBuffer[VK_LEFT]) {
					tree.x -= (main.speed + tree.speed);
					Fruits.x -= (main.speed + Fruits.speed);
					dog.x -= (main.speed + dog.speed);
					woman.x -= (main.speed + woman.speed);
					main.walk = true;
					main.x -= main.speed;
					main.mod = 1;
					if ((main.x < 720 && main.y < 3050 && main.x>180 && main.y>890) || main.x < 200 || (main.x > 710 && main.y > 890 && main.x < 1000 && main.y < 980) || (main.x > 980 && main.y > 930 && main.x < 1120 && main.y < 1060)
						|| (main.x > 1100 && main.y > 890 && main.x < 1680 && main.y < 980) || (main.x > 1660 && main.y > 1170 && main.x < 2520 && main.y < 1260) || (main.x == 1670 && main.y < 1180 && main.y>950) || (main.x >= 1920 && main.x < 1960 && main.y == (main.x - 940 + 10))
						|| (main.x > 1860 && main.y > 1020 && main.x < 1960 && main.y < 1060) || (main.x > 2820 && main.y > 1450 && main.x < 2880 && main.y < 2070) || (main.x > 2870 && main.y > 1570 && main.x < 3430 && main.y < 2070)
						|| (main.x > 2700 && main.y > 1450 && main.x < 2840 && main.y < 1550) || (main.x > 2510 && main.y > 1450 && main.x < 2640 && main.y < 1550) || (main.x == 2510 && main.y > 1250 && main.y < 1460)
						|| (main.x > 3420 && main.y > 2070 && main.x < 3540 && main.y < 2170) || (main.x > 2870 && main.y > 2130 && main.x < 3480 && main.y < 2310) || (main.x > 2820 && main.y > 2130 && main.x < 2880 && main.y < 2700)
						|| (main.x > 2780 && main.y > 2620 && main.x < 2890 && main.y < 2940) || (main.x > 2180 && main.y > 2620 && main.x < 2720 && main.y < 2890) || (main.x > 2710 && main.y > 2850 && main.x < 2810 && main.y < 2980)
						|| (main.x < 2190 && main.y>2540 && main.x > 2160 && main.y < 2990) || (main.x > 2120 && main.y > 2540 && main.x < 2190 && main.y < 2580) || (main.x < 2070 && main.y < 2580 && main.x>2000 && main.y>2540)
						|| (main.x < 2190 && main.y < 3020 && main.x>1720 && main.y>2980) || (main.x < 1960 && main.y < 2990 && main.x>1740 && main.y>2820) || (main.x < 1960 && main.y < 2790 && main.x>1740 && main.y>2620) || (main.x > 1740 && main.y > 2420 && main.x < 1960 && main.y < 2590)
						|| (main.x < 1750 && main.y < 3020 && main.x>1720 && main.y>2380) || (main.x < 1720 && main.y < 3030 && main.x>1660 && main.y>2130) || (main.x < 2030 && main.y < 2550 && main.x>2000 && main.y>2380)
						|| (main.x > 1700 && main.y > 1370 && main.x < 1990 && main.y < 1700) || (main.x < 2510 && main.y < 1380 && main.x>1660 && main.y>1300) || (main.x > 1660 && main.y > 1300 && main.x < 1720 && main.y < 2070)
						|| (main.x > 1540 && main.y > 2130 && main.x < 1680 && main.y < 2180) || (main.x > 1380 && main.y > 2130 && main.x < 1520 && main.y < 2180) || (main.x < 1710 && main.y < 2070 && main.x>1380 && main.y>1980)
						|| (main.x > 2060 && main.y > 1850 && main.x < 2200 && main.y < 2270) || (main.x > 2190 && main.y > 1850 && main.x < 2360 && main.y < 2020) || (main.x > 2340 && main.y > 1850 && main.x < 2480 && main.y < 2270)
						|| (main.x > 2310 && main.y > 2000 && main.x < 2350 && main.y < 2060) || (main.x > 2180 && main.y > 2000 && main.x < 2240 && main.y < 2060) || (main.x > 740 && main.y > 1060 && main.x < 840 && main.y < 1100)
						|| (main.x > 910 && main.y > 1230 && main.x < 950 && main.y < 1250) || (main.x > 1420 && main.y > 1100 && main.x < 1480 && main.y < 1180) || (main.x > 1140 && main.y > 1260 && main.x < 1200 && main.y < 1340)
						|| (main.x > 1420 && main.y > 1300 && main.x < 1480 && main.y < 1380) || (main.x > 820 && main.y > 1740 && main.x < 880 && main.y < 1820) || (main.x > 1020 && main.y > 2100 && main.x < 1080 && main.y < 2180)
						|| (main.x > 1340 && main.y > 1620 && main.x < 1400 && main.y < 1700) || (main.x > 820 && main.y > 2420 && main.x < 880 && main.y < 2500) || (main.x > 940 && main.y > 2540 && main.x < 1000 && main.y < 2620) || (main.x > 1340 && main.y > 2380 && main.x < 1400 && main.y < 2460)
						|| (main.x > 820 && main.y > 2740 && main.x < 880 && main.y < 2820) || (main.x > 2990 && main.y > 2100 && main.x < 3040 && main.y < 2140) || (main.x > 3190 && main.y > 2100 && main.x < 3240 && main.y < 2140)
						|| (main.x > 3350 && main.y > 2100 && main.x < 3400 && main.y < 2140) || (main.x > 3380 && main.y > 2060 && main.x < 3430 && main.y < 2100) || (main.x > 2790 && main.y > 2130 && main.x < 2830 && main.y < 2180) || (main.x > 2790 && main.y > 2020 && main.x < 2830 && main.y < 2070)
						|| (main.x > 2720 && main.y > 2820 && main.x < 2760 && main.y < 2860) || (main.x > 2100 && main.y > 2700 && main.x < 2160 && main.y < 2820) || (main.x > 1950 && main.y > 2740 && main.x < 2000 && main.y < 2790)
						|| (main.x > 1950 && main.y > 2940 && main.x < 2000 && main.y < 2990) || (main.x > 2060 && main.y > 2900 && main.x < 2120 && main.y < 2980) || (main.x > 2100 && main.y > 2940 && main.x < 2160 && main.y < 2980) || (main.x > 1710 && main.y < 2070 && main.x < 1750 && main.y>2020) || (main.x > 1710 && main.y > 2130 && main.x < 1750 && main.y < 2180)
						|| (main.x > 1990 && main.y > 1420 && main.x < 2040 && main.y < 1460) || (main.x > 2600 && main.y > 1540 && main.x < 2630 && main.y < 1580) || (main.x > 2750 && main.y > 1300 && main.x < 2840 && main.y < 1380) || (main.x > 2150 && main.y > 1010 && main.x < 2240 && main.y < 1060)
						|| (main.x > 2510 && main.y > 1010 && main.x < 2600 && main.y < 1060) || (main.x > 1310 && main.y > 2030 && main.x < 1360 && main.y < 2060) || (main.x > 2470 && main.y > 1250 && main.x < 2510 && main.y < 1310) || (main.x > 1110 && main.y > 1090 && main.x < 1200 && main.y < 1140)
						|| (main.x > 750 && main.y > 1530 && main.x < 840 && main.y < 1580) || (main.x > 1200 && main.y > 1590 && main.x < 1230 && main.y < 1610) || (main.x > 1030 && main.y > 1780 && main.x < 1080 && main.y < 1820)
						|| (main.x > 1040 && main.y > 2310 && main.x < 1070 && main.y < 2330) || (main.x > 960 && main.y > 2870 && main.x < 990 && main.y < 2890) || (main.x > 1120 && main.y > 2790 && main.x < 1150 && main.y < 2810)
						|| (main.x > 1240 && main.y > 2750 && main.x < 1270 && main.y < 2770) || (main.x > 1360 && main.y > 2830 && main.x < 1390 && main.y < 2850) || (main.x > 1280 && main.y > 2870 && main.x < 1310 && main.y < 2890)
						|| (main.x > 1240 && main.y > 2990 && main.x < 1270 && main.y < 3110) || (main.x > 1160 && main.y > 2510 && main.x < 1190 && main.y < 2530) || (main.x > 1550 && main.y > 2740 && main.x < 1600 && main.y < 2860)
						|| (main.x > 1470 && main.y > 2830 && main.x < 1520 && main.y < 2860) || (main.x > 1430 && main.y > 2910 && main.x < 1470 && main.y < 2940) || (main.x > 870 && main.y > 2210 && main.x < 960 && main.y < 2260)
						|| (main.x > 990 && main.y > 2450 && main.x < 1080 && main.y < 2500) || (main.x > 790 && main.y > 2930 && main.x < 880 && main.y < 2980) || (main.x > 710 && main.y > 2610 && main.x < 800 && main.y < 2660)
						|| (main.x > 1040 && main.y > 2830 && main.x < 1090 && main.y < 2910) || (main.x > 2010 && main.y > 2740 && main.x < 2070 && main.y < 2790) || (main.x > 2160 && main.y > 2330 && main.x < 2200 && main.y < 2370) || (main.x > 1980 && main.y > 1450 && main.x < 2000 && main.y < 1490)
						|| (main.x > 2400 && main.y > 1480 && main.x < 2430 && main.y < 1520) || (main.x > 3360 && main.y > 2060 && main.x < 3400 && main.y < 2100) || (main.x > 2020 && main.y > 2570 && main.x < 2070 && main.y < 2600) || (main.x > 990 && main.y > 1050 && main.x < 1040 && main.y < 1080)
						|| (main.x > 1390 && main.y > 2820 && main.x < 1440 && main.y < 2860)

						|| (main.x > 2100 && main.y > 1680 && main.x < 2160 && main.y < 1730) || (main.x > 2720 && main.y > 1690 && main.x < 2770 && main.y < 1740) || (main.x > 2670 && main.y > 1930 && main.x < 2710 && main.y < 1980)
						|| (main.x > 2970 && main.y > 2100 && main.x < 3000 && main.y < 2140) || (main.x > 2500 && main.y > 2490 && main.x < 2540 && main.y < 2530) || (main.x > 1790 && main.y > 2170 && main.x < 1840 && main.y < 2210)

						|| (main.x >= 800 && main.x <= 830 && main.y == (main.x + 220 + 10)) || (main.x < 2240 && main.x >= 2200 && main.y == (main.x - 2240 + 1020 + 10))) {
						main.x += main.speed;
						dog.x += (main.speed + dog.speed);
						woman.x += (main.speed + woman.speed);
						Fruits.x += (main.speed + Fruits.speed);
						tree.x += (main.speed + tree.speed);
					}

					if (main.x >= 1920 && main.x < 1970 && main.y == (main.x - 940) || (main.x >= 800 && main.x <= 830 && main.y == (main.x + 220))
						|| (main.x < 2240 && main.x >= 2200 && main.y == (main.x - 2240 + 1020)) || (main.x < 2600 && main.x >= 2560 && main.y == (main.x - 2600 + 1020))
						|| (main.x < 1200 && main.x >= 1160 && main.y == (main.x - 1200 + 1100)) || (main.x < 840 && main.x >= 800 && main.y == (main.x - 830 + 1540))
						|| (main.x < 970 && main.x >= 920 && main.y == (main.x - 950 + 2210)) || (main.x < 1080 && main.x >= 1040 && main.y == (main.x - 1080 + 2460))
						|| (main.x < 880 && main.x >= 840 && main.y == (main.x - 880 + 2940)) || (main.x < 800 && main.x >= 760 && main.y == (main.x - 800 + 2620))) {
						main.y -= main.speed;
						dog.y -= (main.speed + dog.speed);
						woman.y -= (main.speed + woman.speed);
						Fruits.y -= (main.speed + Fruits.speed);
						tree.y -= (main.speed + tree.speed);
					}
					if (timber.mod) {
						if (main.x > 1280 && main.y > 2820 && main.x < 1310 && main.y < 2890) {
							main.x += main.speed;
							dog.x += (main.speed + dog.speed);
							woman.x += (main.speed + woman.speed);
							Fruits.x += (main.speed + Fruits.speed);
							tree.x += (main.speed + tree.speed);
						}
					}
				}
				if (KeyBuffer[VK_RIGHT]) {
					tree.x += (main.speed + tree.speed);
					Fruits.x += (main.speed + Fruits.speed);
					dog.x += (main.speed + dog.speed);
					woman.x += (main.speed + woman.speed);
					main.walk = true;
					main.x += main.speed;
					main.mod = 3;
					if ((main.x < 720 && main.y < 3050 && main.x>180 && main.y>890) || (main.x > 3180 && main.y < 2000 && main.y>890) || (main.x > 980 && main.y > 930 && main.x < 1120 && main.y < 1060)
						|| (main.x > 1100 && main.y > 890 && main.x < 1680 && main.y < 980) || (main.x == 1670 && main.y < 1180 && main.y>950) || (main.x > 1660 && main.y > 1170 && main.x < 2520 && main.y < 1260)
						|| (main.x >= 1870 && main.x <= 1900 && main.y == (-main.x + 2880) + 10) || (main.x > 1860 && main.y > 1020 && main.x < 1960 && main.y < 1060)
						|| (main.x > 2820 && main.y > 1450 && main.x < 2880 && main.y < 2070) || (main.x > 2870 && main.y > 1570 && main.x < 3430 && main.y < 2070) || (main.x > 2700 && main.y > 1450 && main.x < 2840 && main.y < 1550)
						|| (main.x > 2510 && main.y > 1450 && main.x < 2640 && main.y < 1550) || (main.x == 2510 && main.y > 1250 && main.y < 1550 || (main.x > 2820 && main.y > 2130 && main.x < 2880 && main.y < 2700))
						|| (main.x > 3420 && main.y > 2050 && main.x < 3540 && main.y < 2170) || (main.x > 2870 && main.y > 2130 && main.x < 3480 && main.y < 2310) || (main.x > 2780 && main.y > 2620 && main.x < 2890 && main.y < 2940)
						|| (main.x > 2180 && main.y > 2620 && main.x < 2720 && main.y < 2890) || (main.x > 2710 && main.y > 2850 && main.x < 2810 && main.y < 2980) || (main.x < 2190 && main.y>2540 && main.x > 2160 && main.y < 2990)
						|| (main.x > 2120 && main.y > 2540 && main.x < 2190 && main.y < 2580) || (main.x < 2070 && main.y < 2580 && main.x>2000 && main.y>2540) || (main.x < 2190 && main.y < 3020 && main.x>1720 && main.y>2980)
						|| (main.x < 1960 && main.y < 2980 && main.x>1740 && main.y>2820) || (main.x < 1960 && main.y < 2790 && main.x>1740 && main.y>2620) || (main.x > 1740 && main.y > 2420 && main.x < 1960 && main.y < 2590)
						|| (main.x < 2030 && main.y < 2550 && main.x>2000 && main.y>2380) || (main.x > 1720 && main.y > 2380 && main.x < 2010 && main.y < 2430) || (main.x < 1750 && main.y < 3020 && main.x>1720 && main.y>2380) || (main.x < 1720 && main.y < 3030 && main.x>1660 && main.y>2130)
						|| (main.x > 1700 && main.y > 1370 && main.x < 1990 && main.y < 1700) || (main.x < 2510 && main.y < 1380 && main.x>1660 && main.y>1300) || (main.x > 1660 && main.y > 1300 && main.x < 1720 && main.y < 2070)
						|| (main.x > 1540 && main.y > 2130 && main.x < 1680 && main.y < 2180) || (main.x > 1380 && main.y > 2130 && main.x < 1520 && main.y < 2180) || (main.x < 1710 && main.y < 2070 && main.x>1380 && main.y>1980)
						|| (main.x > 2060 && main.y > 1850 && main.x < 2200 && main.y < 2270) || (main.x > 2190 && main.y > 1850 && main.x < 2360 && main.y < 2020) || (main.x > 2340 && main.y > 1850 && main.x < 2480 && main.y < 2270)
						|| (main.x > 2310 && main.y > 2000 && main.x < 2350 && main.y < 2060) || (main.x > 2180 && main.y > 2000 && main.x < 2240 && main.y < 2060) || (main.x > 740 && main.y > 1060 && main.x < 840 && main.y < 1100)
						|| (main.x > 910 && main.y > 1230 && main.x < 950 && main.y < 1250) || (main.x > 1420 && main.y > 1100 && main.x < 1480 && main.y < 1180) || (main.x > 1140 && main.y > 1260 && main.x < 1200 && main.y < 1340)
						|| (main.x > 1420 && main.y > 1300 && main.x < 1480 && main.y < 1380) || (main.x > 820 && main.y > 1740 && main.x < 880 && main.y < 1820) || (main.x > 1020 && main.y > 2100 && main.x < 1080 && main.y < 2180)
						|| (main.x > 1340 && main.y > 1620 && main.x < 1400 && main.y < 1700) || (main.x > 820 && main.y > 2420 && main.x < 880 && main.y < 2500) || (main.x > 940 && main.y > 2540 && main.x < 1000 && main.y < 2620) || (main.x > 1340 && main.y > 2380 && main.x < 1400 && main.y < 2460)
						|| (main.x > 820 && main.y > 2740 && main.x < 880 && main.y < 2820) || (main.x > 2990 && main.y > 2100 && main.x < 3040 && main.y < 2140) || (main.x > 3190 && main.y > 2100 && main.x < 3240 && main.y < 2140)
						|| (main.x > 3350 && main.y > 2100 && main.x < 3400 && main.y < 2140) || (main.x > 3380 && main.y > 2060 && main.x < 3430 && main.y < 2100) || (main.x > 2790 && main.y > 2130 && main.x < 2830 && main.y < 2180) || (main.x > 2790 && main.y > 2020 && main.x < 2830 && main.y < 2070)
						|| (main.x > 2720 && main.y > 2820 && main.x < 2760 && main.y < 2860) || (main.x > 2100 && main.y > 2700 && main.x < 2160 && main.y < 2820) || (main.x > 1950 && main.y > 2740 && main.x < 2000 && main.y < 2790)
						|| (main.x > 1950 && main.y > 2940 && main.x < 2000 && main.y < 2990) || (main.x > 2060 && main.y > 2900 && main.x < 2120 && main.y < 2980) || (main.x > 2100 && main.y > 2940 && main.x < 2160 && main.y < 2980) || (main.x > 1710 && main.y < 2070 && main.x < 1750 && main.y>2020) || (main.x > 1710 && main.y > 2130 && main.x < 1750 && main.y < 2180)
						|| (main.x > 1990 && main.y > 1420 && main.x < 2040 && main.y < 1460) || (main.x > 2600 && main.y > 1540 && main.x < 2630 && main.y < 1580) || (main.x > 2750 && main.y > 1300 && main.x < 2840 && main.y < 1380) || (main.x > 2150 && main.y > 1010 && main.x < 2240 && main.y < 1060)
						|| (main.x > 2510 && main.y > 1010 && main.x < 2600 && main.y < 1060) || (main.x > 1310 && main.y > 2030 && main.x < 1360 && main.y < 2060) || (main.x > 2470 && main.y > 1250 && main.x < 2510 && main.y < 1310) || (main.x > 1110 && main.y > 1090 && main.x < 1200 && main.y < 1140)
						|| (main.x > 750 && main.y > 1530 && main.x < 840 && main.y < 1580) || (main.x > 1200 && main.y > 1590 && main.x < 1230 && main.y < 1610) || (main.x > 1030 && main.y > 1780 && main.x < 1080 && main.y < 1820)
						|| (main.x > 1040 && main.y > 2310 && main.x < 1070 && main.y < 2330) || (main.x > 960 && main.y > 2870 && main.x < 990 && main.y < 2890) || (main.x > 1120 && main.y > 2790 && main.x < 1150 && main.y < 2810)
						|| (main.x > 1240 && main.y > 2750 && main.x < 1270 && main.y < 2770) || (main.x > 1360 && main.y > 2830 && main.x < 1390 && main.y < 2850) || (main.x > 1280 && main.y > 2870 && main.x < 1310 && main.y < 2890)
						|| (main.x > 1240 && main.y > 2990 && main.x < 1270 && main.y < 3110) || (main.x > 1160 && main.y > 2510 && main.x < 1190 && main.y < 2530) || (main.x > 1550 && main.y > 2740 && main.x < 1600 && main.y < 2860)
						|| (main.x > 1470 && main.y > 2830 && main.x < 1520 && main.y < 2860) || (main.x > 1430 && main.y > 2910 && main.x < 1470 && main.y < 2940) || (main.x > 870 && main.y > 2210 && main.x < 960 && main.y < 2260)
						|| (main.x > 990 && main.y > 2450 && main.x < 1080 && main.y < 2500) || (main.x > 790 && main.y > 2930 && main.x < 880 && main.y < 2980) || (main.x > 710 && main.y > 2610 && main.x < 800 && main.y < 2660)
						|| (main.x > 3220 && main.y > 850 && main.y < 930) || (main.x > 1040 && main.y > 2830 && main.x < 1090 && main.y < 2910) || (main.x > 2190 && main.y > 3000 && main.y < 3030) || (main.x > 2010 && main.y > 2740 && main.x < 2070 && main.y < 2790)
						|| (main.x > 2160 && main.y > 2330 && main.x < 2200 && main.y < 2370) || (main.x > 1980 && main.y > 1450 && main.x < 2000 && main.y < 1490) || (main.x > 2400 && main.y > 1480 && main.x < 2430 && main.y < 1520) || (main.x > 3360 && main.y > 2060 && main.x < 3400 && main.y < 2100)
						|| (main.x > 2020 && main.y > 2570 && main.x < 2070 && main.y < 2600) || (main.x > 990 && main.y > 1050 && main.x < 1040 && main.y < 1080) || (main.x > 1390 && main.y > 2820 && main.x < 1440 && main.y < 2860)

						|| (main.x > 2100 && main.y > 1680 && main.x < 2160 && main.y < 1730) || (main.x > 2720 && main.y > 1690 && main.x < 2770 && main.y < 1740) || (main.x > 2670 && main.y > 1930 && main.x < 2710 && main.y < 1980)
						|| (main.x > 2970 && main.y > 2100 && main.x < 3000 && main.y < 2140) || (main.x > 2500 && main.y > 2490 && main.x < 2540 && main.y < 2530) || (main.x > 1790 && main.y > 2170 && main.x < 1840 && main.y < 2210)

						|| (main.x >= 750 && main.x <= 790 && main.y == (-main.x + 750 + 1060 + 10)) || (main.x <= 2190 && main.x > 2150 && main.y == (-main.x + 2150 + 1020 + 10))) {
						main.x -= main.speed;
						dog.x -= (main.speed + dog.speed);
						woman.x -= (main.speed + woman.speed);
						Fruits.x -= (main.speed + Fruits.speed);
						tree.x -= (main.speed + tree.speed);

					}
					if ((main.x >= 1860 && main.x <= 1900 && main.y == (-main.x + 2880)) || (main.x >= 750 && main.x <= 790 && main.y == (-main.x + 750 + 1060))
						|| (main.x <= 2190 && main.x > 2150 && main.y == (-main.x + 2150 + 1020)) || (main.x <= 2550 && main.x > 2510 && main.y == (-main.x + 2510 + 1020))
						|| (main.x <= 1150 && main.x > 1110 && main.y == (-main.x + 1110 + 1100)) || (main.x <= 790 && main.x > 750 && main.y == (-main.x + 750 + 1540 + 10))
						|| (main.x <= 910 && main.x > 870 && main.y == (-main.x + 870 + 2220)) || (main.x <= 1030 && main.x > 990 && main.y == (-main.x + 990 + 2460))
						|| (main.x <= 830 && main.x > 790 && main.y == (-main.x + 790 + 2940)) || (main.x <= 750 && main.x > 710 && main.y == (-main.x + 710 + 2620))) {
						main.y -= main.speed;
						dog.y -= (main.speed + dog.speed);
						woman.y -= (main.speed + woman.speed);
						Fruits.y -= (main.speed + Fruits.speed);
						tree.y -= (main.speed + tree.speed);
					}
					if (timber.mod) {
						if (main.x > 1280 && main.y > 2820 && main.x < 1310 && main.y < 2890) {
							main.x -= main.speed;
							dog.x -= (main.speed + dog.speed);
							woman.x -= (main.speed + woman.speed);
							Fruits.x -= (main.speed + Fruits.speed);
							tree.x -= (main.speed + tree.speed);
						}
					}
				}
				if (KeyBuffer[VK_UP]) {

					main.walk = true;
					main.y -= main.speed;
					if (main.x == 1050 && main.y == 1050 && Map == 0) {
						main.x = 1960;
						main.y = 3580;
						/////////////////초기화
						main.count = 0;
						main.mod = 2;// 위쪽바라보는 모습
						main.speed = 10;//캐릭터 속도
						Map = 1;
						pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[6], false, &pChannel[0]);
					}
					else {
						tree.y -= (main.speed + tree.speed);
						dog.y -= (main.speed + dog.speed);
						woman.y -= (main.speed + woman.speed);
						Fruits.y -= (main.speed + Fruits.speed);
						main.mod = 2;
						if ((main.x < 720 && main.y < 3050 && main.x>180 && main.y>890) || main.y < 860 || (main.x > 710 && main.y > 890 && main.x < 1000 && main.y < 980) || (main.x > 980 && main.y > 930 && main.x < 1120 && main.y < 1060)
							|| (main.x > 1100 && main.y > 890 && main.x < 1680 && main.y < 980) || (main.x > 1660 && main.y > 1170 && main.x < 2520 && main.y < 1260) || (main.x > 1860 && main.y > 1020 && main.x < 1960 && main.y < 1060)
							|| (main.x > 2820 && main.y > 1450 && main.x < 2880 && main.y < 2070) || (main.x > 2870 && main.y > 1570 && main.x < 3430 && main.y < 2070)
							|| (main.x > 2700 && main.y > 1450 && main.x < 2840 && main.y < 1550) || (main.x > 2510 && main.y > 1450 && main.x < 2640 && main.y < 1550)
							|| (main.x > 3420 && main.y > 2070 && main.x < 3540 && main.y < 2170) || (main.x > 2870 && main.y > 2130 && main.x < 3480 && main.y < 2310)
							|| (main.x > 2820 && main.y > 2130 && main.x < 2880 && main.y < 2700) || (main.x > 2780 && main.y > 2620 && main.x < 2890 && main.y < 2940) || (main.x > 2180 && main.y > 2620 && main.x < 2720 && main.y < 2890)
							|| (main.x > 2710 && main.y > 2850 && main.x < 2810 && main.y < 2980) || (main.x < 2190 && main.y>2540 && main.x > 2160 && main.y < 2990) || (main.x > 2120 && main.y > 2540 && main.x < 2190 && main.y < 2580)
							|| (main.x < 2070 && main.y < 2580 && main.x>2000 && main.y>2540) || (main.x < 2190 && main.y < 3020 && main.x>1720 && main.y>2980) || (main.x < 1960 && main.y < 2980 && main.x>1740 && main.y>2820)
							|| (main.x < 1960 && main.y < 2790 && main.x>1740 && main.y>2620) || (main.x > 1740 && main.y > 2420 && main.x < 1960 && main.y < 2590) || (main.x < 2030 && main.y < 2550 && main.x>2000 && main.y>2380)
							|| (main.x > 1720 && main.y > 2380 && main.x < 2010 && main.y < 2430) || (main.x < 1750 && main.y < 3020 && main.x>1720 && main.y>2380) || (main.x < 1720 && main.y < 3030 && main.x>1660 && main.y>2130)
							|| (main.x > 1700 && main.y > 1370 && main.x < 1990 && main.y < 1700) || (main.x < 2510 && main.y < 1380 && main.x>1660 && main.y>1300) || (main.x > 1660 && main.y > 1300 && main.x < 1720 && main.y < 2070)
							|| (main.x > 1540 && main.y > 2130 && main.x < 1680 && main.y < 2180) || (main.x > 1380 && main.y > 2130 && main.x < 1520 && main.y < 2180) || (main.x < 1710 && main.y < 2070 && main.x>1380 && main.y>1980)
							|| (main.x > 2060 && main.y > 1850 && main.x < 2200 && main.y < 2270) || (main.x > 2190 && main.y > 1850 && main.x < 2360 && main.y < 2020) || (main.x > 2340 && main.y > 1850 && main.x < 2480 && main.y < 2270)
							|| (main.x > 2310 && main.y > 2000 && main.x < 2350 && main.y < 2060) || (main.x > 2180 && main.y > 2000 && main.x < 2240 && main.y < 2060) || (main.x > 740 && main.y > 1070 && main.x < 840 && main.y < 1100)
							|| (main.x > 910 && main.y > 1230 && main.x < 950 && main.y < 1250) || (main.x > 1420 && main.y > 1100 && main.x < 1480 && main.y < 1180) || (main.x > 1140 && main.y > 1260 && main.x < 1200 && main.y < 1340)
							|| (main.x > 1420 && main.y > 1300 && main.x < 1480 && main.y < 1380) || (main.x > 820 && main.y > 1740 && main.x < 880 && main.y < 1820) || (main.x > 1020 && main.y > 2100 && main.x < 1080 && main.y < 2180)
							|| (main.x > 1340 && main.y > 1620 && main.x < 1400 && main.y < 1700) || (main.x > 820 && main.y > 2420 && main.x < 880 && main.y < 2500) || (main.x > 940 && main.y > 2540 && main.x < 1000 && main.y < 2620) || (main.x > 1340 && main.y > 2380 && main.x < 1400 && main.y < 2460)
							|| (main.x > 820 && main.y > 2740 && main.x < 880 && main.y < 2820) || (main.x > 3380 && main.y > 2060 && main.x < 3430 && main.y < 2100) || (main.x > 2790 && main.y > 2130 && main.x < 2830 && main.y < 2180) || (main.x > 2790 && main.y > 2020 && main.x < 2830 && main.y < 2070)
							|| (main.x > 2720 && main.y > 2820 && main.x < 2760 && main.y < 2860) || (main.x > 2100 && main.y > 2700 && main.x < 2160 && main.y < 2820) || (main.x > 1950 && main.y > 2740 && main.x < 2000 && main.y < 2790)
							|| (main.x > 1950 && main.y > 2940 && main.x < 2000 && main.y < 2990) || (main.x > 2060 && main.y > 2900 && main.x < 2120 && main.y < 2980) || (main.x > 2100 && main.y > 2940 && main.x < 2160 && main.y < 2980) || (main.x > 1710 && main.y < 2070 && main.x < 1750 && main.y>2020)
							|| (main.x > 1710 && main.y > 2130 && main.x < 1750 && main.y < 2180) || (main.x > 1990 && main.y > 1420 && main.x < 2040 && main.y < 1460) || (main.x > 2600 && main.y > 1540 && main.x < 2630 && main.y < 1580) || (main.x > 2750 && main.y > 1300 && main.x < 2840 && main.y < 1380)
							|| (main.x > 2150 && main.y > 1010 && main.x < 2240 && main.y < 1060) || (main.x > 2510 && main.y > 1010 && main.x < 2600 && main.y < 1060) || (main.x > 1310 && main.y > 2030 && main.x < 1360 && main.y < 2060) || (main.x > 2470 && main.y > 1250 && main.x < 2510 && main.y < 1310)
							|| (main.x > 1110 && main.y > 1090 && main.x < 1200 && main.y < 1140) || (main.x > 750 && main.y > 1530 && main.x < 840 && main.y < 1580) || (main.x > 1200 && main.y > 1590 && main.x < 1230 && main.y < 1610) || (main.x > 1030 && main.y > 1780 && main.x < 1080 && main.y < 1820)
							|| (main.x > 1040 && main.y > 2310 && main.x < 1070 && main.y < 2330) || (main.x > 960 && main.y > 2870 && main.x < 990 && main.y < 2890) || (main.x > 1120 && main.y > 2790 && main.x < 1150 && main.y < 2810)
							|| (main.x > 1240 && main.y > 2750 && main.x < 1270 && main.y < 2770) || (main.x > 1360 && main.y > 2830 && main.x < 1390 && main.y < 2850) || (main.x > 1280 && main.y > 2870 && main.x < 1310 && main.y < 2890)
							|| (main.x > 1240 && main.y > 2990 && main.x < 1270 && main.y < 3110) || (main.x > 1160 && main.y > 2510 && main.x < 1190 && main.y < 2530) || (main.x > 1550 && main.y > 2740 && main.x < 1600 && main.y < 2860)
							|| (main.x > 1470 && main.y > 2830 && main.x < 1520 && main.y < 2860) || (main.x > 1430 && main.y > 2910 && main.x < 1470 && main.y < 2940) || (main.x > 870 && main.y > 2210 && main.x < 960 && main.y < 2260)
							|| (main.x > 990 && main.y > 2450 && main.x < 1080 && main.y < 2500) || (main.x > 790 && main.y > 2930 && main.x < 880 && main.y < 2980) || (main.x > 710 && main.y > 2610 && main.x < 800 && main.y < 2660)
							|| (main.x > 1040 && main.y > 2830 && main.x < 1090 && main.y < 2910) || (main.x > 2010 && main.y > 2740 && main.x < 2070 && main.y < 2790) || (main.x > 2160 && main.y > 2330 && main.x < 2200 && main.y < 2370) || (main.x > 1980 && main.y > 1450 && main.x < 2000 && main.y < 1490)
							|| (main.x > 2400 && main.y > 1480 && main.x < 2430 && main.y < 1520) || (main.x > 3360 && main.y > 2060 && main.x < 3400 && main.y < 2100) || (main.x > 2020 && main.y > 2570 && main.x < 2070 && main.y < 2600) || (main.x > 990 && main.y > 1050 && main.x < 1040 && main.y < 1080)
							|| (main.x > 1390 && main.y > 2820 && main.x < 1440 && main.y < 2860)

							|| (main.x > 2100 && main.y > 1680 && main.x < 2160 && main.y < 1730) || (main.x > 2720 && main.y > 1690 && main.x < 2770 && main.y < 1740) || (main.x > 2670 && main.y > 1930 && main.x < 2710 && main.y < 1980)
							|| (main.x > 2970 && main.y > 2100 && main.x < 3000 && main.y < 2140) || (main.x > 2500 && main.y > 2490 && main.x < 2540 && main.y < 2530) || (main.x > 1790 && main.y > 2170 && main.x < 1840 && main.y < 2210)
							) {
							main.y += main.speed;
							dog.y += (main.speed + dog.speed);
							woman.y += (main.speed + woman.speed);
							Fruits.y += (main.speed + Fruits.speed);
							tree.y += (main.speed + tree.speed);
						}
						if (timber.mod) {
							if (main.x > 1280 && main.y > 2820 && main.x < 1310 && main.y < 2890) {
								main.y += main.speed;
								dog.y += (main.speed + dog.speed);
								woman.y += (main.speed + woman.speed);
								Fruits.y += (main.speed + Fruits.speed);
								tree.y += (main.speed + tree.speed);
							}
						}
					}
				}
				if (KeyBuffer[VK_DOWN]) {
					if (main.x > 2710 && main.y > 2790 && main.x < 2790) {
						captin.mod = 1;
						Talk = true;
						captin.button = true;
					}
					tree.y += (main.speed + tree.speed);
					dog.y += (main.speed + dog.speed);
					woman.y += (main.speed + woman.speed);
					Fruits.y += (main.speed + Fruits.speed);
					main.walk = true;
					main.y += main.speed;
					main.mod = 0;
					if ((main.x < 720 && main.y < 3050 && main.x>180 && main.y>890) || main.y > 3020 || (main.x > 710 && main.y > 890 && main.x < 1000 && main.y < 980) || (main.x > 980 && main.y > 930 && main.x < 1120 && main.y < 1060)
						|| (main.x > 1100 && main.y > 890 && main.x < 1680 && main.y < 980) || (main.x > 1660 && main.y > 1170 && main.x < 2520 && main.y < 1260) || (main.x >= 1870 && main.x <= 1910 && main.y == (-main.x + 2880) + 10)
						|| (main.x >= 1920 && main.x < 1960 && main.y == (main.x - 940 + 10)) || (main.x == 1910 && main.y == 980) || (main.x > 1860 && main.y > 1020 && main.x < 1960 && main.y < 1060) || (main.x > 2870 && main.y > 1570 && main.x < 3430 && main.y < 2070)
						|| (main.x > 2820 && main.y > 1450 && main.x < 2880 && main.y < 2070) || (main.x > 2700 && main.y > 1450 && main.x < 2840 && main.y < 1550) || (main.x > 2510 && main.y > 1450 && main.x < 2640 && main.y < 1550)
						|| (main.x > 3420 && main.y > 2070 && main.x < 3540 && main.y < 2170) || (main.x > 2870 && main.y > 2130 && main.x < 3480 && main.y < 2310) || (main.x > 2820 && main.y > 2130 && main.x < 2880 && main.y < 2700)
						|| (main.x > 2780 && main.y > 2620 && main.x < 2890 && main.y < 2940) || (main.x > 2180 && main.y > 2620 && main.x < 2720 && main.y < 2890) || (main.x > 2710 && main.y > 2850 && main.x < 2810 && main.y < 2980)
						|| (main.x < 2190 && main.y>2540 && main.x > 2160 && main.y < 2990) || (main.x > 2120 && main.y > 2540 && main.x < 2190 && main.y < 2580) || (main.x < 2070 && main.y < 2580 && main.x>2020 && main.y>2540)
						|| (main.x < 2190 && main.y < 3020 && main.x>1720 && main.y>2980) || (main.x < 1960 && main.y < 2980 && main.x>1740 && main.y>2820) || (main.x < 1960 && main.y < 2790 && main.x>1740 && main.y>2620)
						|| (main.x > 1740 && main.y > 2420 && main.x < 1960 && main.y < 2590) || (main.x < 2030 && main.y < 2550 && main.x>2000 && main.y>2380) || (main.x > 1720 && main.y > 2380 && main.x < 2010 && main.y < 2430)
						|| (main.x < 1750 && main.y < 3020 && main.x>1720 && main.y>2380) || (main.x < 1720 && main.y < 3030 && main.x>1660 && main.y>2130) || (main.x > 1700 && main.y > 1370 && main.x < 1990 && main.y < 1700)
						|| (main.x < 2510 && main.y < 1380 && main.x>1660 && main.y>1300) || (main.x > 1660 && main.y > 1300 && main.x < 1720 && main.y < 2070) || (main.x > 1540 && main.y > 2130 && main.x < 1680 && main.y < 2180)
						|| (main.x > 1380 && main.y > 2130 && main.x < 1520 && main.y < 2180) || (main.x < 1710 && main.y < 2070 && main.x>1380 && main.y>1980) || (main.x > 2060 && main.y > 1850 && main.x < 2200 && main.y < 2270)
						|| (main.x > 2190 && main.y > 1850 && main.x < 2360 && main.y < 2000) || (main.x > 2340 && main.y > 1850 && main.x < 2480 && main.y < 2270) || (main.x > 2310 && main.y > 2000 && main.x < 2350 && main.y < 2060)
						|| (main.x > 2180 && main.y > 2000 && main.x < 2240 && main.y < 2060) || (main.x > 740 && main.y > 1070 && main.x < 840 && main.y < 1100) || (main.x > 910 && main.y > 1230 && main.x < 950 && main.y < 1250)
						|| (main.x > 1420 && main.y > 1100 && main.x < 1480 && main.y < 1180) || (main.x > 1140 && main.y > 1260 && main.x < 1200 && main.y < 1340) || (main.x > 1420 && main.y > 1300 && main.x < 1480 && main.y < 1380)
						|| (main.x > 820 && main.y > 1740 && main.x < 880 && main.y < 1820) || (main.x > 1020 && main.y > 2100 && main.x < 1080 && main.y < 2180) || (main.x > 1340 && main.y > 1620 && main.x < 1400 && main.y < 1700)
						|| (main.x > 820 && main.y > 2420 && main.x < 880 && main.y < 2500) || (main.x > 940 && main.y > 2540 && main.x < 1000 && main.y < 2620) || (main.x > 1340 && main.y > 2380 && main.x < 1400 && main.y < 2460)
						|| (main.x > 820 && main.y > 2740 && main.x < 880 && main.y < 2820) || (main.x > 2990 && main.y > 2100 && main.x < 3040 && main.y < 2140) || (main.x > 3190 && main.y > 2100 && main.x < 3240 && main.y < 2140)
						|| (main.x > 3350 && main.y > 2100 && main.x < 3400 && main.y < 2140) || (main.x > 2790 && main.y > 2130 && main.x < 2830 && main.y < 2180) || (main.x > 2790 && main.y > 2020 && main.x < 2830 && main.y < 2070)
						|| (main.x > 2720 && main.y > 2820 && main.x < 2760 && main.y < 2860) || (main.x > 2100 && main.y > 2700 && main.x < 2160 && main.y < 2820) || (main.x > 1950 && main.y > 2740 && main.x < 2000 && main.y < 2790)
						|| (main.x > 1950 && main.y > 2940 && main.x < 2000 && main.y < 2990) || (main.x > 2060 && main.y > 2900 && main.x < 2120 && main.y < 2980) || (main.x > 2100 && main.y > 2940 && main.x < 2160 && main.y < 2980)
						|| (main.x > 1710 && main.y < 2070 && main.x < 1750 && main.y>2020) || (main.x > 1710 && main.y > 2130 && main.x < 1750 && main.y < 2180) || (main.x > 1990 && main.y > 1420 && main.x < 2040 && main.y < 1460) || (main.x > 2600 && main.y > 1540 && main.x < 2630 && main.y < 1580)
						|| (main.x > 2750 && main.y > 1300 && main.x < 2840 && main.y < 1380) || (main.x > 2150 && main.y > 1010 && main.x < 2240 && main.y < 1060) || (main.x > 2510 && main.y > 1010 && main.x < 2600 && main.y < 1060)
						|| (main.x > 1310 && main.y > 2030 && main.x < 1360 && main.y < 2060) || (main.x > 2470 && main.y > 1250 && main.x < 2510 && main.y < 1310) || (main.x > 1110 && main.y > 1090 && main.x < 1200 && main.y < 1140)
						|| (main.x > 750 && main.y > 1530 && main.x < 840 && main.y < 1580) || (main.x > 1200 && main.y > 1590 && main.x < 1230 && main.y < 1610) || (main.x > 1030 && main.y > 1780 && main.x < 1080 && main.y < 1820)
						|| (main.x > 1040 && main.y > 2310 && main.x < 1070 && main.y < 2330) || (main.x > 960 && main.y > 2870 && main.x < 990 && main.y < 2890) || (main.x > 1120 && main.y > 2790 && main.x < 1150 && main.y < 2810)
						|| (main.x > 1240 && main.y > 2750 && main.x < 1270 && main.y < 2770) || (main.x > 1360 && main.y > 2830 && main.x < 1390 && main.y < 2850) || (main.x > 1280 && main.y > 2870 && main.x < 1310 && main.y < 2890)
						|| (main.x > 1240 && main.y > 2990 && main.x < 1270 && main.y < 3110) || (main.x > 1160 && main.y > 2510 && main.x < 1190 && main.y < 2530) || (main.x > 1550 && main.y > 2740 && main.x < 1600 && main.y < 2860)
						|| (main.x > 1470 && main.y > 2830 && main.x < 1520 && main.y < 2860) || (main.x > 1430 && main.y > 2910 && main.x < 1470 && main.y < 2940) || (main.x > 870 && main.y > 2210 && main.x < 960 && main.y < 2260)
						|| (main.x > 990 && main.y > 2450 && main.x < 1080 && main.y < 2500) || (main.x > 790 && main.y > 2930 && main.x < 880 && main.y < 2980) || (main.x > 710 && main.y > 2610 && main.x < 800 && main.y < 2660)
						|| (main.x > 3180 && main.x < 3260 && main.y == 900) || (main.x > 1040 && main.y > 2830 && main.x < 1090 && main.y < 2910) || (main.x > 2010 && main.y > 2740 && main.x < 2070 && main.y < 2790)
						|| (main.x > 2160 && main.y > 2330 && main.x < 2200 && main.y < 2370) || (main.x > 1980 && main.y > 1450 && main.x < 2000 && main.y < 1490) || (main.x > 2400 && main.y > 1480 && main.x < 2430 && main.y < 1520) || (main.x > 3360 && main.y > 2060 && main.x < 3400 && main.y < 2100)
						|| (main.x > 2020 && main.y > 2570 && main.x < 2070 && main.y < 2600) || (main.x > 990 && main.y > 1050 && main.x < 1040 && main.y < 1080) || (main.x > 1390 && main.y > 2820 && main.x < 1440 && main.y < 2860)

						|| (main.x > 2100 && main.y > 1680 && main.x < 2160 && main.y < 1730) || (main.x > 2720 && main.y > 1690 && main.x < 2770 && main.y < 1740) || (main.x > 2670 && main.y > 1930 && main.x < 2710 && main.y < 1980)
						|| (main.x > 2970 && main.y > 2100 && main.x < 3000 && main.y < 2140) || (main.x > 2500 && main.y > 2490 && main.x < 2540 && main.y < 2530) || (main.x > 1790 && main.y > 2170 && main.x < 1840 && main.y < 2210)

						|| (main.x >= 750 && main.x <= 790 && main.y == (-main.x + 750 + 1060 + 10)) || (main.x >= 800 && main.x <= 830 && main.y == (main.x + 220 + 10))
						|| (main.x >= 1120 && main.x <= 1150 && main.y == (-main.x + 1110 + 1100 + 10)) || (main.x >= 1160 && main.x <= 1200 && main.y == (main.x + 1160 - 1050 + 10))
						|| (main.x <= 2190 && main.x > 2150 && main.y == (-main.x + 2150 + 1020 + 10)) || (main.x < 2240 && main.x >= 2200 && main.y == (main.x - 2240 + 1020 + 10))
						|| (main.x <= 2550 && main.x > 2510 && main.y == (-main.x + 2510 + 1020 + 10)) || (main.x < 2600 && main.x >= 2560 && main.y == (main.x - 2600 + 1020 + 10))
						|| (main.x <= 1150 && main.x > 1110 && main.y == (-main.x + 1110 + 1100 + 10)) || (main.x < 1200 && main.x >= 1160 && main.y == (main.x - 1200 + 1100 + 10))
						|| (main.x <= 790 && main.x > 750 && main.y == (-main.x + 750 + 1540 + 10)) || (main.x < 830 && main.x >= 800 && main.y == (main.x - 830 + 1540))
						|| (main.x <= 910 && main.x > 870 && main.y == (-main.x + 870 + 2220 + 10)) || (main.x < 950 && main.x >= 920 && main.y == (main.x - 950 + 2210 + 10))
						|| (main.x <= 1030 && main.x > 990 && main.y == (-main.x + 990 + 2460 + 10)) || (main.x < 1080 && main.x >= 1040 && main.y == (main.x - 1080 + 2460 + 10))
						|| (main.x <= 830 && main.x > 790 && main.y == (-main.x + 790 + 2940 + 10)) || (main.x < 880 && main.x >= 840 && main.y == (main.x - 880 + 2940 + 10))
						|| (main.x <= 750 && main.x > 710 && main.y == (-main.x + 710 + 2620 + 10)) || (main.x < 800 && main.x >= 760 && main.y == (main.x - 800 + 2620 + 10))) {
						main.y -= main.speed;
						dog.y -= (main.speed + dog.speed);
						woman.y -= (main.speed + woman.speed);
						Fruits.y -= (main.speed + Fruits.speed);
						tree.y -= (main.speed + tree.speed);
					}

					if ((main.x >= 1870 && main.x <= 1910 && main.y == (-main.x + 2880)) || (main.x >= 750 && main.x <= 790 && main.y == (-main.x + 750 + 1060))
						|| (main.x <= 2190 && main.x > 2150 && main.y == (-main.x + 2150 + 1020)) || (main.x <= 2550 && main.x > 2510 && main.y == (-main.x + 2510 + 1020))
						|| (main.x <= 1150 && main.x > 1110 && main.y == (-main.x + 1110 + 1100)) || (main.x <= 790 && main.x > 750 && main.y == (-main.x + 750 + 1540))
						|| (main.x <= 910 && main.x > 870 && main.y == (-main.x + 870 + 2220)) || (main.x <= 1030 && main.x > 990 && main.y == (-main.x + 990 + 2460))
						|| (main.x <= 830 && main.x > 790 && main.y == (-main.x + 790 + 2940)) || (main.x <= 750 && main.x > 710 && main.y == (-main.x + 710 + 2620))) {
						main.x -= main.speed;
						dog.x -= (main.speed + dog.speed);
						woman.x -= (main.speed + woman.speed);
						Fruits.x -= (main.speed + Fruits.speed);
						tree.x -= (main.speed + tree.speed);
					}
					if (main.x >= 1920 && main.x < 1960 && main.y == (main.x - 940) || (main.x >= 800 && main.x <= 830 && main.y == (main.x + 220))
						|| (main.x < 2240 && main.x >= 2200 && main.y == (main.x - 2240 + 1020)) || (main.x < 2600 && main.x >= 2560 && main.y == (main.x - 2600 + 1020))
						|| (main.x < 1200 && main.x >= 1160 && main.y == (main.x - 1200 + 1100)) || (main.x < 840 && main.x >= 800 && main.y == (main.x - 830 + 1540 - 10))
						|| (main.x < 960 && main.x >= 920 && main.y == (main.x - 950 + 2210)) || (main.x < 1080 && main.x >= 1040 && main.y == (main.x - 1080 + 2460))
						|| (main.x < 880 && main.x >= 840 && main.y == (main.x - 880 + 2940)) || (main.x < 800 && main.x >= 760 && main.y == (main.x - 800 + 2620))) {
						main.x += main.speed;
						dog.x += (main.speed + dog.speed);
						woman.x += (main.speed + woman.speed);
						Fruits.x += (main.speed + Fruits.speed);
						tree.x += (main.speed + tree.speed);
					}
					if (timber.mod) {
						if (main.x > 1280 && main.y > 2820 && main.x < 1310 && main.y < 2890) {
							main.y -= main.speed;
							dog.y -= (main.speed + dog.speed);
							woman.y -= (main.speed + woman.speed);
							Fruits.y -= (main.speed + Fruits.speed);
							tree.y -= (main.speed + tree.speed);
						}
					}
				}
			}
		}
		else if (Map == 1) {
			if (main.atk == false) {
				if (KeyBuffer[VK_LEFT]) {
					main.walk = true;
					main.x -= main.speed;
					main.mod = 1;
					if ((main.x < 1870 && main.y >= 3410) || (main.x < 1910 && main.y >= 3530) || (main.x < 1030 && main.y >= 3330 && main.x>990) || (main.x < 990 && main.x>970 && main.y >= 3290) ||
						(main.x < 1070 && main.y <= 3280 && main.y >= 2730 && main.x>1050) || (main.x < 710 && main.y>2760) || (main.x < 830 && main.y <= 2800 && main.y >= 2120) || main.x < 670
						|| (main.x < 1070 && main.x>1050 && main.y < 2610 && main.y>2000) || (main.x < 1550 && main.x>1530 && main.y < 2730 && main.y >= 2200) || (main.x < 1310 && main.x>1290 && main.y < 2500 && main.y>2000)
						|| (main.x < 1830 && main.x>1800 && main.y <= 2720 && main.y >= 1680) || (main.x < 1030 && main.x>1000 && main.y < 1700 && main.y>1530)
						|| (main.x < 2070 && main.x>2050 && main.y <= 1560) || (main.x < 1350 && main.x>1330 && main.y <= 1560 && main.y >= 1200) || (main.x < 1150 && main.y < 1200)
						|| (main.x > 2120 && main.x < 2310 && main.y>2700 && main.y < 2930) || (main.x < 2790 && main.x>2750 && main.y <= 2760) || (main.y > 3270 && main.x > 2800 && main.x < 3070)
						|| (main.x > 3160 && main.x < 3350 && main.y>3150) || (main.x > 2680 && main.y < 3170 && main.x < 3350 && main.y>2870)
						|| (main.x > 1160 && main.y < 3290 && main.x < 1630 && main.y>2830) || (main.x > 1620 && main.y > 2830 && main.x < 1870 && main.y < 3130) || (main.x > 1720 && main.y < 3290 && main.x < 2590 && main.y>3030)
						|| (main.x > 2440 && main.y > 2830 && main.x < 2590 && main.y < 3080) || (main.x > 1370 && main.y > 960 && main.x < 1430 && main.y < 1040))
						main.x += main.speed;

				}
				if (KeyBuffer[VK_RIGHT]) {
					main.walk = true;
					main.x += main.speed;
					main.mod = 3;
					if ((main.x > 920 && main.x < 940 && main.y >= 2730) || (main.x <= 920 && main.x >= 810 && main.y >= 2880) || (main.x > 760 && main.x < 780 && main.y >= 1650 && main.y < 2010)
						|| (main.x > 920 && main.y < 2610 && main.x < 940) || (main.x > 1160 && main.x < 1180 && main.y>2000 && main.y < 2730) || (main.x > 1400 && main.x < 1420 && main.y >= 2200 && main.y < 2500)
						|| (main.x > 1640 && main.x < 1660 && main.y>2080 && main.y < 2730) || (main.x > 2160 && main.y > 1270 && main.y < 1700) || (main.x < 1500 && main.x>1440 && main.y <= 1560 && main.y >= 1200)
						|| (main.x > 1640 && main.y < 1200) || (main.x > 1920 && main.x < 1950 && main.y>1680 && main.y <= 2040) || (main.x > 2400 && main.x < 2450 && main.y>2000 && main.y < 2200)
						|| (main.x > 1920 && main.x < 1950 && main.y >= 2160 && main.y <= 2720) || (main.x > 2120 && main.x < 2150 && main.y>2700 && main.y < 2930)
						|| (main.x > 2600 && main.x < 2650 && main.y>2700 && main.y <= 2760) || (main.x < 2900 && main.x>2880 && main.y <= 2760 && main.y >= 2280)
						|| (main.x > 3120 && main.y > 2150 && main.y < 2330) || main.x > 3440 || (main.x > 2000 && main.y >= 3520) || (main.x > 2040 && main.y >= 3400) || (main.y > 3270 && main.x > 2800 && main.x < 3070) || (main.x > 3160 && main.x < 3350 && main.y>3150)
						|| (main.x > 2680 && main.y < 3170 && main.x < 3350 && main.y>2870) || (main.x > 1160 && main.y < 3290 && main.x < 1630 && main.y>2830) || (main.x > 1620 && main.y > 2830 && main.x < 1870 && main.y < 3130)
						|| (main.x > 1720 && main.y < 3290 && main.x < 2590 && main.y>3030) || (main.x > 2440 && main.y > 2830 && main.x < 2590 && main.y < 3080) || (main.x > 1370 && main.y > 960 && main.x < 1430 && main.y < 1040))
						main.x -= main.speed;
				}
				if (KeyBuffer[VK_UP]) {
					main.walk = true;
					main.y -= main.speed;
					main.mod = 2;
					if ((main.x >= 990 && main.x <= 1060 && main.y < 3290 && main.y >= 3280) || (main.x >= 710 && main.x < 830 && main.y < 2810 && main.y>2790) || (main.x >= 670 && main.x <= 760 && main.y < 1650)
						|| (main.x >= 770 && main.y < 2010 && main.x <= 1200 && main.y>1990) || (main.x >= 930 && main.x <= 1060 && main.y < 2610 && main.y>2580) || (main.x >= 1170 && main.x <= 1540 && main.y < 2730 && main.y>2700)
						|| (main.x > 1290 && main.x < 1650 && main.y < 2090 && main.y>2050) || (main.x <= 1820 && main.x >= 1650 && main.y < 2730 && main.y>2700) || (main.x <= 2720 && main.x >= 1930 && main.y < 2730 && main.y>2700)
						|| (main.x > 1000 && main.x <= 1340 && main.y < 1570 && main.y>1550) || (main.x >= 1450 && main.x <= 2060 && main.y < 1570 && main.y>1550)
						|| (main.x > 2050 && main.x < 2200 && main.y < 1290) || main.y < 890 || (main.x >= 1930 && main.y < 2050 && main.y>2000) || (main.x >= 2130 && main.x <= 2300 && main.y < 2930 && main.y>1900)
						|| (main.x >= 2610 && main.x <= 2780 && main.y < 2770 && main.y>2750) || (main.x >= 2890 && main.x <= 3450 && main.y < 2770 && main.y>2750)
						|| (main.x > 2750 && main.y < 2170) || (main.x > 2680 && main.y < 3170 && main.x < 3350 && main.y>2870) || (main.x > 1160 && main.y < 3290 && main.x < 1630 && main.y>2830)
						|| (main.x > 1620 && main.y > 2830 && main.x < 1870 && main.y < 3130) || (main.x > 1720 && main.y < 3290 && main.x < 2590 && main.y>3030) || (main.x > 2440 && main.y > 2830 && main.x < 2590 && main.y < 3080)
						|| (main.x > 1370 && main.y > 960 && main.x < 1430 && main.y < 1040))
						main.y += main.speed;
				}
				if (KeyBuffer[VK_DOWN]) {
					main.walk = true;
					main.y += main.speed;
					main.mod = 0;
					if ((main.x < 1870 && main.y>3390) || (main.x < 1910 && main.y >= 3530) || (main.x < 1030 && main.y >= 3330 && main.x >= 990)
						|| (main.x < 1070 && main.y >= 2730 && main.x>920 && main.y <= 2750) || (main.x <= 920 && main.x >= 810 && main.y >= 2880) || (main.x <= 820 && main.y > 2110 && main.y < 2130)
						|| (main.x <= 1540 && main.x > 1400 && main.y > 2190 && main.y <= 2210) || (main.x < 1420 && main.x>1300 && main.y > 2470 && main.y < 2500)
						|| (main.x <= 1820 && main.x > 1000 && main.y > 1670 && main.y < 1700) || (main.x <= 2200 && main.x >= 1930 && main.y > 1670 && main.y < 1700)
						|| (main.x <= 1340 && main.y > 1190 && main.y < 1250) || (main.x >= 1450 && main.y > 1190 && main.y < 1250) || (main.x < 2450 && main.x >= 1930 && main.y>2150 && main.y < 2180)
						|| (main.x < 3130 && main.x >= 2890 && main.y>2270 && main.y < 2300) || (main.x >= 2050 && main.y > 3390) || (main.x >= 2010 && main.y > 3510)
						|| (main.y > 3270 && main.x > 2800 && main.x < 3070) || (main.x > 2680 && main.y < 3170 && main.x < 3350 && main.y>2870)
						|| (main.x > 1160 && main.y < 3290 && main.x < 1630 && main.y>2830) || (main.x > 1620 && main.y > 2830 && main.x < 1870 && main.y < 3130)
						|| (main.x > 1720 && main.y < 3290 && main.x < 2590 && main.y>3030) || (main.x > 2440 && main.y > 2830 && main.x < 2590 && main.y < 3080) || (main.x > 1370 && main.y > 960 && main.x < 1430 && main.y < 1040))
						main.y -= main.speed;
				}

			}
		}
		*/
		//else
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
	else if (main.actmod == 1) {
		if (main.mod == 0) {
			if (fish.mod)
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * ((fish.count - 100) / 6 * 3 + 1), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
			else
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * (main.count / 6 * 3 + 1), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
		}
		if (main.mod == 1) {
			if (fish.mod)
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * ((fish.count - 100) / 6 * 3 + 2), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
			else
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * (main.count / 6 * 3 + 2), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
		}
		if (main.mod == 2) {
			if (fish.mod)
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * ((fish.count - 100) / 6 * 3 + 3), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
			else
				fishing.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
					0, fishing.GetHeight() / 9 * (main.count / 6 * 3), fishing.GetWidth(), fishing.GetHeight() / 9, RGB(255, 0, 255));
		}
	}
	else if (main.actmod == 2) {
		Timber.TransparentBlt(Memdc, rect.right / 2, rect.bottom / 2, 80, 110,
			0, fishing.GetHeight() / 9 * (main.count / 6 * 3), Timber.GetWidth(), Timber.GetHeight() / 9, RGB(255, 0, 255));
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
/*
void drawDog(HDC Memdc) {
	int startX = 220, startY = -2300;
	if (dog.mod == 0)
		Dog.TransparentBlt(Memdc, startX - dog.x, startY - dog.y, Dog.GetWidth() / 3 + 20, Dog.GetHeight() / 4 + 20,
			Dog.GetWidth() / 3 * (dog.count / 6), Dog.GetHeight() / 4 * 0, Dog.GetWidth() / 3, Dog.GetHeight() / 4, RGB(255, 255, 255));
	else if (dog.mod == 1)
		Dog.TransparentBlt(Memdc, startX - dog.x, startY - dog.y, Dog.GetWidth() / 3 + 20, Dog.GetHeight() / 4 + 20,
			Dog.GetWidth() / 3 * (dog.count / 6), Dog.GetHeight() / 4 * 1, Dog.GetWidth() / 3, Dog.GetHeight() / 4, RGB(255, 255, 255));
	else if (dog.mod == 2)
		Dog.TransparentBlt(Memdc, startX - dog.x, startY - dog.y, Dog.GetWidth() / 3 + 20, Dog.GetHeight() / 4 + 20,
			Dog.GetWidth() / 3 * (dog.count / 6), Dog.GetHeight() / 4 * 2, Dog.GetWidth() / 3, Dog.GetHeight() / 4, RGB(255, 255, 255));
	else if (dog.mod == 3)
		Dog.TransparentBlt(Memdc, startX - dog.x, startY - dog.y, Dog.GetWidth() / 3 + 20, Dog.GetHeight() / 4 + 20,
			Dog.GetWidth() / 3 * (dog.count / 6), Dog.GetHeight() / 4 * 3, Dog.GetWidth() / 3, Dog.GetHeight() / 4, RGB(255, 255, 255));
}
*/
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
		/*if (B.count == 50) {
		GAME_MODE = 2; CDestroy(); Load_cImage_C();
		}*/
		if (B.count == 72) {
		skip_GAMEMODE_1:
			GAME_MODE = 2; CDestroy(); 
			//Load_cImage_C();
		}
	}
	else {
		B.count_atkmod++;
		if (B.count_atkmod == 100)
		{
			B.count_atkmod = 0;
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
	static int j_insert;

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
	static CImage TITLE[2];
	static int TITLE_onoff;
	static int TITLE_TIMER;					//화면을 바꿔주는 인자값
	static int TITLE_BUFFER;				//시간을 체크하는 인자값

	/*
	static CImage STORY[39];
	static int STORY_onoff;
	static int STORY_TIMER;					//화면을 바꿔주는 인자값
	static int STORY_BUFFER;				//시간을 체크하는 인자값
	*/
											//				GAME_MODE == 1

	/*
	static CImage minimap[2];	//1은 미니맵 이미지 2 는 미니맵 포인트 이미지
	static int onoff_minimap;

	static CImage Help[10];
	static int Help_count;
	static int Help_timer;
	static int Help_2_timer;

	static int bird_onoff;
	static int bird_timer;
	static int bird_x;
	static int bird_y;
	static int bird_count;
	static int bird_count_1;
	static int bird_count_2;
	static int bird_count_3;

	static int bat_onoff;
	static int bat_count;
	static int bat_timer;
	static int bat_x;
	static int bat_y;
	//
	static int parable_onoff;
	static int parable_count;
	*/

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
		SetTimer(hwnd, 1, 50, NULL);
		///////////////////////////////////////////////////////////////////////
		System_Create(&pSystem);
		//	사운드 시스템 기본 세팅
		pSystem->init(1, FMOD_INIT_NORMAL, NULL);
		//	음악파일 생성
		pSystem->createSound(
			"Resource//sound//BossBgm.mp3",	//	사운드파일경로
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
		j_insert = 0;


		//////////////////////////////////////////////////////////////////////////////////// GAME_MODE == 0
		{
			////////////////워프로고작업추가1!
			WARP_count = 0;
			WARP_timer = 0;
			WARP_onoff = 1;
			WARP_number = 1;

			/*
			ZERO_onoff = 0;
			FIRST_onoff = 0;

			ZERO_TIMER = 0;
			ZERO_BUFFER = 0;
			*/
			/*
			FIRST_TIMER = 0;
			FIRST_BUFFER = 0;
			*/

			TITLE_TIMER = 0;
			TITLE_BUFFER = 0;

			/*
			STORY_TIMER = 0;
			STORY_BUFFER = 0;

			jump_function_onoff = 0;
			jump_timer = 0;
			jump_count = 0;
			*/

			jump[0].Load("Resource/jump_1.jpg");
			jump[1].Load("Resource/jump_2.jpg");


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

			/*
			ZERO[0].Load("Resource/storyimage/zero_black.jpg");
			ZERO[1].Load("Resource/storyimage/zero_1.jpg");
			ZERO[2].Load("Resource/storyimage/zero_2.jpg");
			ZERO[3].Load("Resource/storyimage/zero_3.jpg");
			ZERO[4].Load("Resource/storyimage/zero_4.jpg");
			ZERO[5].Load("Resource/storyimage/zero_5.jpg");
			ZERO[6].Load("Resource/storyimage/zero_6.jpg");
			ZERO[7].Load("Resource/storyimage/zero_7.jpg");
			ZERO[8].Load("Resource/storyimage/zero_8.jpg");
			ZERO[9].Load("Resource/storyimage/zero_9.jpg");
			ZERO[10].Load("Resource/storyimage/zero_10.jpg");
			ZERO[11].Load("Resource/storyimage/zero_11.jpg");
			ZERO[12].Load("Resource/storyimage/zero_12.jpg");
			ZERO[13].Load("Resource/storyimage/zero_13.jpg");
			ZERO[14].Load("Resource/storyimage/zero_14.jpg");
			ZERO[15].Load("Resource/storyimage/zero_15.jpg");
			ZERO[16].Load("Resource/storyimage/zero_16.jpg");
			ZERO[17].Load("Resource/storyimage/zero_17.jpg");
			ZERO[18].Load("Resource/storyimage/zero_18.jpg");
			ZERO[19].Load("Resource/storyimage/zero_19.jpg");
			ZERO[20].Load("Resource/storyimage/zero_20.jpg");
			ZERO[21].Load("Resource/storyimage/zero_21.jpg");
			ZERO[22].Load("Resource/storyimage/zero_22.jpg");
			ZERO[23].Load("Resource/storyimage/zero_23.jpg");
			ZERO[24].Load("Resource/storyimage/zero_24.jpg");
			ZERO[25].Load("Resource/storyimage/zero_25.jpg");
			ZERO[26].Load("Resource/storyimage/zero_26.jpg");
			ZERO[27].Load("Resource/storyimage/zero_27.jpg");
			ZERO[28].Load("Resource/storyimage/zero_28.jpg");
			ZERO[29].Load("Resource/storyimage/zero_29.jpg");
			*/

			TITLE[0].Load("Resource/storyimage/title.jpg");
			TITLE[1].Load("Resource/storyimage/title_1.jpg");

			/*
			STORY[0].Load("Resource/storyimage/story_1_0.jpg");
			STORY[1].Load("Resource/storyimage/story_1_1.jpg");
			STORY[2].Load("Resource/storyimage/story_1_2.jpg");
			STORY[3].Load("Resource/storyimage/story_1_3.jpg");
			STORY[4].Load("Resource/storyimage/story_1_4.jpg");
			STORY[5].Load("Resource/storyimage/story_1_5.jpg");
			STORY[6].Load("Resource/storyimage/story_1_6.jpg");
			STORY[7].Load("Resource/storyimage/story_1_7.jpg");
			STORY[8].Load("Resource/storyimage/story_1_8.jpg");
			STORY[9].Load("Resource/storyimage/story_1_9.jpg");
			STORY[10].Load("Resource/storyimage/story_1_10.jpg");
			STORY[11].Load("Resource/storyimage/story_1_11.jpg");
			STORY[12].Load("Resource/storyimage/story_1_12.jpg");
			STORY[13].Load("Resource/storyimage/story_1_13.jpg");
			STORY[14].Load("Resource/storyimage/story_1_14.jpg");
			STORY[15].Load("Resource/storyimage/story_1_15.jpg");
			STORY[16].Load("Resource/storyimage/story_1_16.jpg");
			STORY[17].Load("Resource/storyimage/story_1_17.jpg");
			STORY[18].Load("Resource/storyimage/story_1_18.jpg");
			STORY[19].Load("Resource/storyimage/story_1_19.jpg");
			STORY[20].Load("Resource/storyimage/story_1_20.jpg");
			STORY[21].Load("Resource/storyimage/story_1_21.jpg");
			STORY[22].Load("Resource/storyimage/story_1_22.jpg");
			STORY[23].Load("Resource/storyimage/story_1_23.jpg");
			STORY[24].Load("Resource/storyimage/story_1_24.jpg");
			STORY[25].Load("Resource/storyimage/story_1_25.jpg");
			STORY[26].Load("Resource/storyimage/story_1_26.jpg");
			STORY[27].Load("Resource/storyimage/story_1_27.jpg");
			STORY[28].Load("Resource/storyimage/story_1_28.jpg");
			STORY[29].Load("Resource/storyimage/story_1_29.jpg");
			STORY[30].Load("Resource/storyimage/story_1_30.jpg");
			STORY[31].Load("Resource/storyimage/story_1_31.jpg");
			STORY[32].Load("Resource/storyimage/story_1_32.jpg");
			STORY[33].Load("Resource/storyimage/story_1_33.jpg");
			STORY[34].Load("Resource/storyimage/story_1_34.jpg");
			STORY[35].Load("Resource/storyimage/story_1_35.jpg");
			STORY[36].Load("Resource/storyimage/story_1_36.jpg");
			STORY[37].Load("Resource/storyimage/story_1_37.jpg");
			STORY[38].Load("Resource/storyimage/story_1_38.jpg");
			*/
			/*
			FIRST[0].Load("first_white.jpg");
			FIRST[1].Load("first_1_1.jpg");
			FIRST[2].Load("first_1_2.jpg");
			FIRST[3].Load("first_1_3.jpg");
			FIRST[4].Load("first_1_4.jpg");
			FIRST[5].Load("first_1_5.jpg");
			FIRST[6].Load("first_1_6.jpg");
			FIRST[7].Load("first_1_7.jpg");
			FIRST[8].Load("first_1_8.jpg");
			FIRST[9].Load("first_1_9.jpg");
			FIRST[10].Load("first_1_10.jpg");
			FIRST[11].Load("first_1_11.jpg");
			FIRST[12].Load("first_1_12.jpg");
			FIRST[13].Load("first_1_13.jpg");
			FIRST[14].Load("first_1_14.jpg");
			FIRST[15].Load("first_1_15.jpg");
			FIRST[16].Load("first_1_16.jpg");
			FIRST[17].Load("first_1_17.jpg");
			FIRST[18].Load("first_1_18.jpg");
			FIRST[19].Load("first_1_19.jpg");
			FIRST[20].Load("first_1_20.jpg");
			FIRST[21].Load("first_1_21.jpg");
			FIRST[22].Load("first_1_22.jpg");
			FIRST[23].Load("first_1_23.jpg");
			FIRST[24].Load("first_1_24.jpg");
			FIRST[25].Load("first_1_25.jpg");
			FIRST[26].Load("first_1_26.jpg");
			FIRST[27].Load("first_1_27.jpg");
			FIRST[28].Load("first_1_28.jpg");
			FIRST[29].Load("first_1_29.jpg");
			*/
		}
		//////////////////////////////////////////////////////////////////////////////////// GAME_MODE == 1
		{
			/*
				Help[1].Load("Resource/storyimage/조작.jpg");
				Help[2].Load("Resource/storyimage/인사.jpg");
				Help[3].Load("Resource/storyimage/찾기.jpg");
				Help[4].Load("Resource/storyimage/알바.jpg");
				Help[5].Load("Resource/storyimage/낚시.jpg");
				Help[6].Load("Resource/storyimage/나무.jpg");
				Help[7].Load("Resource/storyimage/던전.jpg");
				Help[8].Load("Resource/storyimage/보스.jpg");

				people.count = 0;

				Help_count = 0;
				Help_timer = 0;
				Help_2_timer = 90;

				bird_onoff = 0;
				bird_timer = 0;
				bird_count_1 = 0;
				bird_count_2 = 3;
				bird_count_3 = 6;
				bird_x = 1400;
				bird_y = 100;
				bird_count = 0;

				bat_onoff = 0;
				bat_timer = 0;
				bat_count = 0;
				bat_x = 0;
				bat_y = 0;
		*/
			{
				main.x = 2270;
				main.y = 2270;
				main.count = 0;
				main.mod = 0;
				main.speed = 10;
				main.atk = false;
				//fish.mod = 0;
				//fish.button = false;
				//dog.walk = false;
				//Map = 0;
				//timber.speed = 40;
				//timber.y = 0;
				//timber.button = false;
				//timber.mod = true;
				Load_CImage();
				Load_CImage_B();
				B.mod = 0;
				B.count = 0;
				B.posX = 400;
				B.posY = -250;
				B.speed = 10;
				B.count_atk = 0;
				B.atk = false;
				B.HP = 100;
				for (int i = 0; i < 5; ++i) {
					HELLO[i] = true;
				}
				Fruits.select = -1;
				Head->next = nullptr;

				Map = 2;

				//	onoff_minimap = 0;
				//	minimap[0].Load("Resource/minimap.jpg");
			}
		}
		//////////////////////////////////////////////////////////////////////////////////// GAME_MODE == 2
		{
			/*
			END_onoff = 1;
			END_TIMER = 0;
			END_BUFFER = 0;

			FunFish_count = 0;
			FunFish_onoff = 0;

			FunShop_count = 0;
			FunShop_onoff = 0;

			parable_onoff = 0;
			parable_count = 12;

			dream_onoff = 0;
			dream_count = 0;
			*/
		}
		break;
	}
	case WM_PAINT:
	{
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
			{Memdc = CreateCompatibleDC(hdc);
			GetClientRect(hwnd, &rect);
			hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
			OldBitmap = (HBITMAP)SelectObject(Memdc, hBitmap);
			FillRect(Memdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			}
			Simul_Input();
			/*
			if (Map == 1 && main.x > 1900 && main.x < 2010 && main.y == 3610) {
				main.x = 1050;
				main.y = 1060;
				main.count = 0;
				main.mod = 0;// 위쪽바라보는 모습
				Map = 0;
				pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[2], false, &pChannel[0]);
			}
			if (Map == 0) {
				bk.TransparentBlt(Memdc, 0, 0, rect.right, rect.bottom,
					main.x, main.y, rect.right / 2, rect.bottom / 2, RGB(0, 255, 252));
				if (timber.mod)
				{
					Tree.TransparentBlt(Memdc, -1340 - tree.x, +1480 - tree.y, Tree.GetWidth() * 2, Tree.GetHeight() * 2,
						0, 0, Tree.GetWidth(), Tree.GetHeight(), RGB(255, 255, 255));

					//if (main.actmod != 2) {
					//MyPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
					//OldPen = (HPEN)SelectObject(Memdc, MyPen);
					//MoveToEx(Memdc, -1340 - tree.x, +1480 - tree.y + Tree.GetHeight() * 2 + 10, NULL);
					//LineTo(Memdc, -1340 - tree.x, +1480 - tree.y + Tree.GetHeight() * 2 - 10);
					//
					//MoveToEx(Memdc, -1340 - tree.x, +1480 - tree.y + Tree.GetHeight() * 2 + 10, NULL);
					//LineTo(Memdc, -1340 - tree.x + Tree.GetWidth() * 2, +1480 - tree.y + Tree.GetHeight() * 2 + 10);
					//
					//MoveToEx(Memdc, -1340 - tree.x + Tree.GetWidth() * 2, +1480 - tree.y + Tree.GetHeight() * 2 + 10, NULL);
					//LineTo(Memdc, -1340 - tree.x + Tree.GetWidth() * 2, +1480 - tree.y + Tree.GetHeight() * 2 - 10);
					//
					//MoveToEx(Memdc, -1340 - tree.x, +1480 - tree.y + Tree.GetHeight() * 2 - 10, NULL);
					//LineTo(Memdc, -1340 - tree.x + Tree.GetWidth() * 2, +1480 - tree.y + Tree.GetHeight() * 2 - 10);
					//DeleteObject(SelectObject(Memdc, OldPen));
					//}
				}
				if (!Fruits.atk) {

					//MyPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
					//OldPen = (HPEN)SelectObject(Memdc, MyPen);
					//MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y, NULL);
					//LineTo(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y + 100);
					//
					//MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y, NULL);
					//LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y);
					//
					//MoveToEx(Memdc, 342 - Fruits.x, 1345 - Fruits.y, NULL);
					//LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 100);
					//
					//MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y + 100, NULL);
					//LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 100);
					//DeleteObject(SelectObject(Memdc, OldPen));
				}
				if (dog.walk)
					drawDog(Memdc);

				if (fish.mod == 1) {
					fishGage[0].StretchBlt(Memdc, 20, 100, rect.right / 10, rect.bottom - 100, 0, 0, fishGage[0].GetWidth(), fishGage[0].GetHeight(), SRCAND);
					fishGage[1].StretchBlt(Memdc, 20, 100, rect.right / 10, rect.bottom - 100, 0, 0, fishGage[1].GetWidth(), fishGage[1].GetHeight(), SRCPAINT);
					fishPoint[0].StretchBlt(Memdc, 76, rect.bottom - 55 - fish.y, 44, 40, 0, 0, fishPoint[0].GetWidth(), fishPoint[0].GetHeight(), SRCAND);
					fishPoint[1].StretchBlt(Memdc, 76, rect.bottom - 55 - fish.y, 44, 40, 0, 0, fishPoint[1].GetWidth(), fishPoint[1].GetHeight(), SRCPAINT);
				}
				if (main.actmod == 2) {//245 325
					hFont = CreateFont(27.55, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY목각파임B"));
					saveFont = (HFONT)SelectObject(Memdc, hFont);
					SetBkMode(Memdc, TRANSPARENT);
					wsprintf(temp3, "%d", timber.count);
					TextOut(Memdc, rect.right / 2 - 3, rect.bottom / 2 + 240, temp3, strlen(temp3));
					DeleteObject(SelectObject(Memdc, saveFont));
					if (!timber.button)
						TimberGage.StretchBlt(Memdc, rect.right / 4, rect.bottom - 50 - 20, rect.right / 2, 50, 0, 0, TimberGage.GetWidth(), TimberGage.GetHeight(), SRCCOPY);
					else
						TimberGage.StretchBlt(Memdc, rect.right / 4 - 10, rect.bottom - 60 - 20, rect.right / 2 + 10, 60, 0, 0, TimberGage.GetWidth(), TimberGage.GetHeight(), SRCCOPY);
					timber.button = false;
					TimberPoint.StretchBlt(Memdc, rect.right / 4 + timber.y, rect.bottom - 50 - 20, 20, 50, 0, 0, TimberPoint.GetWidth(), TimberPoint.GetHeight(), SRCCOPY);
				}
				if (Fruits.atk) {
					if (main.y < 2550)
					{
						Fruits.atk = false;
						Fruits.score = 0;
					}
					if (Fruits.actmod == 1) {
						fruits[Fruits.count].TransparentBlt(Memdc, 162 - Fruits.x, 1345 - Fruits.y, fruits[0].GetWidth() * 2, fruits[0].GetHeight() * 2,
							0, 0, fruits[0].GetWidth(), fruits[0].GetHeight(), RGB(255, 255, 255));
					}
					if (Fruits.actmod == 2) {
						fruits[(Fruits.count + Fruits.mod) % 3].TransparentBlt(Memdc, 342 - Fruits.x, 1340 - Fruits.y, fruits[0].GetWidth() * 2, fruits[0].GetHeight() * 2,
							0, 0, fruits[0].GetWidth(), fruits[0].GetHeight(), RGB(255, 255, 255));
						fruits[(Fruits.count + Fruits.mod + 1) % 3].TransparentBlt(Memdc, 342 - Fruits.x, 1260 - Fruits.y, fruits[0].GetWidth() * 2, fruits[0].GetHeight() * 2,
							0, 0, fruits[0].GetWidth(), fruits[0].GetHeight(), RGB(255, 255, 255));
						fruits[(Fruits.count + Fruits.mod + 2) % 3].TransparentBlt(Memdc, 342 - Fruits.x, 1420 - Fruits.y, fruits[0].GetWidth() * 2, fruits[0].GetHeight() * 2,
							0, 0, fruits[0].GetWidth(), fruits[0].GetHeight(), RGB(255, 255, 255));
					}
					hFont = CreateFont(27.55, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("HY목각파임B"));
					saveFont = (HFONT)SelectObject(Memdc, hFont);
					SetBkMode(Memdc, TRANSPARENT);
					if (Fruits.select != -1)
					{
						wsprintf(temp3, "Select");
						TextOut(Memdc, rect.right / 2 - 30, rect.bottom / 2 - 50, temp3, strlen(temp3));
												MyPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
						OldPen = (HPEN)SelectObject(Memdc, MyPen);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10, NULL);
						LineTo(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10);

						MoveToEx(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70);
						//
						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10 - 80, NULL);
						LineTo(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70 - 80);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10 - 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10 - 80);

						MoveToEx(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10 - 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70 - 80);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70 - 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70 - 80);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10 + 80, NULL);
						LineTo(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70 + 80);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 10 + 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10 + 80);

						MoveToEx(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 10 + 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70 + 80);

						MoveToEx(Memdc, 342 - Fruits.x - 20, 1345 - Fruits.y + 70 + 80, NULL);
						LineTo(Memdc, 342 - Fruits.x, 1345 - Fruits.y + 70 + 80);

						DeleteObject(SelectObject(Memdc, OldPen));

					}
					else {
						/*
						MyPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
						OldPen = (HPEN)SelectObject(Memdc, MyPen);
						MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y, NULL);
						LineTo(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y + 100);

						MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y, NULL);
						LineTo(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2 + 20, 1345 - Fruits.y);

						MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2 + 20, 1345 - Fruits.y, NULL);
						LineTo(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2 + 20, 1345 - Fruits.y + 100);

						MoveToEx(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2, 1345 - Fruits.y + 100, NULL);
						LineTo(Memdc, 162 - Fruits.x + fruits[0].GetWidth() * 2 + 20, 1345 - Fruits.y + 100);
						DeleteObject(SelectObject(Memdc, OldPen));

					}
					wsprintf(temp3, "%d", Fruits.score);
					TextOut(Memdc, rect.right / 2 - 3, rect.bottom / 2 + 240, temp3, strlen(temp3));
					DeleteObject(SelectObject(Memdc, saveFont));

				}

				if (chief.count < 7) {
					FeelChart.TransparentBlt(Memdc, 467 - tree.x, 470 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));
				}
				else {
					if (HELLO_count > HELLO_LIMIT && chief.count < 9)
						Qmark.TransparentBlt(Memdc, 467 - tree.x - 10, 470 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));

					if (shop.count == 2 && house.count == 3)
						Qmark.TransparentBlt(Memdc, 200 - tree.x - 10, 930 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (shop.count != 3 && house.count == 3)
						FeelChart.TransparentBlt(Memdc, 200 - tree.x, 930 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));

					if (house.count == 3)
						Qmark.TransparentBlt(Memdc, 70 - tree.x - 10, -1300 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (house.count != 4)
						FeelChart.TransparentBlt(Memdc, 70 - tree.x, -1300 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));

					if (fisher.count == 2)
						Qmark.TransparentBlt(Memdc, 2865 - tree.x - 10, -90 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (fisher.count != 3)
						FeelChart.TransparentBlt(Memdc, 2865 - tree.x, -90 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));

					if (kid.count == 2)
						Qmark.TransparentBlt(Memdc, 945 - tree.x - 10, -1240 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (kid.count != 3)
						FeelChart.TransparentBlt(Memdc, 945 - tree.x, -1240 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));

					if (timberman.count == 2)
						Qmark.TransparentBlt(Memdc, -1050 - tree.x - 10, 1420 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (timberman.count != 3)
						FeelChart.TransparentBlt(Memdc, -1050 - tree.x, 1420 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));

					if (soldier.count != 4)
						Qmark.TransparentBlt(Memdc, -1860 - tree.x - 10, -2110 - tree.y, Qmark.GetWidth(), Qmark.GetHeight(), 0, 0, Qmark.GetWidth(), Qmark.GetHeight(), RGB(255, 255, 255));
					else if (soldier.count != 4)
						FeelChart.TransparentBlt(Memdc, -1860 - tree.x, -2110 - tree.y, FeelChart.GetWidth(), FeelChart.GetHeight(), 0, 0, FeelChart.GetWidth(), FeelChart.GetHeight(), RGB(255, 255, 255));
				}
			}
			else if (Map == 1) {
				maze.StretchBlt(Memdc, 0, 0, rect.right, rect.bottom, main.x, main.y, rect.right, rect.bottom, SRCCOPY);

			}
			*/

			//else 
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
					Rectangle(Memdc, 360 + i * 7, 615, 360 + (i + 1) * 7, 645);
				}

				HPbar.TransparentBlt(Memdc, 240, 580, 840, 100, RGB(0, 0, 0));

				SelectObject(Memdc, oldBrush);
				DeleteObject(Brush);


				SelectObject(Memdc, OldPen);
				DeleteObject(RedPen);

				if (B.HP > 0)
				{
					if (B.atk)drawBullet(Memdc);
				}
			}
			HELLOWORLD.TransparentBlt(Memdc, 0, 0, 50 * HELLO_count, 50,
				HELLOWORLD.GetWidth() * 0, 0, HELLOWORLD.GetWidth() / 10 * HELLO_count, HELLOWORLD.GetHeight(), RGB(255, 255, 255));

			drawFinn(Memdc);

			/*
			if (Map == 1)
			{
				if (bat_onoff)
				{
					Rectangle(Memdc, 0, 0, 100, 100);
					bat[bat_count].TransparentBlt(Memdc, bat_x, bat_y, 60, 60, RGB(255, 255, 255));
				}

				blind.TransparentBlt(Memdc, 0, 0, blind.GetWidth(), blind.GetHeight(),
					0, 0, blind.GetWidth() - 30, blind.GetHeight(), RGB(255, 255, 255));
			}

			if (bird_onoff)
			{
				bird[bird_count_1].TransparentBlt(Memdc, bird_x, bird_y, 100, 50, RGB(255, 0, 0));
				bird[bird_count_2].TransparentBlt(Memdc, bird_x + 120, bird_y - 40, 100, 50, RGB(255, 0, 0));
				bird[bird_count_3].TransparentBlt(Memdc, bird_x + 80, bird_y + 60, 100, 50, RGB(255, 0, 0));
			}

			if (onoff_minimap)
				minimap[0].StretchBlt(Memdc, 360, 90, 550, 550, 0, 0, minimap[0].GetWidth(), minimap[0].GetHeight(), SRCCOPY);
			*/
			//wsprintf(temp3, "%d :%d", main.x, main.y);
			//TextOut(Memdc, rect.right / 2 - 3, rect.bottom / 2, temp3, strlen(temp3));
			/*
			if (Talk) {
				if (people.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					People[people.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (chief.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[chief.count].GetWidth(), VillageChief[chief.count].GetHeight(), SRCAND);
					VillageChief[chief.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[chief.count].GetWidth(), VillageChief[chief.count].GetHeight(), SRCPAINT);
				}
				if (shop.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					ShopKeeper[shop.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (house.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					HouseWife[house.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (kid.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					Kid[kid.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (fisher.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					Fisherman[fisher.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (timberman.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					TImberman[timberman.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (soldier.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					Soldier[soldier.count].StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
				if (captin.button) {
					talkMask.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCAND);
					Captin.StretchBlt(Memdc, rect.right / 4, rect.bottom - 200, rect.right / 2, 200,
						0, 0, VillageChief[0].GetWidth(), VillageChief[0].GetHeight(), SRCPAINT);
				}
			}
			*/

			/*
			if (Help_count)
			{
				Help[Help_count].StretchBlt(Memdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
			}
			*/

			BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Memdc, 0, 0, SRCCOPY);
			hBitmap = (HBITMAP)SelectObject(Memdc, OldBitmap);
			DeleteDC(Memdc);
			/*ReleaseDC(hwnd, hdc);*/
			DeleteObject(hBitmap);
		}
		//EndPaint(hwnd, &ps);
	/*
	else if (GAME_MODE == 2)
	{
		if (dream_onoff)
		{
			dream[dream_onoff][dream_count].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);

		}
		else if (FunShop_onoff)
			FunShop[FunShop_count].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
		else if (FunFish_onoff)
			FunFish[FunFish_count].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
		else if (parable_onoff)
			parable[parable_count].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
		else if (END_onoff)
			END[END_TIMER].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 30, SRCCOPY);
	}

	if (jump_function_onoff)
		jump[jump_count].StretchBlt(hdc, 950, 0, 310, 100, SRCCOPY);
	*/

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
		break;
	}
	case WM_TIMER:
	{
		/*
		if (jump_function_onoff)
		{
			jump_timer++;
			if (jump_timer == 100)
			{
				jump_timer = 0;
				jump_function_onoff = 0;
			}
		}
		*/
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
					pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[3], false, &pChannel[0]);
					WARP_onoff = 0;
					TITLE_onoff = 1;
				}
			}
			/*
			else if (ZERO_onoff)
			{
				ZERO_BUFFER++;

				if (ZERO_BUFFER == 165)
					pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[0]);


				if (ZERO_BUFFER >= 21 && ZERO_BUFFER <= 30)		//영어 문구 출력용
					ZERO_TIMER++;
				else if (ZERO_BUFFER >= 80 && ZERO_BUFFER < 90)		//한글 문구 출력용
					ZERO_TIMER++;
				else if (ZERO_BUFFER >= 150 && ZERO_BUFFER < 159)
					ZERO_TIMER++;
				else if (ZERO_BUFFER == 175)
				{
					ZERO_BUFFER = 0;
					ZERO_onoff = 0;
					//FIRST_onoff = 1;
					TITLE_onoff = 1;
				}
			}
			*/
			/*
			else if (FIRST_onoff)
			{
			FIRST_BUFFER++;

			if (FIRST_BUFFER >= 21 && FIRST_BUFFER <= 30)
			FIRST_TIMER++;
			else if (FIRST_BUFFER >= 61 && FIRST_BUFFER <= 70)
			FIRST_TIMER++;
			else if (FIRST_BUFFER >= 110 && FIRST_BUFFER <= 118)
			FIRST_TIMER++;
			else if (FIRST_BUFFER == 119)
			{
			FIRST_BUFFER = 0;
			FIRST_onoff = 0;

			for (i = 0; i < 30; i++)
			FIRST[i].Destroy();
			}
			}*/
			else if (TITLE_onoff)
			{
				TITLE_BUFFER++;

				if (TITLE_BUFFER > 60)
				{
					if (TITLE_BUFFER % 30 < 15)
						TITLE_TIMER = 0;
					else
						TITLE_TIMER = 1;
				}
			}
			/*
			else if (STORY_onoff)
			{
				STORY_BUFFER++;
				if (STORY_TIMER == 1)  //? 이해를 할 수 없는 구조라서 바꿀수 가 없다.
				{
					//pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[5], false, &pChannel[0]);
				}
				if (STORY_BUFFER > 20 && STORY_BUFFER < 30)
					STORY_TIMER++;
				else if (STORY_BUFFER % 60 == 29 && STORY_TIMER < 13)
				{
					STORY_TIMER++;
					if (STORY_TIMER == 11)
					{
						j_insert = 1;
						jump_function_onoff = 1;
						jump_count = 0;
					}
				}
				else if (12 < STORY_TIMER && STORY_TIMER < 25)
					STORY_TIMER++;
				else if (24 < STORY_TIMER && STORY_TIMER < 32 && STORY_BUFFER % 60 == 29)
					STORY_TIMER++;
				else if (31 < STORY_TIMER && STORY_TIMER < 38)
				{
					STORY_TIMER++;
					STORY_BUFFER = 300;
				}
				else if (STORY_TIMER == 38 && STORY_BUFFER == 360)
				{
				skip_GAMEMODE_0:

					GAME_MODE = 1;
					END_onoff = 1;
					pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[2], false, &pChannel[0]);
					Help_count = 1;

					STORY_onoff = 0;

					TITLE[0].Destroy();
					TITLE[1].Destroy();

					for (i = 0; i < 30; i++)
						ZERO[i].Destroy();

					Destroy_WARP();

					for (i = 0; i < 39; i++)
					{
						STORY[i].Destroy();
					}
				}
			}
			*/
		}
		else if (GAME_MODE == 1)
		{
			if (Skip_Gamemode_1)
			{
				GAME_MODE = 2; CDestroy();
				//Load_cImage_C();
			}
			/*
			if (Map == 1)
			{
				bat_timer++;

				if (bat_timer == 100)
				{
					bat_onoff = 1;
					bat_count++;
					bat_timer = 0;

					if (bat_count == 3)
						bat_count = 0;

					if (bat_count == 0)
					{
						bat_x = 800;
						bat_y = 100;
					}
					else if (bat_count == 1)
					{
						bat_x = 380;
						bat_y = 100;
					}
					else if (bat_count == 2)
					{
						bat_x = 640;
						bat_y = 100;
					}
				}
			}
			if (bat_onoff)
			{
				if (bat_count == 0)
				{
					bat_x = bat_x - 20;
					bat_y = bat_y + 20;
				}
				else if (bat_count == 1)
				{
					bat_x = bat_x + 20;
					bat_y = bat_y + 20;
				}
				else if (bat_count == 2)
				{
					bat_y = bat_y + 25;
				}

				if (bat_y == 1000)
				{
					bat_onoff = 0;
				}
			}


			if (Map == 0)
			{
				bird_timer++;

				if (bird_timer == 180)
				{
					bird_onoff = 1;
					bird_count++;
				}
				else if (bird_timer == 3600)
				{
					bird_timer = 0;
				}
			}

			if (bird_onoff == 1)
			{
				bird_count_1++;
				bird_count_2++;
				bird_count_3++;

				if (bird_count_1 == 9)
					bird_count_1 = 0;
				if (bird_count_2 == 9)
					bird_count_2 = 0;
				if (bird_count_3 == 9)
					bird_count_3 = 0;

				bird_x = bird_x - 10;

				if (bird_count % 3 == 2)
				{
					if (bird_x > 800)
						bird_y = bird_y + 1;
					else if (bird_x < 600)
						bird_y = bird_y - 2;
					else if (bird_x < 400)
						bird_y = bird_y - 1;
				}
				else if (bird_count % 3 == 0)
				{
					if (bird_x > 800)
						bird_y = bird_y + 1;
					else if (bird_x > 600)
						bird_y = bird_y - 2;
					else if (bird_x > 300)
						bird_y = bird_y + 2;
					else
						bird_y = bird_y - 2;
				}

				if (bird_x < -200)
				{
					bird_onoff = 0;
					bird_x = 1400;
					bird_y = 100;
				}
			}

			*/

			/*
			if (Map != 0)
			{
				bird_onoff = 0;
			}

			if (Help_count != 0 && Help_timer == 0)
			{
				Help_timer = 65;
			}
			else if (Help_count != 0 && Help_timer == 1)
			{
				if (Help_count == 1)
					Help_2_timer = 69;
				else if (Help_count == 8)
					Map = 2;

				Help_count = 0;
				Help_timer = 0;
			}
			else if (Help_count != 0 && Help_timer != 0)
			{
				Help_timer--;
			}


			if (Help_2_timer < 90 && Help_2_timer > 0)
			{
				Help_2_timer--;
			}
			else if (Help_2_timer == 0)
			{
				Help_count = 2;
				Help_2_timer = 100; //스톱!
				jump_function_onoff = 1;
				jump_count = 1;
			}
			*/

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
			/*
			else if (main.actmod == 1) {
				if (main.count == 17) {
					fish.count++;
					if (fish.count == 99)
						PlaySound("Resource\\sound\\Reeling.wav", NULL, SND_ASYNC | SND_LOOP);
					if (fish.count >= 100)
					{
						fish.mod = 1;
						if (fish.y > 0)
							fish.y -= 1;
						if (fish.count == 111)fish.count = 100;

						fishcount++;
						if (fishcount == 30) { if (fish.y > 0)fish.y -= 50; fishcount = 0; if (fish.y < 0)fish.y = 0; }
					}
				}
				else
					main.count++;
			}
			else if (main.actmod == 2) {
				main.count++;
				if (main.count == 18) {
					main.count = 0;
				}

				timber.y += timber.speed;
				if (timber.y > 590 || timber.y < 20)timber.speed *= -1;
			}
			else if (main.actmod == 3) {
				main.count++;
				if (main.count == 3) { main.count = 0; main.actmod = 0; }
			}
			else if (main.actmod == 4) {
				main.count++;
				if (main.count == 12) {
					B.mod = 0;
					B.count = 0;
					B.posX = 400;
					B.posY = -250;
					B.speed = 10;
					B.count_atk = 0;
					B.atk = false;
					B.HP = 100;

					main.x = 745;
					main.y = 780;
					main.count = 0;
					main.mod = 0;
					main.speed = 10;
					main.atk = false;
					start = 0;
					count_intro = 0;
					main.actmod = 0;
					HELLO_count = main.HP;
				}
			}
			*/
			/*
			if (Fruits.atk) {

				if (Fruits.actmod == 2) {
					Fruits.atkmod++;
					if (Fruits.atkmod == 20) {
						Fruits.atkmod = 0;
						Fruits.actmod = 1;
					}
				}
			}
			if (dog.walk) {
				dog.count++;
				if (dog.count == 18) {
					dog.count = 0;
					dog.mod = rand() % 4;
				}
				if (dog.mod == 0) {
					dog.y -= 2;
					//if (dog.y < 200)dog.y+=2;
				}
				else if (dog.mod == 1)
					dog.x += 2;
				else if (dog.mod == 2)
					dog.x -= 2;
				else if (dog.mod == 3) {
					dog.y += 2;
					//if (dog.y>-200)	dog.y -= 2;
				}

				if (230 - dog.x > rect.right / 2 - 10 && -(2290 + dog.y) > rect.bottom / 2 - 10 &&
					230 - dog.x + Dog.GetWidth() / 3 < rect.right / 2 + 50 + 10 &&
					-(2290 + dog.y) + Dog.GetHeight() / 4 < rect.bottom / 2 + Finn.GetHeight() / 44 * 2 + 10) {
					dog.walk = false;
					PlaySound("Resource\\sound\\멍5.wav", NULL, SND_ASYNC);
					kid.mod = 1;
				}
			}
			*/
		}
		/*
		else if (GAME_MODE == 2)
		{
			if (END_onoff)
			{
				END_BUFFER++;

				if (END_TIMER == 1)
				{
					pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[3], false, &pChannel[0]);
					minimap[0].Destroy(); minimap[1].Destroy();

					Help[0].Destroy(); Help[1].Destroy(); Help[2].Destroy(); Help[3].Destroy(); Help[4].Destroy();
					Help[5].Destroy(); Help[6].Destroy(); Help[7].Destroy(); Help[8].Destroy(); Help[9].Destroy();
					jump[0].Destroy(); jump[1].Destroy();
				}

				if (END_TIMER < 11)
				{
					END_TIMER++;
					END_BUFFER = 0;

					if (END_TIMER > 0)
						END[END_TIMER - 1].Destroy();
				}
				else if (END_TIMER < 32)
				{
					//if (END_TIMER == 14)
					//{
					//pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[6], false, &pChannel[0]);
					//}

					if (dream_onoff == 1 || dream_onoff == 2)
					{
						if (END_BUFFER % 3 == 0)
						{
							dream_count++;
							END_BUFFER = 0;

							if (dream_count == 10)
							{
								if (dream_onoff == 1)
								{
									dream_onoff = 0;
									dream_count = 0;
									FunShop_onoff = 1;
								}
								else if (dream_onoff == 2)
								{
									dream_onoff = 0;
									dream_count = 0;
									FunFish_onoff = 1;
								}
							}
						}
					}
					else if (FunShop_onoff == 1)
					{
						if (END_BUFFER % 60 == 0)
						{
							FunShop_count++;
							END_BUFFER = 0;

							if (FunShop_count == 3)
							{
								END_TIMER = 21;

								FunShop_onoff = 0;
								END_BUFFER = 0;
							}
						}
					}
					else if (FunFish_onoff == 1)
					{
						if (FunFish_count < 13)
						{
							if (END_BUFFER % 5 == 0)
							{
								FunFish_count++;
								END_BUFFER = 0;
							}
						}
						else if (FunFish_count < 15)
						{
							if (END_BUFFER % 20 == 0)
							{
								FunFish_count++;
								END_BUFFER = 0;
							}
						}
						else
						{
							if (END_BUFFER % 60 == 0)
							{
								FunFish_count++;
								END_BUFFER = 0;

								if (FunFish_count == 18)
								{
									END_TIMER = 27;

									FunFish_onoff = 0;
									END_BUFFER = 0;
								}
							}
						}

					}
					else if (END_TIMER == 19)
					{
						dream_onoff = 1;

						END_BUFFER = 0;
					}
					else if (END_TIMER == 25)
					{
						dream_onoff = 2;

						END_BUFFER = 0;
					}
					else if (END_BUFFER % 60 == 0)
					{
						END_TIMER++;
						END_BUFFER = 0;
					}
				}
				else if (END_TIMER < 38)
				{
					if (END_TIMER == 12 && END_BUFFER == 1)
					{
						for (i = 11; i < 32; i++)
							END[i].Destroy();
					}

					if (END_TIMER == 37)
						pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[3], false, &pChannel[0]);

					if (END_BUFFER % 2 == 0)
					{
						END_TIMER++;

						if (END_TIMER > 0)
							END[END_TIMER - 1].Destroy();

						END_BUFFER = 0;
					}
				}
				else if (END_TIMER == 38)
				{
					if (END_BUFFER == 1)
					{


						for (i = 32; i < 38; i++)
							END[i].Destroy();
					}

					parable_onoff = 1;

					if (parable_onoff)
					{
						if (parable_count % 2 == 0 && END_BUFFER == 50)
						{
							parable_count++;
							END_BUFFER = 0;
						}
						if (parable_count % 2 == 1 && END_BUFFER == 15)
						{
							parable_count++;
							END_BUFFER = 0;
						}

						if (parable_count == 12)
						{

							pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[4], false, &pChannel[0]);
						}
						else if (parable_count == 14)
						{
							parable_onoff = 0;
							END_TIMER = 39;
							END_BUFFER = 0;
						}
					}
					/*
					if (END_BUFFER % 80 == 0)
					{
					END_TIMER++;

					if (END_TIMER > 0)
					END[END_TIMER - 1].Destroy();

					END_BUFFER = 0;
					}

				}
				else if (END_TIMER < 49)
				{
					if (END_BUFFER % 2 == 0)
					{
						END_TIMER++;

						if (END_TIMER > 0)
							END[END_TIMER - 1].Destroy();

						END_BUFFER = 0;
					}
				}
				else if (END_TIMER < 55)
				{
					if (END_BUFFER % 15 == 0)
					{
						END_TIMER++;

						if (END_TIMER > 0)
							END[END_TIMER - 1].Destroy();

						END_BUFFER = 0;
					}
				}
			}
		}
		*/
		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
		/*
		if (j_insert)
		{
			if (GAME_MODE == 0)
			{
				if (wParam == 'j' || wParam == 'J')
				{
					jump_timer = 0;
					jump_function_onoff = 0;
					goto skip_GAMEMODE_0;
				}
			}
			else if (GAME_MODE == 1)
			{
				if (wParam == 'j' || wParam == 'J')
				{
					Skip_Gamemode_1 = 1;
					jump_timer = 0;
					jump_function_onoff = 0;
				}
			}
		}
		*/
		if (TITLE_onoff)
		{
			if (TITLE_BUFFER > 60)	// 노래 조금 들려줘야지!!
			{
				TITLE_onoff = 0;	// 
									//책갈피 게임 시작				
				Map = 2;
				GAME_MODE = 1;
				main.actmod == 0;

				HELLO[0] = true;
				HELLO[1] = true;
				HELLO[2] = true;
				HELLO[3] = true;
				HELLO[4] = true;

				
				//STORY_onoff = 1;	// 

									//FMOD_Channel_Stop(channel);  -> FMOD:: Channel 관련 수정작업 필요
				pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[3], false, &pChannel[0]);	//NoSOund
				PlaySound("Resource\\sound\\push.wav", NULL, SND_ASYNC);	//Push Start Button - "any key setting"
			}
		}
		else if (GAME_MODE == 1)
		{
			/*
			if (Talk) {
				if (people.button) {
					if (people.mod == 1) {
						Talk = false;
						people.button = false;
						people.mod = 0;
					}
				}
				if (captin.button) {

					if (captin.mod == 1) {
						Talk = false;
						captin.button = false;
						captin.mod = 0;
					}
				}
				if (chief.button) {
					//if (chief.count < 7)
					//{
					chief.count++;
					//}
					//else if (HELLO_count > 2)
					//	chief.count++;

					if (chief.count == 7) {
						Talk = false;
						chief.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
					if (chief.count == 9) {
						Talk = false;
						chief.button = false;
					}
				}
				if (shop.button) {
					shop.count++;
					if (shop.count == 2) {
						Talk = false;
						shop.button = false;
						Help_count = 4;
					}
					if (shop.count == 3) {
						Talk = false;
						shop.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (house.button) {
					house.count++;
					if (house.count == 3) {
						Talk = false;
						house.button = false;
					}
					if (house.count == 4) {
						Talk = false;
						house.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (kid.button) {
					kid.count++;
					if (kid.count == 2) {
						Talk = false;
						kid.button = false;
						dog.walk = true;
						Help_count = 3;
					}
					if (kid.count == 3) {
						Talk = false;
						kid.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (fisher.button) {
					fisher.count++;
					if (fisher.count == 2) {
						Talk = false;
						fisher.button = false;
						Help_count = 5;
					}
					if (fisher.count == 3) {
						Talk = false;
						fisher.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (timberman.button) {
					timberman.count++;
					if (timberman.count == 2) {
						Talk = false;
						timberman.button = false;
						Help_count = 6;
					}
					if (timberman.count == 3) {
						Talk = false;
						timberman.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (soldier.button) {
					soldier.count++;
					if (soldier.count == 3) {
						Talk = false;
						soldier.button = false;
						Help_count = 7;
					}
					if (soldier.count == 4) {
						Talk = false;
						soldier.button = false;
						if (HELLO_count < 10)
						{
							HELLO_count++;
						}
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
			}
			*/
			{
			//else {
				if (wParam == 'P' || wParam == 'p') {
					if (HELLO_count < 10)
					{
						HELLO_count++;
						PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
					}
				}
				if (wParam == 'a' || wParam == 'A') {

					if (main.actmod == 0 && timber.mod == true && Map == 0) {
						if (main.x > 1280 && main.x < 1310 && main.y == 2890) {
							main.actmod = 2;
							main.count = 0;
						}
					}

				/*
					if (Map == 0) {
						if ((main.x >= 2100 && main.y >= 1680 && main.x <= 2160 && main.y <= 1730) || (main.x >= 2720 && main.y >= 1690 && main.x <= 2770 && main.y < 1740) || (main.x >= 2670 && main.y >= 1930 && main.x <= 2710 && main.y <= 1980)
							|| (main.x >= 2970 && main.y >= 2100 && main.x <= 3000 && main.y <= 2130) || (main.x >= 2500 && main.y >= 2490 && main.x <= 2540 && main.y <= 2530) || (main.x >= 1790 && main.y >= 2170 && main.x <= 1840 && main.y <= 2210)) {
							Talk = true;
							people.button = true;
							//people.count = rand() % 12;
							people.count = people.count++;

							if (people.count == 14)
								people.count = 0;

							people.mod = 1;
						}
						else if (main.x >= 3360 && main.y >= 2060 && main.x <= 3400 && main.y <= 2100 && fisher.count != 3) {
							if (fisher.count != 2) {
								Talk = true;
								fisher.button = true;
							}
							else if (fisher.count == 2 && fisher.mod == 1) {
								Talk = true;
								fisher.button = true;
							}
						}
						else if (main.actmod == 0 && fish.mod == 0 && Map == 0) {
							if ((main.x > 2880 && main.x < 3420 && (main.y == 2130 || main.y == 2070)) || (main.x == 3420 && main.y > 2090 && main.y < 2140)) {
								main.actmod = 1;
								main.count = 0;
								PlaySound("Resource\\sound\\catch.wav", NULL, SND_ASYNC);
							}
							if (main.x > 2880 && main.x < 3420 && main.y == 2130)
								main.mod = 0;
							if (main.x > 2880 && main.x < 3420 && main.y == 2070)
								main.mod = 2;
							if (main.x == 3420 && main.y > 2090 && main.y < 2140)
								main.mod = 1;
						}

						if (main.x >= 2160 && main.y >= 2330 && main.x <= 2200 && main.y <= 2370 && chief.count != 9) {
							if (chief.count < 7)
							{
								Talk = true;
								chief.button = true;
							}
							else if (HELLO_count > HELLO_LIMIT)
							{
								Talk = true;
								chief.button = true;
							}
						}
						if (main.x >= 1980 && main.y >= 1450 && main.x <= 2000 && main.y <= 1490 && house.count != 4) {
							if (house.count != 3) {
								Talk = true;
								house.button = true;
							}
							else if (house.count == 3 && shop.mod == 1) {
								Talk = true;
								house.button = true;
							}
						}
						if (main.x >= 2400 && main.y >= 1480 && main.x <= 2430 && main.y <= 1520 && kid.count != 3) {
							if (kid.count != 2) {
								Talk = true;
								kid.button = true;
							}
							else if (kid.count == 2 && kid.mod == 1) {
								Talk = true;
								kid.button = true;
							}
						}

						if (main.x >= 2020 && main.y >= 2570 && main.x <= 2070 && main.y <= 2600 && shop.count != 3) {
							if (shop.count != 2) {
								Talk = true;
								shop.button = true;
							}
							else if (shop.count == 2 && shop.mod == 1) {
								Talk = true;
								shop.button = true;
							}
						}
						if (main.x >= 990 && main.y >= 1050 && main.x <= 1040 && main.y <= 1080 && soldier.count != 4) {
							if (soldier.count != 3) {
								Talk = true;
								soldier.button = true;
							}
							else if (soldier.count == 3 && soldier.mod == 1) {
								Talk = true;
								soldier.button = true;
							}
						}
						if (main.x >= 1390 && main.y >= 2820 && main.x <= 1440 && main.y <= 2860 && timberman.count != 3) {
							if (timberman.count != 2) {
								Talk = true;
								timberman.button = true;
							}
							else if (timberman.count == 2 && timberman.mod == 1) {
								Talk = true;
								timberman.button = true;
							}
						}

						if (main.x >= 224 && main.x <= 2310 && main.y == 2020) {
							if (HELLO_count > HELLO_LIMIT)
							{
								Help_count = 8;
								main.x = 745;
								main.y = 780;
								main.count = 0;
								main.mod = 0;
								main.speed = 10;
								main.atk = false;
								main.HP = HELLO_count;
								pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[0], false, &pChannel[0]);
							}
						}
						if (main.x == 200 && main.y > 850 && main.y < 900) {
							if (HELLO[0]) {
								if (HELLO_count < 10)
								{
									HELLO_count++;
								}
								PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
								HELLO[0] = false;
							}
						}
						if (main.x == 2470 && main.y > 1250 && main.y < 1310) {
							if (HELLO[1]) {
								if (HELLO_count < 10)
								{
									HELLO_count++;
								}
								PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
								HELLO[1] = false;
							}
						}
						if (main.x == 2190 && main.y == 3020) {
							if (HELLO[2]) {
								if (HELLO_count < 10)
								{
									HELLO_count++;
								}
								PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
								HELLO[2] = false;
							}
						}
						if (!Fruits.atk) {
							if (main.x >= 2070 && main.y >= 2740 && main.x <= 2100 && main.y < 2790)
							{
								Fruits.atk = true;
								Fruits.actmod = 2;
								Fruits.count = rand() % 3;
								Fruits.mod = rand() % 10;
							}
						}
					}
					else if (Map == 1) {
						if (main.x >= 1370 && main.y >= 960 && main.x <= 1430 && main.y <= 1040)
						{
							if (HELLO[3]) {
								soldier.mod = 1;
								main.x = 1050;
								main.y = 1060;
								//main.y = 1110;
								main.count = 0;
								main.mod = 0;// 위쪽바라보는 모습
								Map = 0;
								pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[2], false, &pChannel[0]);
								PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
								HELLO[3] = false;
							}
						}
					}
					*/
				}
				else if (wParam == VK_SPACE) {
					/*
					if (fish.mod == 1 && fish.button == false && main.actmod == 1) {
						fish.y += 10;
						if (fish.y >= rect.bottom - 170)
						{
							fish.mod = 0;
							fish.y = 0;
							main.actmod = 0;
							fish.count = 0;
							fisher.mod = 1;
							PlaySound("Resource\\sound\\Fish.wav", NULL, SND_ASYNC);
						}
						fish.button = true;
					}
					else if (main.actmod == 2) {
						if (timber.y > 265 && timber.y < 345)
						{
							timber.count++;
							timber.button = true;
							PlaySound("Resource\\sound\\Chopping.wav", NULL, SND_ASYNC);
						}
						else {
							if (timber.count != 0)
								timber.count--;
						}
						if (timber.count == 5)
						{
							main.actmod = 0;
							main.count = 0;
							timber.y = 0;
							timber.count = 0;
							timber.mod = false;
							timberman.mod = 1;
							PlaySound("Resource\\sound\\TreeFall.wav", NULL, SND_ASYNC);
						}
					}
					else if (Fruits.atk) {
						if (main.x == 2070 && main.y >= 2750 && main.y <= 2780) {
							Fruits.select = Fruits.count;
							PlaySound("Resource\\sound\\상자오픈.wav", NULL, SND_ASYNC);
						}
						if (Fruits.select != -1) {
							if (main.x == 2100 && main.y >= 2760 && main.y <= 2780) {
								if (Fruits.select == (Fruits.count + Fruits.mod) % 3) {
									Fruits.select = -1;
									Fruits.score++;
									Fruits.actmod = 2;
									Fruits.count = rand() % 3;
									Fruits.mod = rand() % 10;
									if (Fruits.score > 4) {
										Fruits.atk = false;
										Fruits.score = 0;
										shop.mod = 1;
									}
									PlaySound("Resource\\sound\\상자오픈.wav", NULL, SND_ASYNC);
								}
								else {
									if (Fruits.score != 0)
										Fruits.score--;
									PlaySound("Resource\\sound\\No.wav", NULL, SND_ASYNC);
								}
							}
							if (main.x == 2100 && main.y >= 2710 && main.y <= 2740) {
								if (Fruits.select == (Fruits.count + Fruits.mod + 1) % 3) {
									Fruits.select = -1;
									Fruits.score++;
									Fruits.actmod = 2;
									Fruits.count = rand() % 3;
									Fruits.mod = rand() % 10;
									if (Fruits.score > 4) {
										Fruits.atk = false;
										Fruits.score = 0;
										shop.mod = 1;
									}
									PlaySound("Resource\\sound\\상자오픈.wav", NULL, SND_ASYNC);
								}
								else {
									if (Fruits.score != 0)
										Fruits.score--;
									PlaySound("Resource\\sound\\No.wav", NULL, SND_ASYNC);
								}
							}
							if (main.x == 2100 && main.y >= 2800 && main.y <= 2820) {
								if (Fruits.select == (Fruits.count + Fruits.mod + 2) % 3) {
									Fruits.select = -1;
									Fruits.score++;
									Fruits.actmod = 2;
									Fruits.count = rand() % 3;
									Fruits.mod = rand() % 10;
									if (Fruits.score == 6) {
										Fruits.atk = false;
										Fruits.score = 0;
										shop.mod = 1;
									}
									PlaySound("Resource\\sound\\상자오픈.wav", NULL, SND_ASYNC);
								}
								else {
									if (Fruits.score != 0)
										Fruits.score--;
									PlaySound("Resource\\sound\\No.wav", NULL, SND_ASYNC);
								}
							}
						}
					}
					*/
				}
				/*
				else if (wParam == 'm' || wParam == 'M') {
					if (Map == 0)
						onoff_minimap = 1;
				}
				*/
			}
			KeyBuffer[wParam] = true;
		}
		InvalidateRgn(hwnd, NULL, false);
		break;
	case WM_KEYUP:
		if (GAME_MODE == 1)
		{
			KeyBuffer[wParam] = false;
			//if (main.walk == true)
			//	main.count = 2;
			//fish.button = false;
			//main.walk = false;

			//if (wParam == 'm' || wParam == 'M') {
			//	onoff_minimap = 0;
			//}

			InvalidateRgn(hwnd, NULL, false);
		}
		break;
	case WM_LBUTTONDOWN:
		if (Map == 2 && GAME_MODE == 1) {
			my = HIWORD(lParam);
			mx = LOWORD(lParam);
			mainAttak(mx, my);
			InvalidateRect(hwnd, NULL, false);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}
