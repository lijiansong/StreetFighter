#include <Windows.h>

// struct DecoderUnit
// {
// 	UINT message;
// 	LONG (*func)(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam );
// };
#define dim(x) (sizeof(x)/sizeof(x[0]))

/*LONG OnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);*/
/*LONG OnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);*/
BOOL Game_Init(HWND hwnd);//在此函数中进行资源的初始化
VOID Game_Paint( HWND hwnd);//在此函数中进行绘图代码的书写
BOOL Game_CleanUp(HWND hwnd );//在此函数中进行资源的清理
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//窗口处理过程函数