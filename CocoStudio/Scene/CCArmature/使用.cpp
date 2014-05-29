/// 这里实现的是动画移动过程；
/// 重点：CCArmature *pButterFlyFish= (CCArmature *)(pFishRender->getNode()); 
void HelloWorld::update( float delta )
{
	 
	/// 获取节点信息 只有节点信息才是我们设置的信息
	CCNode* pFishNode = m_pLoginScene->getChildByTag(10009);
	CCPoint p1 = pFishNode->getPosition();	
	//获取comrender组件 
	CCComRender *pFishRender = (CCComRender*)(m_pLoginScene->getChildByTag(10009)->getComponent("CCArmature"));  
	//转换为armature  这里的armature是我们在动画中制作的相关数据了，因此下面我们要获得信息将会是我们在动画制作的数据
	/// 而不是在场景制作中的数据；
	CCArmature *pButterFlyFish= (CCArmature *)(pFishRender->getNode()); 
	/// 这里获得动画中的坐标
	CCPoint pt = pButterFlyFish->getPosition();

	if (!m_bDirection)
	{
		p1.x -= 2.0f;
		if (10 > p1.x)
		{
			m_bDirection = true;
		}
	}
	else
	{
		p1.x += 2.0f;
		if (900 < p1.x)
		{
			m_bDirection = false;
		}
	}
	pFishNode->setPosition(p1);
}