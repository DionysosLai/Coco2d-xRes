2014-04-22
场景当中可以包含多种元素，总体包括精灵、骨骼动作（动画）、UI、
地图(就是使用TileMap生产的XML文件)、例子、声音。

使用:
/// 导入场景
CCNode *pFishJoyScene = SceneReader::sharedSceneReader()->createNodeWithSceneFile("FishJoy2.json");
this->addChild(pFishJoyScene, 0);

/// 获取节点信息
/// 获取节点信息 只有节点信息才是我们设置的信息
CCNode* pFishNode = m_pLoginScene->getChildByTag(10009);	///< 获取tag为10009的节点信息	
CCPoint p1 = pFishNode->getPosition();	