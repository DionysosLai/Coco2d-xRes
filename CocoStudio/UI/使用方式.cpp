	/// 获取UI
 	Layout* equipe_root =dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("LoginUI/DemoLogin.json"));     
	UILayer* ul =UILayer::create();
	ul->addWidget(equipe_root);
	addChild(ul, 1);