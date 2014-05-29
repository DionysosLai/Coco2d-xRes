//LabelAtlas示例  
UILabelAtlas* labelAtlas = UILabelAtlas::create();  
/// labelWithString就是字符，charMapFile 就是字符的图像文件（这个只要图像文件就可以了），
/// itemWidth是每个字符的宽度，	itemHeight是每个字符的高度，这个不能设错，否则显示的时候可能就不对了。
/// 最后一个是起始字符，它是使用这个其实字符来对应字符和图像关系的。
labelAtlas->setProperty("03151", "labelatlasimg.png",24, 32, "0");  
labelAtlas->setPosition(ccp(100, 100));  
ul->addWidget(labelAtlas);

注：这里的ul
UILayer* ul =UILayer::create();
ul->addWidget(equipe_root);
addChild(ul, 1);