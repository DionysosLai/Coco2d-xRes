#ifndef __CC_APPLICATION_WIN32_H__  
  
#define __CC_APPLICATION_WIN32_H__  
  
//Windows头文件  
  
#include <Windows.h>   
  
//Cocos2d-x公共头文件,声明了一些公共函数以及语言类型枚举ccLanguageType  
  
#include "CCCommon.h"  
  
//使用cocos2d的命名空间来包括后面的代码  
  
NS_CC_BEGIN;  
  
//声明一下CCRect类，在CCApplication中会用到CCRect类指针  
  
class CCRect;  
  
class CC_DLL CCApplication  
  
{  
  
public:  
  
//构造  
  
CCApplication();  
  
//析构  
  
    virtual ~CCApplication();  
  
    //初始化  
  
    virtual bool initInstance() = 0;  
  
//程序启动后调用的函数，在这里创建设备与场景  
  
    virtual bool applicationDidFinishLaunching() = 0;  
  
//当程序转入后台，如电话打入时调用  
  
    virtual void applicationDidEnterBackground() = 0;  
  
//当程序转入前台，再次恢复时调用  
  
    virtual void applicationWillEnterForeground() = 0;  
  
//在“设备”设置FPS时调用的函数，设置帧间隔时间  
  
void setAnimationInterval(double interval);  
  
//声明一个枚举，列出当前设备的摆放方向  
  
    typedef enum  
  
    {  
  
        /// 垂直方向, home 键在下面  
  
        kOrientationPortrait = 0,  
  
        /// 垂直方向, home 键在上面  
  
        kOrientationPortraitUpsideDown = 1,  
  
        /// 水平方向，home键在右边  
  
        kOrientationLandscapeLeft = 2,  
  
        /// 水平方向，home健在左边  
  
        kOrientationLandscapeRight = 3,  
  
    } Orientation;  
  
      
  
//在“设备”改变了摆放方向后调用的函数,设置设备摆放方向  
  
    Orientation setOrientation(Orientation orientation);  
  
//取得窗口的状态栏所在的矩形位置  
  
    void    statusBarFrame(CCRect * rect);  
  
    //运行程序  
  
    int run();  
  
//取得当前的程序实例，这种用法可参考C++模式设计中的“单件”模式  
  
    static CCApplication& sharedApplication();  
  
//取得当前的语言类型  
  
    static ccLanguageType getCurrentLanguage();  
  
protected:  
  
//程序实例句柄  
  
HINSTANCE           m_hInstance;  
  
//加速键句柄  
  
HACCEL              m_hAccelTable;  
  
//声明为帧间隔，实际上是每两帧之间的频率次数  
  
    LARGE_INTEGER       m_nAnimationInterval;  
  
//单件的程序实例指针  
  
    static CCApplication * sm_pSharedApplication;  
  
};  
  
NS_CC_END;  
  
#endif// __CC_APPLICATION_WIN32_H__