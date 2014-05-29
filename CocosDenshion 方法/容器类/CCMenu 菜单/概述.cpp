2014-03-07
1.概况
CCMenu是cocos2d-x的菜单类，但它本身并不具备菜单具体属性，而是作为一个父节点，将一些具体类型的菜单添加进去来达到菜单目的的。我们正常会这么使用它：先创建子菜单项CCMenuItem，然后将子菜单项CCMenuItem加入菜单CCMenu，最后把CCMenu加入当前层CCLayer。
如果直接在层中添加CCMenuItem会怎样？其实显示也是会正常，但是无法响应回调，因为CCMenu是继承至CCLayer，也就继承了触摸的相关事件，而CCMenuItem只是从CCNode继承而来，并不响应触摸，因此无法调用回调函数。

2.菜单CCMenu


3.子菜单项CCMenuItem
子菜单项可以是用精灵，图片，标签文本，或者开关都可以，这里不具体解释。请看：

4.锚点
CCMenu继承于CCLayer，所以它具有忽略锚点的属性。而CCMenuItem的锚点是正常的(0.5,0.5)。
（1）看一下自带的HelloCpp这个工程，在添加退出按钮的时候，是不是有这么一句：
[cpp] view plaincopy 
pMenu->setPosition(CCPointZero);  
这句话的作用就是把Menu放在原点的位置，假设Menu有大小，那就是下图1。
（2）现在再做点改变，把MenuItem指定的位置去掉，也就是让它默认在(0,0)位置上。
[cpp] view plaincopy 
//pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,origin.y + pCloseItem->getContentSize().height/2));  
那么再看一下效果，如图2，抽象出来就是图3，因为CCMenuItem的锚点是(0.5,0.5)。
            
（3）所以现在可以理解HelloCpp中为什么定义的是CCMenuItem的位置，而不是定义CCMenu的位置了吧。CCMenu包含了多个子菜单项，每个子菜单的位置都不一样，如果定义了CCMenu的位置，那它作为父节点会影响到所有的子菜单的位置，因此我们一般把它设置在原点处，然后通过设置子菜单的位置（也就是相对父节点的偏移量）来定位整个菜单。

5.多子菜单的排列
（1）横向排列，纵向排列，可以指定间隔
[cpp] view plaincopy 
CCMenuItemImage *pCloseItem1 = CCMenuItemImage::create(  
    "CloseNormal.png",  
    "CloseSelected.png",  
    this,  
    menu_selector(HelloWorld::menuCloseCallback));  
CCMenuItemImage *pCloseItem2 = CCMenuItemImage::create(  
    "CloseNormal.png",  
    "CloseSelected.png",  
    this,  
    menu_selector(HelloWorld::menuCloseCallback));  
  
CCMenu* pMenu = CCMenu::create(pCloseItem1,pCloseItem2,NULL);  
pMenu->alignItemsHorizontally();//默认间隔是kDefaultPadding=5  
//pMenu->alignItemsHorizontallyWithPadding(20);//指定间隔  
//pMenu->alignItemsVertically();//默认间隔也是kDefaultPadding=5  
//pMenu->alignItemsVerticallyWithPadding(20);//指定间隔  
this->addChild(pMenu, 1);  



（2）按行排列，按列排列，不能指定间隔，只能自己实现。希望下一版本有这个功能。
[cpp] view plaincopy 
CCMenuItemImage *pCloseItem1 = CCMenuItemImage::create(  
    "CloseNormal.png",  
    "CloseSelected.png",  
    this,  
    menu_selector(HelloWorld::menuCloseCallback));  
...  
CCMenuItemImage *pCloseItem6 = CCMenuItemImage::create(  
    "CloseNormal.png",  
    "CloseSelected.png",  
    this,  
    menu_selector(HelloWorld::menuCloseCallback));  
CCMenu* pMenu = CCMenu::create(pCloseItem1,pCloseItem2,pCloseItem3,pCloseItem4,pCloseItem5,pCloseItem6,NULL);  
pMenu->alignItemsInRows(3,2,1,NULL);  
//pMenu->alignItemsInColumns(3,2,1,NULL);  
this->addChild(pMenu, 1);  

