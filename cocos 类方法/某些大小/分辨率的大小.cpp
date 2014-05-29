分辨率：即是屏幕上图像的精细度，在Cocos2d-x中其大小为屏幕在横向和纵向可以容纳的逻辑点数量，
为了好理解，我们把它想像成投影机的分辨率。

影幕：想想小时候看电影时用到的那块白布吧，当然也许公司会议室里也能看到它。也就是投影机用
来投射画面的画布。

视口：其实就是上面说的投影机投影出来的影片画面所占的矩形。它如果大于影幕的大小，那么你就
不能看到完整的影片画面。如果小于影幕的大小。你就可以在它显示的区域里看到影片画面。


void CCEGLViewProtocol::setFrameSize(float width, float height)
{
	//这里使用参数对两个变量进行了赋值，第一个就是分辨率大小。第二个就是影幕的大小。
    m_obDesignResolutionSize = m_obScreenSize = CCSizeMake(width, height);
}

void CCEGLViewProtocol::setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy)
{
	//确保分辨率显示方式有效
    CCAssert(resolutionPolicy != kResolutionUnKnown, "should set resolutionPolicy");
    
	//如果参数无效，直接返回。
    if (width == 0.0f || height == 0.0f)
    {
        return;
    }
	//在这里对变量m_obDesignResolutionSize进行了设置。
    m_obDesignResolutionSize.setSize(width, height);
    //这里计算出影幕大小与分辨率大小的比率。存放在变量m_fScaleX,m_fScaleY中。
    m_fScaleX = (float)m_obScreenSize.width / m_obDesignResolutionSize.width;
    m_fScaleY = (float)m_obScreenSize.height / m_obDesignResolutionSize.height;
    
	//如果模式是kResolutionNoBorder，按照最大影幕方式，即保证像素不失真的情况下，影幕适应分辨率  
	//如果是低调整模式 
    if (resolutionPolicy == kResolutionNoBorder)
    {
		//缩放值按最大
        m_fScaleX = m_fScaleY = MAX(m_fScaleX, m_fScaleY);
    }
    
	//如果是高调整模式
    if (resolutionPolicy == kResolutionShowAll)
    {
		//缩放值按最小
        m_fScaleX = m_fScaleY = MIN(m_fScaleX, m_fScaleY);
    }

    if ( resolutionPolicy == kResolutionFixedHeight) {
    	m_fScaleX = m_fScaleY;
    	m_obDesignResolutionSize.width = ceilf(m_obScreenSize.width/m_fScaleX);
    }

    if ( resolutionPolicy == kResolutionFixedWidth) {
    	m_fScaleY = m_fScaleX;
    	m_obDesignResolutionSize.height = ceilf(m_obScreenSize.height/m_fScaleY);
    }

    // calculate the rect of viewport 根据缩放值和分辨率计算视口的宽高 
    float viewPortW = m_obDesignResolutionSize.width * m_fScaleX;
    float viewPortH = m_obDesignResolutionSize.height * m_fScaleY;
	// 设置视口位置居屏幕中间
    m_obViewPortRect.setRect((m_obScreenSize.width - viewPortW) / 2, (m_obScreenSize.height - viewPortH) / 2, viewPortW, viewPortH);
    //保存分辨率模式
    m_eResolutionPolicy = resolutionPolicy;
	
	// reset director's member variables to fit visible rect 重置director成员值，适应显示矩形
    CCDirector::sharedDirector()->m_obWinSizeInPoints = getDesignResolutionSize();
    CCDirector::sharedDirector()->createStatsLabel();
    CCDirector::sharedDirector()->setGLDefaultValues();
}
ResolutionPolicy 就是用来设置分辨率模式
在CCEGLViewProtocol.h
// 分辨率模式
enum ResolutionPolicy
{
    // The entire application is visible in the specified area without trying to preserve the original aspect ratio.
    // Distortion can occur, and the application may appear stretched or compressed.
	// 扩展填充模式：这里等于是直接设置投影机的分辨率。如果这个分辨率与画布大小的比例失调，就会出现失真。
    kResolutionExactFit,
    // The entire application fills the specified area, without distortion but possibly with some cropping,
    // while maintaining the original aspect ratio of the application.
	// 低调整模式：这个吧，是按照设置的分辨率在横纵方向上最低值调整视口大小，使投影机投影出来的影片画面所占的矩形在  
	//画布上所对应的相应方向上与最低值一致。
    kResolutionNoBorder,
    // The entire application is visible in the specified area without distortion while maintaining the original
    // aspect ratio of the application. Borders can appear on two sides of the application.
	// 高调整模式：与上面恰恰相反，按照设置的分辨率在横纵方向上最高值调整视口大小，使投影机投影出来的影片画面所占的  
	// 矩形在画布上所对应的相应方向上与最高值一致。同时这个矩形的另一个方向按最低值进行裁剪，区域外部分填充黑色。
    kResolutionShowAll,
    // The application takes the height of the design resolution size and modifies the width of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedHeight,
    // The application takes the width of the design resolution size and modifies the height of the internal
    // canvas so that it fits the aspect ratio of the device
    // no distortion will occur however you must make sure your application works on different
    // aspect ratios
    kResolutionFixedWidth,

    kResolutionUnKnown,
};
实际调用；
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	//通过CCDirector的静态函数sharedDirector来获取单件显示设备管理器指针 
    CCDirector* pDirector = CCDirector::sharedDirector();
	//通过CCEGLView的静态函数sharedOpenGLView来获取单件管理实例对象并
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	//将其地址通过CCDirector的成员函数setOpenGLView传递给显示设备管理器。
    pDirector->setOpenGLView(pEGLView);

	/// 获得窗口大小
	CCSize frameSize = pEGLView->getFrameSize();

    // Set the design resolution 设置显示分辨率模式 见CCEGLViewProtocol.h
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
#else
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
#endif

    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }


    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}
