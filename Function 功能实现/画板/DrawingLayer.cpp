#include "DrawingLayer.h"

USING_NS_CC;

//创建对象
DrawingLayer* DrawingLayer::create()
{

	DrawingLayer * layer=new DrawingLayer();
	if(layer && layer->init()){
		layer->autorelease();
	}
	else{
		CC_SAFE_DELETE(layer);
	}
	return layer;

}
//构造函数
DrawingLayer::DrawingLayer()
{

}
//析构函数
DrawingLayer::~DrawingLayer()
{


}
//初始化
bool DrawingLayer::init()
{

	//初始化铅笔精灵
	pen=CCSprite::create("pen.png");
	pen->setVisible(false);
	pen->setAnchorPoint(ccp(0, 0));
	this->addChild(pen);


	//注册触屏响应事件 目标接点 当前类的对象 优先级为0 不吞噬
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);


	return true;
}
/*触摸事件*/
bool DrawingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

	//将点击坐标存入容器
	m_touchPoint.push_back(pTouch->getLocation());
	pen->setVisible(true);//显示铅笔精灵
	pen->setPosition(pTouch->getLocation());//设置位置为点击位置

	return true;
}

void DrawingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

	//移动过程中记录点的坐标 并存放到数组中
	m_touchPoint.push_back(pTouch->getLocation());
	pen->setPosition(pTouch->getLocation());

}

void DrawingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

	pen->setVisible(false);
	touchEndPos=ccp(-1, -1);
	m_touchPoint.push_back(touchEndPos);

}

//绘制
void DrawingLayer::draw()
{
	CCLayer::draw();
	::ccDrawColor4B(255,0,255,255);             //设定颜色
	::glLineWidth(5);                         //线条宽度5像素
	//如果数组不为空
	if (m_touchPoint.size()!=0){

		int count=m_touchPoint.size() ;     //记录点的个数
		static bool isFirstPoint = false;

		for(int i=0;i<count;i++)             //遍历容器
		{

			//获取点位置信息
			CCPoint pos=m_touchPoint[i];


			//判断是否重新触摸了 防止抬手后再次触摸时 新的起点与上次触摸的终点连接在一起
			if(pos.equals(touchEndPos)){
				isFirstPoint = true;
			}
			else{
				if(isFirstPoint||i==0){
					ccDrawPoint(ccp(pos.x,pos.y));  //i=0时  画第一个点
					isFirstPoint=false;
				}
				else
				{
					//画线
					CCPoint p=m_touchPoint[i-1];
					::ccDrawLine(ccp(pos.x,pos.y),ccp(p.x,p.y ));

				}
			}
		}
	}
}

