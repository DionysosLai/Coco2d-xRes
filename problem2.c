在test中的LabelTest.cpp
将“中国”改成三个或找个三个以上就不行
LabelBMFontChinese::LabelBMFontChinese()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelBMFont* pLable = CCLabelBMFont::create("中国", "fonts/bitmapFontChinese.fnt");
    pLable->setPosition(ccp(size.width / 2, size.height /2));
    this->addChild(pLable);
}