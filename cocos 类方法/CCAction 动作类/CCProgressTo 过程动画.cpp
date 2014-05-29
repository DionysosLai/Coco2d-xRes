2014-20-19
	很多时候，在游戏之前，需要载入动作，这时需要一些动作来实现载入时的动画。
。CCProgressTo和CCProgressFromTo可以实现这个动画，但是执行这种动作的的节点是
CCProgressTimer。
//------------------------------------------------------------------
//
// SpriteProgressToRadial
//
//------------------------------------------------------------------
void SpriteProgressToRadial::onEnter()
{
    SpriteDemo::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	/// 动作时间 时间结束时，图片显示的百分比
    CCProgressTo *to1 = CCProgressTo::create(2, 100);
    CCProgressTo *to2 = CCProgressTo::create(2, 100);
	/// 参数为	精灵对象
    CCProgressTimer *left = CCProgressTimer::create(CCSprite::create(s_pPathSister1));
	/// 设置动画类型kCCProgressTimerTypeRadial--圆形扫描动画 kCCProgressTimerTypeBar---- 直线扫描动
    left->setType( kCCProgressTimerTypeRadial );
    addChild(left);
    left->setPosition(ccp(100, s.height/2));
    left->runAction( CCRepeatForever::create(to1));
    
    CCProgressTimer *right = CCProgressTimer::create(CCSprite::create(s_pPathBlock));
    right->setType(kCCProgressTimerTypeRadial);
    // Makes the ridial CCW 设置正反方向（配合kCCProgressTimerTypeRadial）
	///　如果是kCCProgressTimerTypeBar 则通过setBarChangeRate设置水平和竖直的变化量。
    right->setReverseProgress(true);
    addChild(right);
    right->setPosition(ccp(s.width-100, s.height/2));
    right->runAction( CCRepeatForever::create(to2));
}

std::string SpriteProgressToRadial::subtitle()
{
    return "ProgressTo Radial";
}

//------------------------------------------------------------------
//
// SpriteProgressToHorizontal
//
//------------------------------------------------------------------

void SpriteProgressToHorizontal::onEnter()
{
    SpriteDemo::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCProgressTo *to1 = CCProgressTo::create(2, 100);
    CCProgressTo *to2 = CCProgressTo::create(2, 100);
    
    CCProgressTimer *left = CCProgressTimer::create(CCSprite::create(s_pPathSister1));
    left->setType(kCCProgressTimerTypeBar);
    //    Setup for a bar starting from the left since the midpoint is 0 for the x
    left->setMidpoint(ccp(0,0));
    //    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
    left->setBarChangeRate(ccp(1, 0));
    addChild(left);
    left->setPosition(ccp(100, s.height/2));
    left->runAction( CCRepeatForever::create(to1));
    
    CCProgressTimer *right = CCProgressTimer::create(CCSprite::create(s_pPathSister2));
    right->setType(kCCProgressTimerTypeBar);
    //    Setup for a bar starting from the left since the midpoint is 1 for the x
	/// 设置开始点（0,0）--左上， （1,1）为右下
    right->setMidpoint(ccp(1, 0));
    //    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
    right->setBarChangeRate(ccp(1, 0));
    addChild(right);
    right->setPosition(ccp(s.width-100, s.height/2));
    right->runAction( CCRepeatForever::create(to2));
}

std::string SpriteProgressToHorizontal::subtitle()
{
    return "ProgressTo Horizontal";
}