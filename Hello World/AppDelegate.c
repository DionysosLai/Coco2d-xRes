#include "AppDelegate.h"  
//加入cocos2d头文件  
#include "cocos2d.h"  
#include "HelloWorldScene.h"  
//加入OpenGL窗口类  
#include "CCEGLView.h"  
//使用cocos2d命名空间  
USING_NS_CC;  
//构造函数  
AppDelegate::AppDelegate() {  
}  
//析构函数  
AppDelegate::~AppDelegate() {  
}  
//初始化函数,解答伏笔1  
bool AppDelegate::initInstance()   
{  
	//定义一个bool返回值用于判断是否正常初始化  
	bool bRet = false;  
	do {  
		//通过对宏定义的匹配来判断当前编译的代码的目标平台是什么，在这里可以知道Cocos2d-x的跨平台都支持哪些平台。我们的教程只针对WIN32平台部分做讲解，其它平台大家可以自行参考WIN32平台部分进行学习。  
		//第一种平台类型，WIN32系统  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		// 创建并初始化OpenGL窗口管理实例对象，注意这里使用的是new来动态实例化的。当程序退出时会通过显示设备的release函数进行对窗口的释放。【伏笔3后面会有讲解】。  
		CCEGLView * pMainWnd = new CCEGLView();  
		//CC_BREAK_IF宏的意思是如果括号中的语句为否则中断循环。配合do_while流程使用。  
		//可以看到这里的意思是，如果pMainWnd实例化失败或者pMainWnd创建窗口失败则中断循环。在这里硬性的设定了窗口的标题和窗口的宽度高度。  
		CC_BREAK_IF(! pMainWnd  
		|| ! pMainWnd->Create(TEXT("cocos2d: Hello World"), 480, 320));  
		#endif  // CC_PLATFORM_WIN32  
		   //第二种平台类型，IOS类型  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
		#endif  // CC_PLATFORM_IOS  
		//第三种平台类型，ANDORID类型  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		#endif  // CC_PLATFORM_ANDROID  
			//第四种平台，WOPHONE平台  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)  
		#endif  // CC_PLATFORM_WOPHONE  
		//第五种平台，MARMALADE  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)  
		#endif  
		//第六种平台，LINUX  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)  
		#endif  // CC_PLATFORM_LINUX  
		//第七种平台，BADA  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_BADA)  
		#endif  // CC_PLATFORM_BADA  
		//第八种平台,QNX  
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_QNX)  
		#endif // CC_PLATFORM_QNX  
		//如果没有被中断，则成功完成初始化。  
		bRet = true;  
		//退出循环  
	} while (0);  
	return bRet;  
}  
    //重载应用程序启动后调用的处理函数  
bool AppDelegate::applicationDidFinishLaunching() {  
	//通过CCDirector的静态函数sharedDirector来获取单件显示设备管理器指针  
	CCDirector *pDirector = CCDirector::sharedDirector();  
	//通过CCEGLView的静态函数sharedOpenGLView来获取单件管理实例对象并将其地址通过CCDirector的成员函数setOpenGLView传递给显示设备管理器。  
	pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());  
	//打开使用高清模式，这里给屏蔽了，如未来有需要可以打开。但是在设计程序时需要考虑到打开后对于位置的影响并提前制定相应的设计方案。  
	//pDirector->enableRetinaDisplay(true);  
	// 打开显示FPS  
	pDirector->setDisplayFPS(true);  
	// 设置当前屏幕的摆放方向，这里给屏蔽了。  
	// pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);  
	// 设置FPS的帧间隔时间差为60分之一秒，从而期望FPS为60帧。  
	pDirector->setAnimationInterval(1.0 / 60);  
	//通过HelloWorld的静态函数scene()创建返回一个场景实例  
	CCScene *pScene = HelloWorld::scene();  
	//运行这个场景  
	pDirector->runWithScene(pScene);  
	return true;  
}  
//重载应用程序转入后台时调用的函数，如电话打进来  
 void AppDelegate::applicationDidEnterBackground() {  
	//暂停显示设备的渲染处理  
	CCDirector::sharedDirector()->pause();  
	// 如果使用了声音引擎，这里进行暂停设置。否则会影响到通话，因为暂时没用到，所以屏蔽  
	// SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();  
}  
//重载应用程序恢复前台时调用的函数  
void AppDelegate::applicationWillEnterForeground() {  
	//恢复显示设备的渲染处理  
	CCDirector::sharedDirector()->resume();  
	//如果使用了声音引擎，这里进行恢复设置  
	// SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();  
}  