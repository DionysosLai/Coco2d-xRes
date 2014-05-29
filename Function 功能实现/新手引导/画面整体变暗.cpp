//第一个参数是颜色ccc4(r,g,b,a) a取值(0~255),越大越不透明 下面两个参数为宽高,不传默认为屏幕大小, 默认起始位置从（0 ，0）开始
//第一个参数是颜色ccc4(r,g,b,a) a取值(0~255),越大越不透明 下面两个参数为宽高,不传默认为屏幕大小, 默认起始位置从（0 ，0）开始
CCLayerColor* pLayer=CCLayerColor::create(ccc4(0,0,0,200),visibleSize.width,visibleSize.height);
//CCLayerColor* pLayer=CCLayerColor::create(ccc4(0,0,0,200));
addChild(pLayer,100);