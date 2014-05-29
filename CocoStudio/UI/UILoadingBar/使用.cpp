/// 进度条 equipe_root---josn Layout
UILoadingBar* loadingBar = dynamic_cast<UILoadingBar*>(equipe_root->getChildByName("LoadingBar_52"));
m_nCount = 0;
loadingBar->setPercent(m_nCount);	///< 开始时，设置进度为0；
//LoadingBarTypeLeft 从左向右,LoadingBarTypeRight  从右向左 
loadingBar->setDirection(LoadingBarTypeRight);   
schedule(schedule_selector(HelloWorld::updateLoadingBar));	///< 启动进度条跟新函数

void HelloWorld::updateLoadingBar( float ft )
{
	m_nCount++;  
	if (m_nCount > 100)  
	{  
		//unscheduleUpdate();  
		m_nCount = 0;  
	}  

	UILoadingBar* loadingBar = dynamic_cast<UILoadingBar*>(equipe_root->getChildByName("LoadingBar_52"));
	loadingBar->setPercent(m_nCount); 
}