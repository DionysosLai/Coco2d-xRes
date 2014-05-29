/// 滚动条 equipe_root---josn Layout 
UISlider* slider = dynamic_cast<UISlider*>(equipe_root->getChildByName("Slider_53"));
slider->setTouchEnabled(true);
slider->setScale9Enabled(true);	/// 是否开启9空格
slider->addEventListenerSlider(this, sliderpercentchangedselector(HelloWorld::sliderEvent));		///< 设置滚动条回调函数

函数：
void HelloWorld::sliderEvent( CCObject *pSender, SliderEventType type )
{
	switch (type)  
	{  
	case SLIDER_PERCENTCHANGED:  
		{  
			UISlider* slider = dynamic_cast<UISlider*>(pSender);  
			int percent = slider->getPercent();  
			CCLOG("%d", percent);    
		}  
		break;  

	default:  
		break;  
	} 
}