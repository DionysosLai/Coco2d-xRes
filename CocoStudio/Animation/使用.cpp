/// 导入动画数据
CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("testAnimation/testAnimation0.png","testAnimation/testAnimation0.plist","testAnimation/testAnimation.ExportJson");
m_pArmature = CCArmature::create("testAnimation");
CCArmatureAnimation *pAnimation = m_pArmature->getAnimation();
pAnimation->play("ColorGreen");
// 	/// 设置动作循环情况
// 	pAnimation->setLoopType(ANIMATION_TO_LOOP_FRONT);
pAnimation->setMovementEventCallFunc(this, movementEvent_selector(HelloWorld::onAnimationEvent));	///< 监听角色动作状态 
pAnimation->setFrameEventCallFunc(this, frameEvent_selector(HelloWorld::onFrameEvent));				///< 设置帧监听事件
m_pArmature->setPosition(ccp(200, 200));

addChild(m_pArmature, 1);

void HelloWorld::onAnimationEvent( cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType eventType, const char *animationID )
{
	/// LOOP_COMPLETE 只有在动作是循环播放时，且每循环一次响应一次
	if (eventType == LOOP_COMPLETE)
	{  
		CCLOG("LOOP_COMPLETE");
	}  

	if (eventType == START)
	{
		CCLOG("START");
	}

	/// COMPLETE--只有在动作设置为不循环播放时才在动画结束响应一次
	if (eventType == COMPLETE)
	{
		CCLOG("COMPLETE");
	}
}

void HelloWorld::onFrameEvent( cocos2d::extension::CCBone* bone, const char* evt, int originFrameIndex, int currentFrameIndex )
{
	if (originFrameIndex == currentFrameIndex)
	{
		CCPoint p = m_pArmature->getBone("Layer2")->getDisplayRenderNode()->convertToWorldSpaceAR( ccp (0, 0)); 

		CCLOG("%s", evt);

		m_pArmature->getAnimation()->pause();
	}
}
