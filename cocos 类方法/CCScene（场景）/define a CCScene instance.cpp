CCScene* BackScreen::scene()
{
	// 'scene' is an autorelease object
	/// 新建场景类实例
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	///　定义布景层
	BackScreen *layer = BackScreen::create();

	// add layer as a child to scene 将布景层加入场景中
	scene->addChild(layer);

	// return the scene
	return scene;
}