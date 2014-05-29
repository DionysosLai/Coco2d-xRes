2014-03-27
CCScale9Sprite 是利用9宫格图缩放图片的，也就是缩放带圆角的图片。
这个比较有用处，很多游戏中有很多不同尺寸的圆角图片作为背景。有了CCScale9Sprite之后，只需要提供一个非常小尺寸的圆角图片就可以自由缩放其他尺寸的圆角图。是个不错的东西。

使用方法：
1、导入头文件及命名空间
[cpp] view plaincopy
#include "cocos-ext.h"  
USING_NS_CC_EXT;  


2、初始化代码：
[cpp] view plaincopy
CCScale9Sprite* labBg1 = CCScale9Sprite::create("wd_bg_text.png");  
labBg1->setAnchorPoint(ccp(.5,.5));  
labBg1->setPreferredSize(CCSizeMake(255, 20));  
labBg1->setPosition(ccp(size.width/2, size.height/2));  
addChild(labBg1);  


说明：
setPreferredSize 就是设置需要生成的尺寸大小。