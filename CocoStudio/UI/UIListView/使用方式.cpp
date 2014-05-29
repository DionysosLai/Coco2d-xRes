/// 滚动条示例 equipe_root---josn Layout
UIListView* listView = dynamic_cast<UIListView*>(equipe_root->getChildByName("ListView")); 
listView->setTouchEnabled(true);  
listView->setBounceEnabled(true); 
/* 设置初始位置
LISTVIEW_GRAVITY_LEFT,	向左对齐
LISTVIEW_GRAVITY_RIGHT, 向右对清
LISTVIEW_GRAVITY_CENTER_HORIZONTAL,	向中心对齐

LISTVIEW_GRAVITY_TOP,	向上对齐
LISTVIEW_GRAVITY_BOTTOM,	向底对齐
LISTVIEW_GRAVITY_CENTER_VERTICAL,	水平对齐
*/
listView->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);  //设置列表重心，我个人理解是对齐方式
/*
SCROLLVIEW_DIR_NONE,	不滚动
SCROLLVIEW_DIR_VERTICAL,	垂直
SCROLLVIEW_DIR_HORIZONTAL,	水平
SCROLLVIEW_DIR_BOTH		水平、垂直两种
*/
/*	listView->setDirection(SCROLLVIEW_DIR_BOTH);     //滚动方向*/
/*	listView->setBackGroundImage("cocosgui/green_edit.png");*/
