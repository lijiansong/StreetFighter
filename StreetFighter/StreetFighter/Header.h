#include <Windows.h>

// struct DecoderUnit
// {
// 	UINT message;
// 	LONG (*func)(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam );
// };
#define dim(x) (sizeof(x)/sizeof(x[0]))

/*LONG OnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);*/
/*LONG OnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);*/
BOOL Game_Init(HWND hwnd);//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint( HWND hwnd);//�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd );//�ڴ˺����н�����Դ������
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//���ڴ�����̺���