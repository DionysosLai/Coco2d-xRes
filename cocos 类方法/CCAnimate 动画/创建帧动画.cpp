2014-03-14
源码：
void MyClass::initMyAnim()  
{  
    /** 
    //可以在程序载入的时候预加载这些动画资源，然后在cache中读取 
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache(); 
    cache->addSpriteFramesWithFile("run.plist", "run.png"); 
    */  
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
    CCMutableArray<CCSpriteFrame*>* animFrames = new CCMutableArray<CCSpriteFrame*>(12);  
      
    char str[64] = {0};  
    for(int i = 1; i <=12; i++)   
    {  
        sprintf(str, "run%04d.png", i);  
        CCSpriteFrame* frame = cache->spriteFrameByName( str );  
        animFrames->addObject(frame);  
    }  
      
    CCAnimation* animation = CCAnimation::animationWithFrames(animFrames,0.4f);  
    CCActionInterval* action=CCAnimate::actionWithAnimation(animation,true);  
CCFiniteTimeAction *myRun= CCSequence::actions(action,CCCallFunc::actionWithTarget(this,callfunc_selector(MyClass::callBackRun)),NULL);  
    myRun->retain();  
    animFrames->release();  
}  

CCCallFunc用来做动画回调用，没有回调的话，就不用了。myRun->retain()后记得release掉。
OK，是不是很简单。记录一笔。

更改：
	    /** 
    //可以在程序载入的时候预加载这些动画资源，然后在cache中读取 
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache(); 
    cache->addSpriteFramesWithFile("run.plist", "run.png"); 
    */  
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
	cache->addSpriteFramesWithFile("cat.plist", "cat.png");		///< 必须预先加载数据
/*    CCMutableArray<CCSpriteFrame*>* animFrames = new CCMutableArray<CCSpriteFrame*>(4);  */
	CCSprite* spriteCat = CCSprite::create();
    CCArray* animFrames	= CCArray::createWithCapacity(4);
    char str[64] = {0};  
    for(int i = 1; i <= animFrames->capacity(); i++)   
    {  
		sprintf(str, "catBody%d.png", i); 
        CCSpriteFrame* frame = cache->spriteFrameByName(str);  	///< 只有预先加载数据后，才能读到数据。
        animFrames->addObject(frame);  
    }  
      
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f);  
//     CCActionInterval* action=CCAnimate::create(animation);  
// 	CCFiniteTimeAction *myRun= CCSequence::create(action,NULL);  
//     myRun->retain();  
//     animFrames->release(
	spriteCat->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
	addChild(spriteCat, 1);