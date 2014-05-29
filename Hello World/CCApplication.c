#include "CCApplication.h"  
//设备头文件  
#include "CCDirector.h"  
//在注册表中写入对于PVRFrame的显示和隐藏的设置  
static void PVRFrameEnableControlWindow(bool bEnable);  
//使用cocos2d的命名空间来包括后面的代码  
NS_CC_BEGIN;  
// CCApplication的静态成员指针变量, 单件对象指针  
CCApplication * CCApplication::sm_pSharedApplication = 0;  
//构造函数  
CCApplication::CCApplication()  
: m_hInstance(NULL)  
, m_hAccelTable(NULL)  
{  
//获取当前程序句柄  
m_hInstance= GetModuleHandle(NULL);  
//初始化m_nAnimationInterval  
m_nAnimationInterval.QuadPart = 0;  
//断言程序中只有一个sm_pSharedApplication。确保当前类只有一个实例对象  
CC_ASSERT(! sm_pSharedApplication);  
//设置单件对象指针指向当前类对象实例  
    sm_pSharedApplication = this;  
}  
//析构  
CCApplication::~CCApplication()  
{  
//断言程序只有一个sm_pSharedApplication就是指向当前类的实例对象  
    CC_ASSERT(this == sm_pSharedApplication);  
    sm_pSharedApplication = NULL;  
}  
//程序运行  
int CCApplication::run()  
{  
    //设置注册表PVRFrame隐藏  
    PVRFrameEnableControlWindow(false);  
    //主消息循环  
    MSG msg;  
    LARGE_INTEGER nFreq;  
    LARGE_INTEGER nLast;  
    LARGE_INTEGER nNow;  
//WINDOWS高精度定时器的用法，先获取频率  
QueryPerformanceFrequency(&nFreq);  
//获取当前的计数值,即频率x当前时间  
    QueryPerformanceCounter(&nLast);  
	//initInstance函数为虚函数，由派生类AppDelegate进行了重载。此段代码在调用AppDelegate重载的initInstance函数之后调用applicationDidFinishLaunching函数完成一些初始化处理。  
	//注:AppDelegate重载initInstance函数做了什么我们暂且只先认为它如平时我们WINDOWS基本框架程序一样创建了一个Windows窗口。【伏笔1后面会有讲解】。  
    if (! initInstance() || ! applicationDidFinishLaunching())  
    {  
        return 0;  
    }  
//取得当前使用的OPENGL窗口管理实例对象  
CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();  
//将窗口居中显示  
    mainWnd.centerWindow();  
    ShowWindow(mainWnd.getHWnd(), SW_SHOW);  
//非常熟悉!进入WINDOWS消息循环  
    while (1)  
{  
   //如果没有获取到WINDOWS消息  
        if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  
        {  
            // 取得当前的计数值,即频率x当前时间  
            QueryPerformanceCounter(&nNow);  
            //m_nAnimationInterval.QuadPart的值 为setAnimationInterval函数进行设置的固定值。此处是为了判断时间流逝了多久，是否应该更新显示设备  
            if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)  
            {  
  //如果时间流逝达到了设定的FPS时间差，则更新计数值。  
                nLast.QuadPart = nNow.QuadPart;  
  //这里是设备渲染场景的函数，【伏笔2后面会有讲解】   
                CCDirector::sharedDirector()->mainLoop();  
            }  
            else  
            {  
  //sleep0秒的意义是让CPU做下时间片切换，防止死循环而使系统其它程序得不到响应。  
                Sleep(0);  
            }  
            continue;  
        }  
   //有消息获取到  
        if (WM_QUIT == msg.message)  
        {  
   // 如果获取的消息是退出则退出循环。  
            break;  
        }  
        // 如果没有定义加速键或者处理完加速键信息  
        if (! m_hAccelTable || ! TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))  
        {  
   //处理Windows消息  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
    return (int) msg.wParam;  
}  
   
//外部调用的设置帧间隔时间  
void CCApplication::setAnimationInterval(double interval)  
{  
    //获取高精度定时器的频率  
    LARGE_INTEGER nFreq;  
QueryPerformanceFrequency(&nFreq);  
//计算出频率X帧间隔时差的值存入m_nAnimationInterval用做比较值  
    m_nAnimationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);  
}  
//摆放方向变化时外部自动调用的设置摆放方向  
CCApplication::Orientation CCApplication::setOrientation(Orientation orientation)  
{  
    //切换OPENGL视窗的宽高  
    CCEGLView * pView = CCDirector::sharedDirector()->getOpenGLView();  
    if (pView)  
    {  
        return (Orientation)pView->setDeviceOrientation(orientation);  
    }  
    return (Orientation)CCDirector::sharedDirector()->getDeviceOrientation();  
}  
//获取状态栏的位置矩形  
void CCApplication::statusBarFrame(CCRect * rect)  
{  
    if (rect)  
    {  
        // WINDOWS系统没有状态栏，所以返回的矩形各位置都是0  
        *rect = CCRectMake(0, 0, 0, 0);  
    }  
}  
//////////////////////////////////////////////////////////////////////////  
// 静态成员函数，获取单件指针  
//////////////////////////////////////////////////////////////////////////  
CCApplication& CCApplication::sharedApplication()  
{  
    CC_ASSERT(sm_pSharedApplication);  
    return *sm_pSharedApplication;  
}  
//静态成员函数，获取当前系统的语言类型  
ccLanguageType CCApplication::getCurrentLanguage()  
{  
//默认为英语  
    ccLanguageType ret = kLanguageEnglish;  
//  
LCID localeID = GetUserDefaultLCID();  
unsigned short primaryLanguageID = localeID & 0xFF;  
switch (primaryLanguageID)  
{  
case LANG_CHINESE://中文  
ret = kLanguageChinese;  
break;  
case LANG_FRENCH://法文  
ret = kLanguageFrench;  
break;  
case LANG_ITALIAN://意文  
ret = kLanguageItalian;  
break;  
case LANG_GERMAN://德文  
ret = kLanguageGerman;  
break;  
case LANG_SPANISH://西班牙文  
ret = kLanguageSpanish;  
break;  
case LANG_RUSSIAN://俄文  
ret = kLanguageRussian;  
break;  
}  
    return ret;  
}  
NS_CC_END;  
//////////////////////////////////////////////////////////////////////////  
//在注册表中写入对于PVRFrame的显示和隐藏的设置  
//////////////////////////////////////////////////////////////////////////  
static void PVRFrameEnableControlWindow(bool bEnable)  
{  
    HKEY hKey = 0;  
    // 打开注册表的 PVRFrame 项  
    if(ERROR_SUCCESS != RegCreateKeyExW(HKEY_CURRENT_USER,  
        L"Software\\Imagination Technologies\\PVRVFRame\\STARTUP\\",  
        0,  
        0,  
        REG_OPTION_NON_VOLATILE,  
        KEY_ALL_ACCESS,  
        0,  
        &hKey,  
        NULL))  
    {  
        return;  
    }  
     
    const wchar_t * wszValue = L"hide_gui";  
    const wchar_t * wszNewData = (bEnable) ? L"NO" : L"YES";  
    wchar_t wszOldData[256] = {0};  
DWORD   dwSize = sizeof(wszOldData);  
//读取相应的键值  
LONGstatus = RegQueryValueExW(hKey, wszValue, 0, NULL, (LPBYTE)wszOldData, &dwSize);  
//如果键值不存在，或者键值存在但与当前值不同，重设键值  
    if (ERROR_FILE_NOT_FOUND == status                
        || (ERROR_SUCCESS == status                   
        && 0 != wcscmp(wszNewData, wszOldData)))      
{  
        dwSize = sizeof(wchar_t) * (wcslen(wszNewData) + 1);  
        RegSetValueEx(hKey, wszValue, 0, REG_SZ, (const BYTE *)wszNewData, dwSize);  
    }  
//关闭注册表  
    RegCloseKey(hKey);  
}  