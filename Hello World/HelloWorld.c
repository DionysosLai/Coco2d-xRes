//加入main.h头文件  
  
#include "main.h"  
  
//加入使用的AppDelegate类头文件  
  
#include "../Classes/AppDelegate.h"  
  
//WinMain主函数  
  
int APIENTRY _tWinMain(HINSTANCE hInstance,  
  
   HINSTANCE hPrevInstance,  
  
   LPTSTR    lpCmdLine,  
  
   int       nCmdShow)  
  
{  
  
//UNREFERENCED_PARAMETER用于在VC编译器下告知编译器，不必检测改变量是否使用的警告。  
  
UNREFERENCED_PARAMETER(hPrevInstance);  
  
UNREFERENCED_PARAMETER(lpCmdLine);  
  
    // 创建一个Cocos2d-x程序实例  
  
AppDelegate app;  
  
    // 运行程序实例  
  
    return cocos2d::CCApplication::sharedApplication().run();  
  
}  