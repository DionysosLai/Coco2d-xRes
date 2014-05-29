2014-02-21
	网格动作类似特效，可以实现翻转、抖动、震荡、水波纹等效果
CCGridBase有两个子类CCGrid3D和CCTiledGrid3D，其共同点是，网格
的每个子类都可以分离出来。
	网格动作是基于网格，运行网格运动的节点好像被分成了大小相同的很多
矩形，通过这些矩形动作形成整体动作，这些矩形就好像形成了一个矩阵。
16*12的网格运动的非常快，但效果并不是很好。32*24的网格看起来非常棒，但是有些
时候运动起来不是很快。
	使用网格之前需要先取消OpenGL的深度检测，调用如下语句：
	CCDirector::shareDirector()->setDepthTest(false)；
	网格动作的使用和普通动作一样，调用runAction来运行动作。
	
	具体子类在CCActionGrid3D.h CCActionPageTurn3D.h CCActionTiledGrid.h
	
	使用网格时，网格动作结束后需要把网格清空，参考如下：
	void TextLayer::checkAnim(float dt)
	{
		CCNode* s2 = getChildByTag(kTagBackground);
		if ( s2->numberOfRunningActions() == 0 && s2->getGrid() != NULL)
			s2->setGrid(NULL);;
	}		