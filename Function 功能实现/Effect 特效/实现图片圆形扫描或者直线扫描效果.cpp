2014-03-14

/// 动作时间 时间结束时，图片显示的百分比
CCProgressTo *to1 = CCProgressTo::create(2, 100);
CCProgressTo *to2 = CCProgressTo::create(2, 100);
/// 参数为	精灵对象
CCProgressTimer *left = CCProgressTimer::create(CCSprite::create("cat1.png"));
/// 设置动画类型kCCProgressTimerTypeRadial--圆形扫描动画 kCCProgressTimerTypeBar---- 直线扫描动
left->setType( kCCProgressTimerTypeRadial );
addChild(left);
left->setPosition(ccp(size.width/3, size.height/2));
left->runAction(to1);	///< 出现一次

CCProgressTimer *right = CCProgressTimer::create(CCSprite::create("cat1.png"));
right->setType(kCCProgressTimerTypeRadial);
// Makes the ridial CCW 设置正反方向（配合kCCProgressTimerTypeRadial）
///　如果是kCCProgressTimerTypeBar 则通过setBarChangeRate设置水平和竖直的变化量。
right->setReverseProgress(true);
addChild(right);
right->setPosition(ccp(size.width/3*2, size.height/2));
right->runAction( CCRepeatForever::create(to2));