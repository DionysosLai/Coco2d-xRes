2014-04-21
CCLabelTTF 每次调用 setString （即改变文字）的时候，一个新的OPENGL 纹理将会被创建.。这意味着setString 和创建一个新的标签一样慢。
所以，当你需要频繁的更新它们的时候，尽可能的不用去使用标签对象。  而应该使用CCLabelAtlas或者是CCLabelBMFont。

通常方法：
CCLabelTTF* label = CCLabelTTF::create("Hello World", "Arfial", 24);
label->setPosition(ccp(origin.x + visibleSize.width/2,
	origin.y + visibleSize.height - label->getContentSize().height));
this->addChild(label, 1);
label->setString("ni hao lai wu gong!");	///< 改变文字的方法
label->setColor(ccc3(159, 168, 176));		///< 改变颜色

//