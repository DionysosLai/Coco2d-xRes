注意点。
1. 设置按钮，可以使用
	1)	CCMenuItemImage类，例子如下：
	CCMenuItemImage *newGameItem = CCMenuItemImage::create("newGameA.png", "newGameB.png",this,menu_selector(GameMenu::menuNewGameCallback));
	newGameItem->setScale(0.5);
	newGameItem->setPosition(ccp(size.width / 2,size.height / 2 - 20));
	newGameItem->setEnabled(false);
	然后在将这些菜单作为子菜单项CCMenuItem加入菜单CCMenu中，在将菜单CCMenu加入当前层中CCLayer中。
	eg:
	/// 使用按钮创建菜单
	CCMenu* mainmenu = CCMenu::create(newGameItem,continueItem,aboutItem,soundItem,NULL);
	/// 这里设置的菜单位置，将会加上前面的按钮位置（因此我们一般是直接设置子菜单的位置，而将父菜单位置设置在原点）
	mainmenu->setPosition(CCPointZero);
	this->addChild(mainmenu,1,3);
	2).

2. 文字设置，可以：
	1).	使用CCLabelTTF类
	//初始化关于框
	CCSprite* kuang = CCSprite::create("tb.png");
	kuang->setScale(0.5);
	kuang->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	this->addChild(kuang,2,2);
	char inf[256];
	sprintf(inf,"name:meow war\n\nprogram:shuoquan man\n\nart design:peng xu\n\ncompany:hz books\n\n    powered by cocos2D-x");
	CCLabelTTF * myjineng = CCLabelTTF::create(inf, "Marker Felt", 40, CCSizeMake(400,400), kCCTextAlignmentLeft);
	myjineng->setAnchorPoint(ccp(0,1));
	myjineng->setColor(ccc3(200,200,200));
	myjineng->setPosition(ccp(50,600));
	kuang->addChild(myjineng);
3. 内存自动回收
	使用cocos2d-x时无需再关心这个问题，就是在新创建实例时，加入autorelease，eg：
	CCLayer* pLayer = new CCLayer();
	pLayer->autorelease();
4. 建议还是不要使用加减坐标点。最好使用百分比，虽然麻烦点。图片的大小可以使用getContentSize()函数获取。

5. 内存泄露问题
	保留对象是要retain一下，记得release
 
	或者使用tag.
	my.tag=111;
	 
	获取的时候  CCNode* my2=[self getChildByTag:111]
	
6. 一、音乐音效的播放

[cpp] view plaincopy
//添加SimpleaudioEngine.h  
#include "SimpleAudioEngine/SimpleAudioEngine.h"  
  
//播放背景音乐的代码  
SimpleAudioEngine::getSharedEngine()->playBackgroundMusic("background-music-aac.mp3");  
  
//播放音效的代码  
SimpleAudioEngine::getSharedEngine()->playEffect("pew-pew-lei.wav");  

 
苹果自己音乐文件的格式caf。cocos2d-x下的SimpleAudioEngine对mp3和wav这两种大众格式都支持。

7. 场景类的初始化工作
       在objc的头文件中，可以不声明类成员函数，而直接在.m文件里实现。但是cpp不允许这样做。所以coco2d-x里面有一个init的方法。提供了累世的功能，所有继承自CCNODE的都有init方法。
 
       我马上想到了，既然父类都有了，我就可以直接从父类继承下来，使用self强制转化为指向自己的对象，这样子类就不用自己实现了。
 
       实际上是不行的，cpp没有self关键字。不能像OC那样直接从父类继承下来靠self关键字变成指向自己的对象。所以我们看到所有类都有一个node()方法，它集合了new,init,autorelease等方法，可以减少调用者的代码量。但是几乎每次都是一样的，所以我们可以使用cocos2d-x提供的一个宏来实现。
 
       LAYER_NODE_FUNC和SCENE_NODE_FUNC。但是如果想使用这两个宏，就必须在派生类里实现bool init()方法。 
 
8. 为什么一定要在构造函数结束后调用init()
       主要是因为C++构造函数有个缺陷，就是没有返回值。所以我们在C++构造函数异常时只能采取try-catch捕捉。而一般不推荐使用try-catch，主要是开启try-catch支持会使编译后的二进制程序增加不少体积，(我发现android NDK好像也不支持try-catch）。
 
       cocos2d-x采取了现在比较流行的“二阶段构造”的方法，即使用时先调构造函数，再调用init处理初始化逻辑。这种思路不论是在苹果iOS的接口设计（比如[[NSString alloc] init]，即二阶段构造）、还是在samsung bada操作系统使用C++类时都是如此。
 
9. set/get方法
在cocos2dx\include\Cocos2dDefine.h中实现了一系列的宏定义，可以自己看看。
比如有一个CCX_SYNTHESIZE_READONLY宏，可以用来实现了只读的类成员变量，也就是只有get方法。
 
10. 对象释放

建议在CPP的类构造函数里，对所有成员变量初始化，并在构造函数把释放的对象指针初始化为NULL。
retain了一定记得release。对于xxxwithxxx初始化出来的对象，一般会autorelease。不需要自己release。