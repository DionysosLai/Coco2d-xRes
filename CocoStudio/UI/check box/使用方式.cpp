/// 获取复选框 equipe_root---josn Layout 
UICheckBox* checkBox = dynamic_cast<UICheckBox*>(equipe_root->getChildByName("agree_CheckBox"));
checkBox->setTouchEnabled(true);
checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(HelloWorld::touchCheckBoxEvent));	///< 设置复选框回调函数

void HelloWorld::touchCheckBoxEvent( CCObject *pSender, CheckBoxEventType type )
{
	switch (type)  
	{  
	case CHECKBOX_STATE_EVENT_UNSELECTED://取消  
		{
			CCLOG("UNSELECTED");
		}
		break;  

	case CHECKBOX_STATE_EVENT_SELECTED: //选中  
		{
			CCLOG("SELECTED");
		}
		break;  

	default:  
		break;  
	}
}