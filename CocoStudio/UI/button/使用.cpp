/// 获取按钮 equipe_root---josn Layout 
UIButton* closeBtn = dynamic_cast<UIButton*>(equipe_root->getChildByName("close_Button"));
closeBtn->setVisible(true);
closeBtn->setScale9Enabled(true);	/// 是否开启9空格
closeBtn->setTouchEnabled(true);
closeBtn->addTouchEventListener(this, toucheventselector(HelloWorld::touchCloseBtnEvent));	///< 设置按钮回调函数
	
回调函数：
void HelloWorld::touchCloseBtnEvent( CCObject *pSender, TouchEventType type )
{
	//按下、移动、结束、取消  
	switch (type)  
	{  
	case TOUCH_EVENT_BEGAN:
		
		break;

	case TOUCH_EVENT_MOVED:		
		{
		}
		break;

	case TOUCH_EVENT_ENDED:
		
		break;

	case TOUCH_EVENT_CANCELED:
		
		break;

	default:  
		break;  
	}  
}