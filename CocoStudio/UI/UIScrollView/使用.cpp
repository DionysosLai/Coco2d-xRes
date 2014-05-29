/// 滚动条示例 equipe_root---josn Layout
UIScrollView* scrollView = dynamic_cast<UIScrollView*>(equipe_root->getChildByName("ScrollView")); 
/////设置滚动方向
/*
SCROLLVIEW_DIR_NONE,	不滚动
SCROLLVIEW_DIR_VERTICAL,	垂直
SCROLLVIEW_DIR_HORIZONTAL,	水平
SCROLLVIEW_DIR_BOTH		水平、垂直两种
*/
scrollView->setDirection(SCROLLVIEW_DIR_BOTH);  
scrollView->setTouchEnabled(true);	 
scrollView->setBounceEnabled(true);	///< 是否开启回弹模式
//	scrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));         //显示范围 