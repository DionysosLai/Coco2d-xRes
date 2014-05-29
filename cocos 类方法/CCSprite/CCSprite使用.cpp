tests项目中的SpriteTest目录下是关于精灵类的使用示例。首先是定义CCSprite类的使
用方法，SpriteTest.cpp文件中Sprite1类中的addNewSpriteWithCoords函数，如代码清单3-29
所示。
void Sprite1::addNewSpriteWithCoords(CCPoint p)
{
	/// CCRANDOM_0_1() 0到1的一个随机数： 一张图里有14帧动画
    int idx = (int)(CCRANDOM_0_1() * 1400.0f / 100.0f);
	///85：一帧图的宽度；121：高度
    int x = (idx%5) * 85;	
    int y = (idx/5) * 121;
    
    /// 参数1： 是图片，参数2： 图片中的哪一个区域
    CCSprite* sprite = CCSprite::create("Images/grossini_dance_atlas.png", CCRectMake(x,y,85,121) );
    addChild( sprite );
    
    sprite->setPosition( ccp( p.x, p.y) );
    
    CCActionInterval* action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action = CCScaleBy::create(3, 2);
    else if(random < 0.40)
        action = CCRotateBy::create(3, 360);
    else if( random < 0.60)
        action = CCBlink::create(1, 3);
    else if( random < 0.8 )
        action = CCTintBy::create(2, 0, -255, -255);
    else 
        action = CCFadeOut::create(2);
    CCActionInterval* action_back = action->reverse();
    CCActionInterval* seq = CCSequence::create( action, action_back, NULL );
    
    sprite->runAction( CCRepeatForever::create(seq) );
}