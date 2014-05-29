2014-04-22
1. 制作方式与CCArmature类似


2. 使用：
CCComRender* pLayoutRender = static_cast<CCComRender*>(pFishJoyScene->getChildByTag(10010)->getComponent("GUIComponent"));
UILayer* startLayout = static_cast<UILayer*>(pLayoutRender->getNode());
UIButton* startBtn = (UIButton*)(startLayout->getWidgetByName("StartBtn"));
startBtn->setTouchEnabled(true);
startBtn->addTouchEventListener(this, toucheventselector(HelloWorld::touchCloseBtnEvent));     

监听方法如下：
void HelloWorld::touchCloseBtnEvent( CCObject *pSender, TouchEventType type )
{
	//按下、移动、结束、取消  
	switch (type)  
	{  
	case TOUCH_EVENT_BEGAN:
		{
			CCLOG("Begin");
		}
		break;

	case TOUCH_EVENT_MOVED:		
		{
			CCLOG("Move");
		}
		break;

	case TOUCH_EVENT_ENDED:
		{
			CCLOG("end");
		}
		break;

	case TOUCH_EVENT_CANCELED:
		{
			CCLOG("Cancel");
		}
		break;

	default:  
		break;  
	}  
}
注意：这里的pFishJoyScene
/// 导入场景
CCNode *pFishJoyScene = SceneReader::sharedSceneReader()->createNodeWithSceneFile("FishJoy2.json");
m_pLoginScene = pFishJoyScene;
this->addChild(pFishJoyScene, 0);