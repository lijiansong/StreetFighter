
#include "Header.h"
#include <wchar.h>
#include "resource.h"
#pragma comment(lib,"Coredll.lib")
#define _CRT_SECURE_NO_DEPRECATE
#define CHARACTER_MAX_STRENGTH 100//��CHARACTER_MAX_STRENGTH��ʶ�������ֵ����Ϊ100

//����ö��
enum ActionType
{
	ACTION_TYPE_BOXING=0,//ȭ��
	ACTION_TYPE_KICK=1,//����
	ACTION_TYPE_WAVEBOXING=2,//����
	ACTION_TYPE_NULL=3,//�޹�������
};
//����ʱ�������ӵ�
struct WaveBullets 
{
	int x,y;//��������
	bool exist;
};
//ѩ������ϵͳ
struct Snow 
{
	int x,y;//ѩ����λ������
	BOOL exist;
};

//����ʵ��ȫ�ֱ���
HINSTANCE g_hInst=NULL;
//������˼�������DC
HDC g_hdc=NULL,g_hdcMem=NULL,g_bufdc=NULL;

/************************************************************************/
/* Hero����ر���                                                       */
/************************************************************************/
/*
g_hHeroDirection��ʾHero���ƶ����򣬷�Ϊwait,go back,go forward���֣�
g_hHeroHit�洢Hero�ĳ���ͼ����Ϊȭ�򡢽��ߺ;������֣�
g_hHeroWave�洢Hero��������ʱ�Ƴ��Ĳ����ӵ���
g_hHeroBeAttactedHeavy�洢Hero���ػ���Ч��ͼ��
g_hHeroBeAttactedLight�洢Hero�������Ч��ͼ��
g_hHeroBlood�洢Hero��Ѫ��ͼ��
g_hHeroDie�洢Hero�����Ժ��Ч��ͼ,g_hHeroWin�洢Heroʤ���Ժ��Ч��ͼ
*/
HBITMAP g_hHeroDirection[3]={NULL},g_hHeroHit[3]={NULL},g_hHeroWave=NULL,g_hHeroBeAttactedHeavy=NULL,g_hHeroBeAttactedLight=NULL,g_hHeroBlood=NULL,g_hHeroDie=NULL,g_hHeroWin=NULL;
/*
g_iHeroFileNum��¼Hero����֡�ţ�
g_iHeroX,g_iHeroY�ֱ��¼Hero��ʵʱ����λ�ã�
g_iHeroDirection��¼����0,1,2�ֱ��ʾǰ�������ˡ�wait��������
g_iHeroStrength��¼Hero��Ѫ��������ֵ��
*/
int g_iHeroFileNum=0,g_iHeroX=0,g_iHeroY=0,g_iHeroDirection=0;
float g_iHeroStrength=0.0;
//Hero�ĳ�������
ActionType g_HeroActionType;
//heroWaves�洢Hero����Ĳ����ӵ�����Ϣ
WaveBullets heroWaves[10];
//g_iHeroWavesNum��¼Hero�Ĳ����ӵ�����Ŀ
int g_iHeroWavesNum=0;

/************************************************************************/
/* Robot����ر���                                                      */
/************************************************************************/
/*
g_hRobotDirection�洢Robot���ƶ�����Ҳ��Ϊwait,go back,go forward���֣�
g_hRobotHit�洢Robot�ĳ���ͼ��Ҳ��Ϊȭ�򡢽��ߺ;������֣�
g_hRobotWave�洢Robot��������ʱ�Ƴ��Ĳ����ӵ���
g_hRobotBeAttactedHeavy�洢Robot���ػ���Ч��ͼ��
g_hRobotBeAttactedLight�洢Robot�������Ч��ͼ��
g_hRobotBlood�洢Robot��Ѫ��ͼ��
g_hRobotDie�洢Robot�����Ժ��Ч��ͼ
*/
HBITMAP g_hRobotDirection[3]={NULL},g_hRobotWave=NULL,g_hRobotHit[3]={NULL},g_hRobotBeAttactedHeavy=NULL,g_hRobotBeAttactedLight=NULL,g_hRobotBlood=NULL,g_hRobotDie=NULL;
/*
g_iRobotFileNum�洢RobotͼƬ֡�ţ�g_iRobotX��g_iRobotY��¼Robot��λ�ã�
g_iRobotDirection��¼Robot���ƶ�����0/1/2�ֱ��ʾǰ�������ˡ�wait��
g_iRobotStrength��¼Robot������ֵ
*/
int g_iRobotFileNum=0,g_iRobotX=0,g_iRobotY=0,g_iRobotDirection=0;
float g_iRobotStrength=0.0;
//Robot�ĳ�������
ActionType g_RobotActionType;

//robotWaves�洢Robot����Ĳ����ӵ�����Ϣ
WaveBullets robotWaves[10];
//g_iRobotWaveNum��¼Robot���ӵ�����Ŀ
int g_iRobotWaveNum=0;

/************************************************************************/
/* ��Ϸ������Դ����                                                     */
/************************************************************************/
/*
g_hLogoKO�洢Ѫ��ͼ�м��KO��־��
g_hBackGround�洢��Ϸ����ͼ��
g_hLoading�洢��Ϸ��ʼ���ص�Ч��ͼ��
g_hStart�洢��Ϸ�Ŀ�ʼ���棻
g_hFire�洢����ȼ�ջ��棻
g_hTryAgain�洢��Ϸ�������棻
g_hSnow�洢ѩ������ͼ
*/
HBITMAP g_hLogoKO=NULL,g_hBackGround=NULL,g_hLoading=NULL,g_hStart=NULL,g_hFire=NULL,g_hTryAgain=NULL,g_hSnow=NULL;;
//g_iFireFrameNum��¼����ͼ��֡��
int g_iFireFrameNum=0;
//�洢Hero����ͼ��¼�Ŀ�Ⱥ͸߶���Ϣ
BITMAP g_bmpHeroHit;
//�洢Robot����ͼ��¼�Ŀ�Ⱥ͸߶���Ϣ
BITMAP g_bmpRobotHit;
//�洢HeroѪ��λͼ��¼�Ŀ�Ⱥ͸߶���Ϣ
BITMAP g_bmpHeroBlood;
//�洢RobotѪ��ͼ��¼�Ŀ�Ⱥ͸߶���Ϣ
BITMAP g_bmpRobotBlood;
//Ѫ��ͼ�м��KO logoλͼ��Ϣ
BITMAP g_bmpKO;

/*
����������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣻
g_tNow��¼�˴�׼����ͼ��ʱ��
*/
DWORD g_tHeroPre=0,g_tHeroNow=0;
//SnowFlowers�洢ѩ��������Ϣ
Snow SnowFlowers[100];
//g_SnowNum��¼ѩ������Ŀ
int g_SnowNum=0;

/************************************************************************/
/* Flag��־����                                                         */
/************************************************************************/
/*
g_heroActionFlag��g_robotActionFlag
��ʶ������Ƿ���У����������߶��ͳ���������ͬ�Ķ�����
g_heroBeAttactedLightFlag��g_heroBeAttactedHeavyFlag��
g_robotBeAttactedLightFlag��g_robotBeAttactedHeavyFlag
���ڱ�ʶ�����Ƿ��յ��ػ����������
g_gameOverFlag��ʶ��Ϸ�Ƿ������
first_start���ڱ�ʶ��Ϸ�Ƿ��һ������
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
	//1.����һ��������
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

	
	//2.ע�ᴰ����
	if (! RegisterClass(&ws)) 
		return -1;
	//3.��������

	UINT sysWidth=GetSystemMetrics(SM_CXSCREEN);
	UINT sysHeight=GetSystemMetrics(SM_CYSCREEN);
	HWND hwnd = CreateWindow(TEXT("Hello"),TEXT("Street Fighter V1.0"),WS_VISIBLE | WS_BORDER | WS_SYSMENU /*| WS_MINIMIZEBOX*/ /*| WS_MAXIMIZEBOX*/ | WS_CAPTION,
		0,0,sysWidth,sysHeight,
		NULL,NULL,hInstance,NULL);

	//4.���´�������
	UpdateWindow(hwnd);
	ShowWindow(hwnd,nShowCmd);

	if (!Game_Init (hwnd)) 
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}

	MSG msg={0};
	//5.��ȡϵͳ��Ϣ
	while(msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tHeroNow=GetTickCount();//��ȡ��ǰϵͳʱ��
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
			//��ʼ��Ϸ
			//first_start=false;
			break;
		case VK_Q:
			//�˳���Ϸ
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
			
		//�������(���Է���WINCE�²�֧����ĸ)
		case VK_LEFT://����
			if(!g_gameOverFlag)//��Ϸδ����
			{
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
				g_robotBeAttactedLightFlag=false;
				g_robotBeAttactedHeavyFlag=false;
				g_iHeroX-=10;
				g_iHeroDirection=0;
				if (g_iHeroX<=0)
					g_iHeroX=0;
			
				g_iRobotDirection=1;//Hero������Robotǰ��
				if (g_iRobotX>(g_iHeroX+g_bmpHeroHit.bmWidth/6))//δ����Hero
				{
					g_iRobotX-=(10+rand()%5);//��10-15��������Hero����
				}
				else
				{
					g_iRobotX=(g_iHeroX+g_bmpHeroHit.bmWidth/6);
				}
			}
			else//��Ϸ����
			{
				g_iHeroDirection=2;
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
			}
			break;

		case VK_RIGHT://ǰ��
			if (!g_gameOverFlag)//��Ϸδ����
			{
				g_heroActionFlag=false;
				g_robotBeAttactedLightFlag=false;
				g_robotBeAttactedHeavyFlag=false;
				g_iHeroX+=10;
				g_iHeroDirection=1;
				if(g_iHeroX>=(g_iRobotX-g_bmpRobotHit.bmWidth/6))//Hero����Robot�����ٿ���
					g_iHeroX=g_iRobotX-g_bmpRobotHit.bmWidth/6;
				//g_RobotActionType=ACTION_TYPE_KICK;
			
				g_iRobotDirection=0;//Heroǰ����Robot����
				g_iRobotX+=(10+rand()%5);
				if(g_iRobotX>=(rect.right-116))
					g_iRobotX=rect.right-116;
			}
			else//��Ϸ����
			{
				g_iHeroDirection=2;
				g_heroActionFlag=false;
				g_HeroActionType=ACTION_TYPE_NULL;
			}
			break;

		//��ʽ����
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
					heroWaves[i].y=g_iHeroY+20;//��Ŵ��ڳ���λ�ã�������ʵ
					heroWaves[i].exist=true;
					g_iHeroWavesNum++;//������Ŀ�ۼ�
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
		g_iRobotDirection=2;//wait����״̬
		g_iHeroDirection=2;//wait�Ķ��������ڷ���״̬
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

/*
����Ļ��Ʋ��������弼��,��g_bufdc��ͼ��g_hdcMem���ٽ���͸��������
�ٽ����Ľ����ʾ�ڴ���g_hdc��
���ȶԣ�ʹ�������������Ч�ر��⶯������˸
*/
//��Ϸ��Դ��ʼ������
BOOL Game_Init( HWND hwnd )
{
	HBITMAP wndBmp;

	g_hdc=GetDC(hwnd);
	//�ȴ����ڴ�DC���ٴ���һ������DC
	g_hdcMem=CreateCompatibleDC(g_hdc);
	g_bufdc=CreateCompatibleDC(g_hdc);

	RECT wndRect;//�������
	GetClientRect(hwnd,&wndRect);

	//��һ���ʹ��ڴ�С��ͬ�Ŀյ�λͼ����
	wndBmp=CreateCompatibleBitmap(g_hdc,wndRect.right,wndRect.bottom);
	//���յ�λͼ�ŵ�g_hdcMem��
	SelectObject(g_hdcMem,wndBmp);

	//���ظ���λͼ��Դ
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

	//Hero��ͼ����ʼ����
	g_iHeroX=wndRect.right/4;
	g_iHeroY=wndRect.right/4;
	g_iHeroDirection=2;//wait״̬
	g_HeroActionType=ACTION_TYPE_NULL;
	g_heroActionFlag=false;
	g_heroBeAttactedHeavyFlag=false;
	g_heroBeAttactedLightFlag=false;
	g_iHeroStrength=100.0;//Hero����ֵ��ʼ��Ϊ100
	g_iHeroFileNum=0;

	g_iRobotFileNum=0;
	//Robot��ͼ����ʼ����
	g_iRobotX=wndRect.right/4*3;
	g_iRobotY=wndRect.right/4;
	g_iRobotDirection=2;
	g_RobotActionType=ACTION_TYPE_NULL;
	g_robotActionFlag=false;
	g_robotBeAttactedHeavyFlag=false;
	g_robotBeAttactedLightFlag=false;
	g_iRobotStrength=100.0;//Robot����ֵ��ʼ��Ϊ100

	g_gameOverFlag=false;

	BITMAP bmpLoading;
	GetObject(g_hStart,sizeof(bmpLoading),&bmpLoading);
	//���ؿ�ʼ����
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
//��Ϸ����Ļ���
VOID Game_Paint(HWND hwnd)
{
	RECT wndRect;
	BITMAP bmpBkGrd;
	//��ȡ����ͼƬ�Ĵ�С
	GetObject(g_hBackGround,sizeof(bmpBkGrd),&bmpBkGrd);
	//��ȡ���ڳߴ�
	GetClientRect(hwnd,&wndRect);
	
	BITMAP bmpHero;//heroλͼ��Ϣ63*93
	GetObject(g_hHeroDirection[0],sizeof(bmpHero),&bmpHero);
	BITMAP bmpRobot;//robotλͼ��Ϣ,63*93
	GetObject(g_hRobotDirection[0],sizeof(bmpRobot),&bmpRobot);
	//��ʼ����
	BITMAP bmpStart;
	GetObject(g_hStart,sizeof(bmpStart),&bmpStart);
	BITMAP bmpSnow;
	//��ȡѩ��λͼ�Ĵ�С
	GetObject(g_hSnow,sizeof(bmpSnow),&bmpSnow);
	BITMAP bmpFire;
	GetObject(g_hFire,sizeof(bmpFire),&bmpFire);
	BITMAP bmpHeroWin;
	GetObject(g_hHeroWin,sizeof(bmpHeroWin),&bmpHeroWin);
	BITMAP bmpTryAgain;
	GetObject(g_hTryAgain,sizeof(bmpTryAgain),&bmpTryAgain);

	//���ƿ�ʼ����
	//MessageBox(hwnd, L"�Ƿ�ʼ��Ϸ��", L"��Ϣ����", 0);
	
	//�����ϱ���ͼ,��������ڴ�DC g_hdcMem��
	SelectObject(g_bufdc,g_hBackGround);
	//����ķ�ʽȫ��������ͼ
	StretchBlt(g_hdcMem,
		0,0,
		wndRect.right,wndRect.bottom,
		g_bufdc,0,0,
		bmpBkGrd.bmWidth,bmpBkGrd.bmHeight,
		SRCCOPY);
	//����ȼ�ջ���
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

	//����ѩ������ϵͳ
	if (g_SnowNum<100)
	{
		SnowFlowers[g_SnowNum].x=rand()%wndRect.right;
		SnowFlowers[g_SnowNum].y=0;
		SnowFlowers[g_SnowNum].exist=true;
		g_SnowNum++;
	}
	//�ж������Ƿ���ڣ����������͸����ͼ����
	for (int i=0;i<100;i++)
	{
		if (SnowFlowers[i].exist)
		{
			//��������ͼ
			SelectObject(g_bufdc,g_hSnow);
			TransparentBlt(g_hdcMem,
				SnowFlowers[i].x,SnowFlowers[i].y,
				bmpSnow.bmWidth,bmpSnow.bmHeight,
				g_bufdc,
				0,0,
				bmpSnow.bmWidth,bmpSnow.bmHeight,
				RGB(0,0,0));
			//�������������ƶ������ƫ����
			if (rand()%2==0)
				SnowFlowers[i].x+=rand()%6;
			else
				SnowFlowers[i].x-=rand()%6;
			//�ݷ������������˶�
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
	//Hero��������ʽ���ƣ���ʽ��Ϊ�գ��Ҵ��ڹ���״̬
	if((g_HeroActionType!=ACTION_TYPE_NULL)&&g_heroActionFlag&&!g_gameOverFlag)
	{
		//��ȡ����״̬��λͼ��Ϣ
		BITMAP bmpHeroAction;
		GetObject(g_hHeroHit[0],sizeof(bmpHeroAction),&bmpHeroAction);
		
		if(g_HeroActionType==ACTION_TYPE_WAVEBOXING)
		{
			//������ʽ��ͼ
			SelectObject(g_bufdc,g_hHeroHit[g_HeroActionType]);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY,
				bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
				g_bufdc,
				g_iHeroFileNum*bmpHeroAction.bmWidth/6,0,
				bmpHeroAction.bmWidth/6,bmpHeroAction.bmHeight,
				RGB(255,255,255));
			//����Ч����ͼ
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
						//Hero����Ĳ�����RobotС��20�����ؾ���Ϊ�Ǵ���
						if((g_iRobotX-heroWaves[i].x)<20)
						{
							if(g_iRobotStrength>0)
							{
								g_robotBeAttactedHeavyFlag=true;
								//���5-15֮����˺�
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
						//û���е��ﴰ�ڱ�Ե�Զ���ʧ
						if (heroWaves[i].x>wndRect.right-bmpHeroWave.bmWidth)
						{
							g_iHeroWavesNum--;
							heroWaves[i].exist=false;
						}
					}
			}
		}
		else//��ͨ��ʽ��ͼ
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
				//λͼ���Ϊ116
				if((g_iRobotX-g_iHeroX)<=120)
				{
					g_robotBeAttactedLightFlag=true;
					if (g_iRobotStrength>0)
					{
						//���0-5֮����˺�
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
	//Hero�ܵ������
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

	//Hero��ͼ�������������������ͼ
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

	//Robot�ܵ������
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
	//Robot�ܵ������ػ�
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
	//Robot����
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
				//���5-10֮����˺�
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
	//Robot��ͼ
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

	//Ѫ��λͼ�м��KO logo��ͼ
	SelectObject(g_bufdc,g_hLogoKO);
	StretchBlt(g_hdcMem,
		wndRect.right/2-g_bmpKO.bmWidth/2,0,
		g_bmpKO.bmWidth,g_bmpKO.bmHeight,
		g_bufdc,
		0,0,
		g_bmpKO.bmWidth,g_bmpKO.bmHeight,
		SRCCOPY);
	//����HeroѪ��ͼ����������ֵʵʱ������ͼλ��
	SelectObject(g_bufdc,g_hHeroBlood);
	StretchBlt(g_hdcMem,
		(int)(wndRect.right/2*(float)(1.0-(float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH))),0,
		(int)((float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH)*wndRect.right/2)-g_bmpKO.bmWidth/2,g_bmpHeroBlood.bmHeight,
		g_bufdc,
		0,0,
		(int)(g_bmpHeroBlood.bmWidth*(float)(g_iHeroStrength/CHARACTER_MAX_STRENGTH)),g_bmpHeroBlood.bmHeight,
		SRCCOPY);
	//����RobotѪ��ͼ����������ֵʵʱ������ͼ��ʼλ��
	SelectObject(g_bufdc,g_hRobotBlood);
	StretchBlt(g_hdcMem,
		wndRect.right/2+g_bmpKO.bmWidth/2,0,
		(int)(wndRect.right/2*(float)(g_iRobotStrength/CHARACTER_MAX_STRENGTH)),g_bmpRobotBlood.bmHeight,
		g_bufdc,
		0,0,
		g_bmpRobotBlood.bmWidth,g_bmpRobotBlood.bmHeight,
		SRCCOPY);

	if(g_gameOverFlag)//��Ϸ����
	{
		if (g_iHeroStrength<=0)//Hero����
		{
			//Hero����
			SelectObject(g_bufdc,g_hHeroDie);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY+10,
				g_bmpHeroHit.bmWidth/6,g_bmpHeroHit.bmHeight,
				g_bufdc,
				g_iHeroFileNum*g_bmpHeroHit.bmWidth/6,0,
				g_bmpHeroHit.bmWidth/6,g_bmpHeroHit.bmHeight,
				RGB(255,255,255));
			//Robotվ��
			SelectObject(g_bufdc,g_hRobotDirection[g_iRobotDirection]);
			TransparentBlt(g_hdcMem,
				g_iRobotX,g_iRobotY,
				bmpRobot.bmWidth/6,bmpRobot.bmHeight,
				g_bufdc,
				g_iRobotFileNum*bmpRobot.bmWidth/6,0,
				bmpRobot.bmWidth/6,bmpRobot.bmHeight,
				RGB(255,255,255));
		}
		else if(g_iRobotStrength<=0)//Robot����
		{
			//robot����
			SelectObject(g_bufdc,g_hRobotDie);
			TransparentBlt(g_hdcMem,
				g_iRobotX,g_iRobotY+10,
				g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
				g_bufdc,
				g_iRobotFileNum*g_bmpRobotHit.bmWidth/6,0,
				g_bmpRobotHit.bmWidth/6,g_bmpRobotHit.bmHeight,
				RGB(255,255,255));
			//����Hero Win��ͼƬ
			SelectObject(g_bufdc,g_hHeroWin);
			TransparentBlt(g_hdcMem,
				g_iHeroX,g_iHeroY,
				bmpHeroWin.bmWidth/6,bmpHeroWin.bmHeight,
				g_bufdc,
				g_iHeroFileNum*bmpHeroWin.bmWidth/6,0,
				bmpHeroWin.bmWidth/6,bmpHeroWin.bmHeight,
				RGB(255,255,255));
		}
		//��Ϸ�����������
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
		//Hero�Ĺ���״̬��ʶȫ����Ϊfalse
		g_heroActionFlag=false;
		g_HeroActionType=ACTION_TYPE_NULL;

	}
	//���յ���ʾ
	BitBlt(g_hdc,
		0,0,
		wndRect.right,wndRect.bottom,
		g_hdcMem,
		0,0,SRCCOPY);

	//��¼�˴λ�ͼʱ�䣬���´���Ϸѭ�����ж��Ƿ��Ѿ��ﵽ������²����趨��ʱ����
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
//��Ϸ��Դ�ͷ�
BOOL Game_CleanUp(HWND hwnd )
{
	//�ͷŸ�����Դ���
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