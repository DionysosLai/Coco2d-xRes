#ifndef  _APP_DELEGATE_H_  
#define  _APP_DELEGATE_H_  
#include "CCApplication.h"  
class  AppDelegate : private cocos2d::CCApplication  
{  
public:  
//构造函数  
AppDelegate();  
//析构函数  
virtual ~AppDelegate();  
    //重载初始化函数  
    virtual bool initInstance();  
    //重载应用程序启动后调用的处理函数  
    virtual bool applicationDidFinishLaunching();  
    //重载应用程序转入后台时调用的函数  
    virtual void applicationDidEnterBackground();  
    //重载应用程序恢复前台时调用的函数  
    virtual void applicationWillEnterForeground();  
};  
#endif // _APP_DELEGATE_H_  