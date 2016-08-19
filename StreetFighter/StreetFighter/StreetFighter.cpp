
#include "Header.h"
#include <wchar.h>
#include "resource.h"
#pragma comment(lib,"Coredll.lib")
#define _CRT_SECURE_NO_DEPRECATE
#define CHARACTER_MAX_STRENGTH 100//宏CHARACTER_MAX_STRENGTH标识最大体力值，设为100

//动作枚举
enum ActionType
{
	ACTION_TYPE_BOXING=0,//拳击
	ACTION_TYPE_KICK=1,//踢腿
	ACTION_TYPE_WAVEBOXING=2,//绝招
	ACTION_TYPE_NULL=3,//无攻击动作
};
//绝招时发出的子弹
struct WaveBullets 
{
	int x,y;//波的坐标
	bool exist;
};
//雪花粒子系统
struct Snow 
{
	int x,y;//雪花的位置坐标
	BOOL exist;
};

//窗口实例全局变量
HINSTANCE g_hInst=NULL;
//三缓冲思想的三个DC
HDC g_hdc=NULL,g_hdcMem=NULL,g_bufdc=NULL;

/************************************************************************/
/* Hero的相关变量                                                       */
/************************************************************************/
/*
g_hHeroDirection表示Hero的移动方向，分为wait,go back,go forward三种；
g_hHeroHit存储Hero的出招图，分为拳打、脚踢和绝招三种；
g_hHeroWave存储Hero发出绝招时推出的波形子弹；
g_hHeroBeAttactedHeavy存储Hero被重击的效果图；
g_hHeroBeAttactedLight存储Hero被轻击的效果图；
g_hHeroBlood存储Hero的血量图；
g_hHeroDie存储Hero阵亡以后的效果图,g_hHeroWin存储Hero胜利以后的效果图
*/
HBITMAP g_hHeroDirection[3]={NULL},g_hHeroHit[3]={NULL},g_hHeroWave=NULL,g_hHeroBeAttactedHeavy=NULL,g_hHeroBeAttactedLight=NULL,g_hHeroBlood=NULL,g_hHeroDie=NULL,g_hHeroWin=NULL;
/*
g_iHeroFileNum记录Hero画面帧号；
g_iHeroX,g_iHeroY分别记录Hero的实时坐标位置；
g_iHeroDirection记录方向，0,1,2分别表示前进、后退、wait三个方向
g_iHeroStrength记录Hero的血量（体力值）
*/
int g_iHeroFileNum=0,g_iHeroX=0,g_iHeroY=0,g_iHeroDirection=0;
float g_iHeroStrength=0.0;
//Hero的出招类型
ActionType g_HeroActionType;
//heroWaves存储Hero打出的波形子弹的信息
WaveBullets heroWaves[10];
//g_iHeroWavesNum记录Hero的波形子弹的数目
int g_iHeroWavesNum=0;

/************************************************************************/
/* Robot的相关变量                                                      */
/************************************************************************/
/*
g_hRobotDirection存储Robot的移动方向，也分为wait,go back,go forward三种；
g_hRobotHit存储Robot的出招图，也分为拳打、脚踢和绝招三种；
g_hRobotWave存储Robot发出绝招时推出的波形子弹；
g_hRobotBeAttactedHeavy存储Robot被重击的效果图；
g_hRobotBeAttactedLight存储Robot被轻击的效果图；
g_hRobotBlood存储Robot的血量图；
g_hRobotDie存储Robot阵亡以后的效果图
*/
HBITMAP g_hRobotDirection[3]={NULL},g_hRobotWave=NULL,g_hRobotHit[3]={NULL},g_hRobotBeAttactedHeavy=NULL,g_hRobotBeAttactedLight=NULL,g_hRobotBlood=NULL,g_hRobotDie=NULL;
/*
g_iRobotFileNum存储Robot图片帧号，g_iRobotX、g_iRobotY记录Robot的位置；
g_iRobotDirection记录Robot的移动方向，0/1/2分别表示前进、后退、wait；
g_iRobotStrength记录Robot的体力值
*/
int g_iRobotFileNum=0,g_iRobotX=0,g_iRobotY=0,g_iRobotDirection=0;
float g_iRobotStrength=0.0;
//Robot的出招类型
ActionType g_RobotActionType;

//robotWaves存储Robot打出的波形子弹的信息
WaveBullets robotWaves[10];
//g_iRobotWaveNum记录Robot的子弹的数目
int g_iRobotWaveNum=0;

/************************************************************************/
/* 游戏公共资源部分                                                     */
/************************************************************************/
/*
g_hLogoKO存储血量图中间的KO标志；
g_hBackGround存储游戏背景图；
g_hLoading存储游戏初始加载的效果图；
g_hStart存储游戏的开始画面；
g_hFire存储火焰燃烧画面；
g_hTryAgain存储游戏结束画面；
g_hSnow存储雪花粒子图
*/
HBITMAP g_hLogoKO=NULL,g_hBackGround=NULL,g_hLoading=NULL,g_hStart=NULL,g_hFire=NULL,g_hTryAgain=NULL,g_hSnow=NULL;;
//g_iFireFrameNum记录火焰图的帧号
int g_iFireFrameNum=0;
//存储Hero出招图记录的宽度和高度信息
BITMAP g_bmpHeroHit;
//存储Robot出招图记录的宽度和高度信息
BITMAP g_bmpRobotHit;
//存储Hero血量位图记录的宽度和高度信息
BITMAP g_bmpHeroBlood;
//存储Robot血量图记录的宽度和高度信息
BITMAP g_bmpRobotBlood;
//血量图中间的KO logo位图信息
BITMAP g_bmpKO;

/*
声明两个变量来记录时间,g_tPre记录上一次绘图的时间；
g_tNow记录此次准备绘图的时间
*/
DWORD g_tHeroPre=0,g_tHeroNow=0;
//SnowFlowers存储雪花粒子信息
Snow SnowFlowers[100];
//g_SnowNum记录雪花的数目
int g_SnowNum=0;

/************************************************************************/
/* Flag标志部分                                                         */
/************************************************************************/
/*
g_heroActionFlag、g_robotActionFlag
标识人物的是否出招，用于区别走动和出招两个不同的动作；
g_heroBeAttactedLightFlag、g_heroBeAttactedHeavyFlag、
g_robotBeAttactedLightFlag、g_robotBeAttactedHeavyFlag
用于标识人物是否收到重击或者轻击；
g_gameOverFlag标识游戏是否结束；
first_start用于标识游戏是否第一次启动
*/
bool g_heroActionFlag;
bool g_robotActionFlag;
bool g_heroBeAttactedLightFlag;
bool g_heroBeAttactedHeavyFlag;
bool g_robotBeAttactedLightFlag;
bool g_robotBeAttactedHeavyFlag;
bool g_gameOverFlag;
bool first_start;

int WINAPI WinMain( __in HINSTANCE hInstance, 
				   __in_opt HINSTANCE hPrevInstance, 
				   __in_opt LPWSTR lpCmdLine,
				   __in int nShowCmd )
{
	//1.创建一个窗体类
	WNDCLASS ws;
	g_hInst=hInstance;

	ws.cbClsExtra	= 0;
	ws.cbWndExtra   = 0;
	ws.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	ws.hCursor		= NULL;
	ws.hIcon		= NULL;
	ws.hInstance	= hInstance;
	ws.lpfnWndProc	= WndProc;
	ws.lpszClassName = TEXT("Hello");
	ws.lpszMenuName  = NULL;
	ws.style		= CS_VREDRAW | CS_HREDRAW;

	
	//2.注册窗体类
	if (! RegisterClass(&ws)) 
		return -1;
	//3.创建窗体

	UINT sysWidth=GetSystemMetrics(SM_CXSCREEN);
	UINT sysHeight=GetSystemMetrics(SM_CYSCREEN);
	HWND hwnd = CreateWindow(TEXT("Hello"),TEXT("Street Fighter V1.0"),WS_VISIBLE | WS_BORDER | WS_SYSMENU /*| WS_MINIMIZEBOX*/ /*| WS_MAXIMIZEBOX*/ | WS_CAPTION,
		0,0,sysWidth,sysHeight,
		NULL,NULL,hInstance,NULL);

	//4.更新窗体内容
	UpdateWindow(hwnd);
	ShowWindow(hwnd,nShowCmd);

	if (!Game_Init (hwnd)) 
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0);
		return FALSE;
	}

	MSG msg={0};
	//5.获取系统消息
	while(msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tHeroNow=GetTickCount();//获取当前系统时间
			if ((g_tHeroNow-g_tHeroPre>=50)/*&&!first_start*/)
			{
				Game_Paint(hwnd);
			}
		}
	}
	return 1;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	RECT rect;
	GetClientRect(hwnd,&rect);
	

	switch (message)
	{
		// 	case WM_TIMER:
		// 		Game_Paint(hwnd);
		// 		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_P:
			//开始游戏
			//first_start=false;
			break;
		case VK_Q:
			//退出游戏
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
			
		//方向控制(测试发现WINCE下不支持字母)
		case VK_LEFT://后退
			if(!g_gameOverFlag)//游戏未结束
			{
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
				g_robotBeAttactedLightFlag=false;
				g_robotBeAttactedHeavyFlag=false;
				g_iHeroX-=10;
				g_iHeroDirection=0;
				if (g_iHeroX<=0)
					g_iHeroX=0;
			
				g_iRobotDirection=1;//Hero后退则Robot前进
				if (g_iRobotX>(g_iHeroX+g_bmpHeroHit.bmWidth/6))//未靠近Hero
				{
					g_iRobotX-=(10+rand()%5);//以10-15的像素向Hero靠近
				}
				else
				{
					g_iRobotX=(g_iHeroX+g_bmpHeroHit.bmWidth/6);
				}
			}
			else//游戏结束
			{
				g_iHeroDirection=2;
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
			}
			break;

		case VK_RIGHT://前进
			if (!g_gameOverFlag)//游戏未结束
			{
				g_heroActionFlag=false;
				g_robotBeAttactedLightFlag=false;
				g_robotBeAttactedHeavyFlag=false;
				g_iHeroX+=10;
				g_iHeroDirection=1;
				if(g_iHeroX>=(g_iRobotX-g_bmpRobotHit.bmWidth/6))//Hero靠近Robot则不能再靠近
					g_iHeroX=g_iRobotX-g_bmpRobotHit.bmWidth/6;
				//g_RobotActionType=ACTION_TYPE_KICK;
			
				g_iRobotDirection=0;//Hero前进则Robot回退
				g_iRobotX+=(10+rand()%5);
				if(g_iRobotX>=(rect.right-116))
					g_iRobotX=rect.right-116;
			}
			else//游戏结束
			{
				g_iHeroDirection=2;
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
			}
			break;

		//招式控制
		case VK_J:
			g_heroActionFlag=true;
			g_HeroActionType=ACTION_TYPE_BOXING;
			//g_RobotActionType=ACTION_TYPE_KICK;
			break;
		case VK_K:
			g_heroActionFlag=true;
			g_HeroActionType=ACTION_TYPE_KICK;
			//g_RobotActionType=ACTION_TYPE_BOXING;
			break;
		case VK_L:
			g_heroActionFlag=true;
			g_HeroActionType=ACTION_TYPE_WAVEBOXING;
			//g_robotBeAttactedHeavyFlag=true;
			for (int i=0;i<10;i++)
			{
				if (!heroWaves[i].exist)
				{
					heroWaves[i].x=g_iHeroX+50;
					heroWaves[i].y=g_iHeroY+20;//大概处于出手位置，更显真实
					heroWaves[i].exist=true;
					g_iHeroWavesNum++;//波的数目累加
					break;
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		Game_CleanUp(hwnd);	
		PostQuitMessage( 0 );
		break;
	default:
		//g_heroActionFlag=false;
		//g_robotActionFlag=true;
		g_iRobotDirection=2;//wait防守状态
		g_iHeroDirection=2;//wait的动作，处于防守状态
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

/*
人物的绘制采用三缓冲技术,从g_bufdc贴图到g_hdcMem，再进行透明化处理，
再将最后的结果显示在窗口g_hdc中
经比对，使用三缓冲可以有效地避免动画的闪烁
*/
//游戏资源初始化函数
BOOL Game_Init( HWND hwnd )
{
	HBITMAP wndBmp;

	g_hdc=GetDC(hwnd);
	//先创建内存DC，再创建一个缓冲DC
	g_hdcMem=CreateCompatibleDC(g_hdc);
	g_bufdc=CreateCompatibleDC(g_hdc);

	RECT wndRect;//窗体矩形
	GetClientRect(hwnd,&wndRect);

	//建一个和窗口大小相同的空的位图对象
	wndBmp=CreateCompatibleBitmap(g_hdc,wndRect.right,wndRect.bottom);
	//将空的位图放到g_hdcMem中
	SelectObject(g_hdcMem,wndBmp);

	//加载各种位图资源
	g_hBackGround=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BKGRD));
	//g_hBlood=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BLOOD));
	g_hHeroDirection[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_GOBACK));
	g_hHeroDirection[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_GOFORWARD));
	g_hHeroDirection[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_WAIT));
	g_hHeroHit[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_BOXING));
	g_hHeroHit[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_KICK));
	g_hHeroHit[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_WAVEBOXING));
	g_hHeroWave=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_WAVE));
	g_hHeroBeAttactedLight=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_BEATTACTED_LIGHT));
	g_hHeroBeAttactedHeavy=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_BEATTACTED_HEAVY));
	g_hHeroBlood=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_BLOOD));
	g_hHeroDie=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_DIE));
	g_hHeroWin=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R1_WIN));

	g_hRobotDirection[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_GOBACK));
	g_hRobotDirection[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_GOFORWARD));
	g_hRobotDirection[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_WAIT));
	
	g_hRobotHit[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_KICK));
	g_hRobotHit[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_BOXING));
	g_hRobotHit[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_WAVEBOXING));
	g_hRobotWave=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_WAVE));
	g_hRobotBeAttactedHeavy=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_BEATTACTED_HEAVY));
	g_hRobotBeAttactedLight=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_BEATTACTED_LIGHT));
	g_hRobotBlood=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_BLOOD));
	g_hRobotDie=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_R2_DIE));

	g_hLogoKO=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_KO));
	g_hLoading=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_LOADING));
	g_hStart=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_START));
	g_hSnow=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_SNOW));
	g_hFire=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_FIRE));
	g_hTryAgain=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_TRY_AGAIN));

	GetObject(g_hHeroHit[0],sizeof(g_bmpHeroHit),&g_bmpHeroHit);
	GetObject(g_hRobotHit[0],sizeof(g_bmpRobotHit),&g_bmpRobotHit);
	GetObject(g_hHeroBlood,sizeof(g_bmpHeroBlood),&g_bmpHeroBlood);
	GetObject(g_hRobotBlood,sizeof(g_bmpRobotBlood),&g_bmpRobotBlood);
	GetObject(g_hLogoKO,sizeof(g_bmpKO),&g_bmpKO);

	//Hero贴图的起始坐标
	g_iHeroX=wndRect.right/4;
	g_iHeroY=wndRect.right/4;
	g_iHeroDirection=2;//wait状态
	g_HeroActionType=ACTION_TYPE_NULL;
	g_heroActionFlag=false;
	g_heroBeAttactedHeavyFlag=false;
	g_heroBeAttactedLightFlag=false;
	g_iHeroStrength=100.0;//Hero体力值初始化为100
	g_iHeroFileNum=0;

	g_iRobotFileNum=0;
	//Robot贴图的起始坐标
	g_iRobotX=wndRect.right/4*3;
	g_iRobotY=wndRect.right/4;
	g_iRobotDirection=2;
	g_RobotActionType=ACTION_TYPE_NULL;
	g_robotActionFlag=false;
	g_robotBeAttactedHeavyFlag=false;
	g_robotBeAttactedLightFlag=false;
	g_iRobotStrength=100.0;//Robot体力值初始化为100

	g_gameOverFlag=false;

	BITMAP bmpLoading;
	GetObject(g_hStart,sizeof(bmpLoading),&bmpLoading);
	//加载开始画面
	first_start=true;
	HDC start_hdc=NULL;
	start_hdc=CreateCompatibleDC(g_hdc);
	if (first_start)
	{
		SelectObject(start_hdc,g_hStart);
		for (int i=0;i<bmpLoading.bmWidth;i++)
		{
			BitBlt(g_hdc,
				(wndRect.right-bmpLoading.bmWidth)/2,(wndRect.bottom-bmpLoading.bmHeight)/2+i,
				bmpLoading.bmWidth,1,
				start_hdc,0,i,SRCCOPY);
			Sleep(10);
		}
	}
	g_iFireFrameNum=0;
	Game_Paint(hwnd);
	return TRUE;
}
//游戏画面的绘制
VOID Game_Paint(HWND hwnd)
{
	RECT wndRect;
	BITMAP bmpBkGrd;
	//获取背景图片的大小
	GetObject(g_hBackGround,sizeof(bmpBkGrd),&bmpBkGrd);
	//获取窗口尺寸
	GetClientRect(hwnd,&wndRect);
	
	BITMAP bmpHero;//hero位图信息63*93
	GetObject(g_hHeroDirection[0],sizeof(bmpHero),&bmpHero);
	BITMAP bmpRobot;//robot位图信息,63*93
	GetObject(g_hRobotDirection[0],sizeof(bmpRobot),&bmpRobot);
	//开始界面
	BITMAP bmpStart;
	GetObject(g_hStart,sizeof(bmpStart),&bmpStart);
	BITMAP bmpSnow;
	//获取雪花位图的大小
	GetObject(g_hSnow,sizeof(bmpSnow),&bmpSnow);
	BITMAP bmpFire;
	GetObject(g_hFire,sizeof(bmpFire),&bmpFire);
	BITMAP bmpHeroWin;
	GetObject(g_hHeroWin,sizeof(bmpHeroWin),&bmpHeroWin);
	BITMAP bmpTryAgain;
	GetObject(g_hTryAgain,sizeof(bmpTryAgain),&bmpTryAgain);

	//绘制开始界面
	//MessageBox(hwnd, L"是否开始游戏？", L"消息窗口", 0);
	
	//先贴上背景图,将其存入内存DC g_hdcMem中
	SelectObject(g_bufdc,g_hBackGround);
	//拉伸的方式全屏贴背景图
	StretchBlt(g_hdcMem,
		0,0,
		wndRect.right,wndRect.bottom,
		g_bufdc,0,0,
		bmpBkGrd.bmWidth,bmpBkGrd.bmHeight,
		SRCCOPY);
	//火焰燃烧画面
	SelectObject(g_bufdc,g_hFire);
	TransparentBlt(g_hdcMem,
		0,wndRect.right/3,
		bmpFire.bmWidth/6,bmpFire.bmHeight,
		g_bufdc,
		g_iHeroFileNum*bmpFire.bmWidth/6,0,
		bmpFire.bmWidth/6,bmpFire.bmHeight,
		RGB(0,0,0));
	TransparentBlt(g_hdcMem,
		wndRect.right/20*19,wndRect.right/3,
		bmpFire.bmWidth/6,bmpFire.bmHeight,
		g_bufdc,
		(g_iHeroFileNum)*bmpFire.bmWidth/6,0,
		bmpFire.bmWidth/6,bmpFire.bmHeight,
		RGB(0,0,0));

	//创建雪花粒子系统
	if (g_SnowNum<100)
	{
		SnowFlowers[g_SnowNum].x=rand()%wndRect.right;
		SnowFlowers[g_SnowNum].y=0;
		SnowFlowers[g_SnowNum].exist=true;
		g_SnowNum++;
	}
	//判断粒子是否存在，存在则进行透明贴图操作
	for (int i=0;i<100;i++)
	{
		if (SnowFlowers[i].exist)
		{
			//贴上粒子图
			SelectObject(g_bufdc,g_hSnow);
			TransparentBlt(g_hdcMem,
				SnowFlowers[i].x,SnowFlowers[i].y,
				bmpSnow.bmWidth,bmpSnow.bmHeight,
				g_bufdc,
				0,0,
				bmpSnow.bmWidth,bmpSnow.bmHeight,
				RGB(0,0,0));
			//随机决定横向的移动方向和偏移量
			if (rand()%2==0)
				SnowFlowers[i].x+=rand()%6;
			else
				SnowFlowers[i].x-=rand()%6;
			//纵方向上做匀速运动
			SnowFlowers[i].y+=10;
			if (SnowFlowers[i].y>wndRect.bottom)
			{
				SnowFlowers[i].x=rand()%wndRect.right;
				SnowFlowers[i].y=0;
			}
		}
	}

	BITMAP bmpHeroWave;
	GetObject(g_hHeroWave,sizeof(bmpHeroWave),&bmpHeroWave);
	//Hero攻击，招式控制，招式不为空，且处于攻击状态
	if((g_HeroActionType!=ACTION_TYPE_NULL)&&g_heroActionFlag&&!g_gameOverFlag)
	{
		//获取攻击状态的位图信息
		BITMAP bmpHeroAction;
		GetObject(g_hHeroHit[0],sizeof(bmpHeroAction),&bmpHeroAction);
		
		if(g_HeroActionType==ACTION_TYPE_WAVEBOXING)
		{
			//人物招式贴图
			SelectObject(g_bufdc,g_hHeroHit[g_HeroActionType]);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY,
				bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
				g_bufdc,
				g_iHeroFileNum*bmpHeroAction.bmWidth/6,0,
				bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
				RGB(255,255,255));
			//绝招效果贴图
			SelectObject(g_bufdc,g_hHeroWave);
			if (g_iHeroWavesNum!=0)
			{
				for (int i=0;i<10;i++)
					if (heroWaves[i].exist)
					{
						TransparentBlt(g_hdcMem,
							heroWaves[i].x+45,heroWaves[i].y,
							bmpHeroWave.bmWidth,bmpHeroWave.bmHeight,
							g_bufdc,
							0,0,
							bmpHeroWave.bmWidth,bmpHeroWave.bmHeight,
							RGB(255,255,255));
						heroWaves[i].x+=20;
						//Hero打出的波距离Robot小于20个像素就认为是打中
						if((g_iRobotX-heroWaves[i].x)<20)
						{
							if(g_iRobotStrength>0)
							{
								g_robotBeAttactedHeavyFlag=true;
								//造成5-15之间的伤害
								float damage=5+(float)(rand()%10);
								g_iRobotStrength-=damage;
							}
							else
							{
								g_iRobotStrength=0;
								g_gameOverFlag=true;
							}
							g_RobotActionType=ACTION_TYPE_NULL;
							g_iHeroWavesNum--;
							heroWaves[i].exist=false;
						}
						//没打中到达窗口边缘自动消失
						if (heroWaves[i].x>wndRect.right-bmpHeroWave.bmWidth)
						{
							g_iHeroWavesNum--;
							heroWaves[i].exist=false;
						}
					}
			}
		}
		else//普通招式贴图
		{
			if(!g_gameOverFlag)
			{
				SelectObject(g_bufdc,g_hHeroHit[g_HeroActionType]);
				TransparentBlt(g_hdcMem,
					g_iHeroX,g_iHeroY,
					bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
					g_bufdc,
					g_iHeroFileNum*bmpHeroAction.bmWidth/6,0,
					bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
					RGB(255,255,255));
				//位图差距为116
				if((g_iRobotX-g_iHeroX)<=120)
				{
					g_robotBeAttactedLightFlag=true;
					if (g_iRobotStrength>0)
					{
						//造成0-5之间的伤害
						float damage=(float)(rand()%5);
						g_iRobotStrength-=damage;
						Sleep(10);
					}
					else
					{
						g_iRobotStrength=0;
						g_gameOverFlag=true;
					}
				}
			}
		}
		//g_heroActionFlag=false;
	}

	/*++++++++++++++++++++++++++*/
	//Hero受到轻击打
	if (!g_gameOverFlag&&g_heroBeAttactedLightFlag&&g_RobotActionType!=ACTION_TYPE_NULL)
	{
		g_heroActionFlag=false;
		SelectObject(g_bufdc,g_hHeroBeAttactedLight);
		TransparentBlt(
			g_hdcMem,
			g_iHeroX,g_iHeroY,
			bmpHero.bmWidth/6,bmpHero.bmHeight,
			g_bufdc,
			g_iHeroFileNum*bmpHero.bmWidth/6,0,
			bmpHero.bmWidth/6,bmpHero.bmHeight,
			RGB(255,255,255));
		g_heroBeAttactedLightFlag=false;
	}

	//Hero贴图，针对其各个方向进行贴图
	if (!g_gameOverFlag&&!g_heroActionFlag&&!g_heroBeAttactedLightFlag&&!g_heroBeAttactedHeavyFlag)
	{
		SelectObject(g_bufdc,g_hHeroDirection[g_iHeroDirection]);
		TransparentBlt(g_hdcMem,
			g_iHeroX,g_iHeroY,
			bmpHero.bmWidth/6,bmpHero.bmHeight,
			g_bufdc,
			g_iHeroFileNum*bmpHero.bmWidth/6,0,
			bmpHero.bmWidth/6,bmpHero.bmHeight,
			RGB(255,255,255));
	}

	//Robot受到轻击打
	if (!g_gameOverFlag&&g_robotBeAttactedLightFlag/*&&g_HeroActionType!=ACTION_TYPE_NULL*/)
	{
		g_robotActionFlag=false;
		SelectObject(g_bufdc,g_hRobotBeAttactedLight);
		TransparentBlt(
			g_hdcMem,
			g_iRobotX,g_iRobotY,
			bmpRobot.bmWidth/6,bmpRobot.bmHeight,
			g_bufdc,
			g_iRobotFileNum*bmpRobot.bmWidth/6,0,
			bmpRobot.bmWidth/6,bmpRobot.bmHeight,
			RGB(255,255,255));
		//g_robotBeAttactedLightFlag=false;
	}
	//Robot受到波的重击
	if(g_robotBeAttactedHeavyFlag)
	{
		g_robotActionFlag=false;
		SelectObject(g_bufdc,g_hRobotBeAttactedHeavy);
		TransparentBlt(g_hdcMem,
			g_iRobotX,g_iRobotY,
			g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
			g_bufdc,
			g_iRobotFileNum*g_bmpRobotHit.bmWidth/6,0,
			g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
			RGB(255,255,255));
		g_robotBeAttactedHeavyFlag=false;
	}
	//Sleep(500);
	//g_robotActionFlag=true;
	if ((!g_gameOverFlag)&&(!g_heroActionFlag)&&(g_HeroActionType==ACTION_TYPE_NULL))
	{
		g_robotActionFlag=true;
		//int probability=rand()%2;
		if(1==rand()%2)
		{
			g_RobotActionType=ACTION_TYPE_KICK;
		}
		else
		{
			g_RobotActionType=ACTION_TYPE_BOXING;
		}
	}
	//Robot攻击
	if(!g_gameOverFlag&&g_robotActionFlag/*&&(g_heroActionFlag==false)*/)
	{
		SelectObject(g_bufdc,g_hRobotHit[g_RobotActionType]);
		TransparentBlt(g_hdcMem,
			g_iRobotX,g_iRobotY,
			g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
			g_bufdc,
			g_iRobotFileNum*g_bmpRobotHit.bmWidth/6,0,
			g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
			RGB(255,255,255));

		if((g_iRobotX-g_iHeroX)<=110)
		{
			g_heroBeAttactedLightFlag=true;
			if (g_iHeroStrength>0)
			{
				//造成5-10之间的伤害
				float damage=(float)(5+rand()%5);
				g_iHeroStrength-=damage;
				//Sleep(10);
			}
			else
			{
				g_iHeroStrength=0;
				g_gameOverFlag=true;
			}
		}
	}
	//Robot贴图
	if (!g_gameOverFlag&&!g_robotActionFlag&&!g_robotBeAttactedLightFlag&&!g_robotBeAttactedHeavyFlag)
	{
		SelectObject(g_bufdc,g_hRobotDirection[g_iRobotDirection]);
		TransparentBlt(g_hdcMem,
			g_iRobotX,g_iRobotY,
			bmpRobot.bmWidth/6,bmpRobot.bmHeight,
			g_bufdc,
			g_iRobotFileNum*bmpRobot.bmWidth/6,0,
			bmpRobot.bmWidth/6,bmpRobot.bmHeight,
			RGB(255,255,255));
// 		g_robotBeAttactedLightFlag=false;
// 		g_robotBeAttactedHeavyFlag=false;
	}

	//血量位图中间的KO logo贴图
	SelectObject(g_bufdc,g_hLogoKO);
	StretchBlt(g_hdcMem,
		wndRect.right/2-g_bmpKO.bmWidth/2,0,
		g_bmpKO.bmWidth,g_bmpKO.bmHeight,
		g_bufdc,
		0,0,
		g_bmpKO.bmWidth,g_bmpKO.bmHeight,
		SRCCOPY);
	//贴上Hero血量图，根据体力值实时计算贴图位置
	SelectObject(g_bufdc,g_hHeroBlood);
	StretchBlt(g_hdcMem,
		(int)(wndRect.right/2*(float)(1.0-(float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH))),0,
		(int)((float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH)*wndRect.right/2)-g_bmpKO.bmWidth/2,g_bmpHeroBlood.bmHeight,
		g_bufdc,
		0,0,
		(int)(g_bmpHeroBlood.bmWidth*(float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH)),g_bmpHeroBlood.bmHeight,
		SRCCOPY);
	//贴上Robot血量图，根据体力值实时计算贴图起始位置
	SelectObject(g_bufdc,g_hRobotBlood);
	StretchBlt(g_hdcMem,
		wndRect.right/2+g_bmpKO.bmWidth/2,0,
		(int)(wndRect.right/2*(float)(g_iRobotStrength/CHARACTER_MAX_STRENGTH)),g_bmpRobotBlood.bmHeight,
		g_bufdc,
		0,0,
		g_bmpRobotBlood.bmWidth,g_bmpRobotBlood.bmHeight,
		SRCCOPY);

	if(g_gameOverFlag)//游戏结束
	{
		if (g_iHeroStrength<=0)//Hero阵亡
		{
			//Hero倒地
			SelectObject(g_bufdc,g_hHeroDie);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY+10,
				g_bmpHeroHit.bmWidth/6,g_bmpHeroHit.bmHeight,
				g_bufdc,
				g_iHeroFileNum*g_bmpHeroHit.bmWidth/6,0,
				g_bmpHeroHit.bmWidth/6,g_bmpHeroHit.bmHeight,
				RGB(255,255,255));
			//Robot站着
			SelectObject(g_bufdc,g_hRobotDirection[g_iRobotDirection]);
			TransparentBlt(g_hdcMem,
				g_iRobotX,g_iRobotY,
				bmpRobot.bmWidth/6,bmpRobot.bmHeight,
				g_bufdc,
				g_iRobotFileNum*bmpRobot.bmWidth/6,0,
				bmpRobot.bmWidth/6,bmpRobot.bmHeight,
				RGB(255,255,255));
		}
		else if(g_iRobotStrength<=0)//Robot阵亡
		{
			//robot倒地
			SelectObject(g_bufdc,g_hRobotDie);
			TransparentBlt(g_hdcMem,
				g_iRobotX,g_iRobotY+10,
				g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
				g_bufdc,
				g_iRobotFileNum*g_bmpRobotHit.bmWidth/6,0,
				g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
				RGB(255,255,255));
			//加载Hero Win的图片
			SelectObject(g_bufdc,g_hHeroWin);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY,
				bmpHeroWin.bmWidth/6,bmpHeroWin.bmHeight,
				g_bufdc,
				g_iHeroFileNum*bmpHeroWin.bmWidth/6,0,
				bmpHeroWin.bmWidth/6,bmpHeroWin.bmHeight,
				RGB(255,255,255));
		}
		//游戏结束画面加载
		SelectObject(g_bufdc,g_hTryAgain);
		BitBlt(g_hdcMem,
			wndRect.right/4,0,
			bmpTryAgain.bmWidth/2,bmpTryAgain.bmHeight,
			g_bufdc,
			bmpTryAgain.bmWidth/2,0,SRCAND);
		BitBlt(g_hdcMem,
			wndRect.right/4,0,
			bmpTryAgain.bmWidth/2,bmpTryAgain.bmHeight,
			g_bufdc,
			0,0,SRCPAINT);
		//Hero的攻击状态标识全部置为false
		g_heroActionFlag=false;
		g_HeroActionType=ACTION_TYPE_NULL;

	}
	//最终的显示
	BitBlt(g_hdc,
		0,0,
		wndRect.right,wndRect.bottom,
		g_hdcMem,
		0,0,SRCCOPY);

	//记录此次绘图时间，供下次游戏循环中判断是否已经达到画面更新操作设定的时间间隔
	g_tHeroPre = GetTickCount();
	g_iFireFrameNum++;
	g_iHeroFileNum++;
	g_iRobotFileNum++;
	if (g_iFireFrameNum==6)
	{
		g_iFireFrameNum=0;
	}
	if (g_iRobotFileNum==6)
		g_iRobotFileNum=0;

	if(g_iHeroFileNum==6)
		g_iHeroFileNum=0;
}
//游戏资源释放
BOOL Game_CleanUp(HWND hwnd )
{
	//释放各种资源句柄
	for(int i=0;i<3;i++)
	{
		DeleteObject(g_hHeroDirection[i]);
		DeleteObject(g_hHeroHit[i]);
		DeleteObject(g_hRobotHit[i]);
		DeleteObject(g_hRobotDirection[i]);
	}
	DeleteObject(g_hTryAgain);
	DeleteObject(g_hHeroWin);
	DeleteObject(g_hHeroDie);
	DeleteObject(g_hRobotDie);
	DeleteObject(g_hFire);
	DeleteObject(g_hSnow);
	DeleteObject(g_hStart);
	DeleteObject(g_hLoading);
	DeleteObject(g_hLogoKO);
	DeleteObject(g_hHeroBlood);
	DeleteObject(g_hRobotBlood);
	DeleteObject(g_hHeroBeAttactedHeavy);
	DeleteObject(g_hHeroBeAttactedLight);
	DeleteObject(g_hRobotBeAttactedLight);
	DeleteObject(g_hRobotBeAttactedHeavy);
	DeleteObject(g_hHeroWave);
	DeleteObject(g_hRobotWave);
	DeleteObject(g_bufdc);
	DeleteDC(g_hdcMem);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}