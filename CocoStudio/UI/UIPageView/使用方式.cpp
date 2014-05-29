UIPageView* pageView = dynamic_cast<UIPageView*>(equipe_root->getChildByName("PageView")); 
pageView->setTouchEnabled(true);    
pageView->addEventListenerPageView(this, pagevieweventselector(HelloWorld::pageViewEvent));		///< 设置page回调函数

回调函数：
void HelloWorld::pageViewEvent( CCObject *pSender, PageViewEventType type )
{
	switch (type)  
	{  
	case PAGEVIEW_EVENT_TURNING:  ///< 是否翻页了
		{  
			UIPageView* pageView = dynamic_cast<UIPageView*>(pSender);  
			CCLog("%s", pageView->getName());
		}  
		break;  
	default:  
		break;  
	}  
}

//UIPageView* pageView = UIPageView::create();  
pageView->setTouchEnabled(true);  
pageView->setSize(CCSizeMake(240, 130));  
CCSize backgroundSize = m_pUiLayer->getContentSize();  
pageView->setPosition(ccp((widgetSize.width - backgroundSize.width) / 2 +  
                          (backgroundSize.width - pageView->getSize().width) / 2,  
                          (widgetSize.height - backgroundSize.height) / 2 +  
                          (backgroundSize.height - pageView->getSize().height) / 2));  
  
for (int i = 0; i < 3; ++i)  
{  
    UILayout* layout = UILayout::create();  
    layout->setSize(CCSizeMake(240, 130));  
      
    UIImageView* imageView = UIImageView::create();  
    imageView->setTouchEnabled(true);  
    imageView->setScale9Enabled(true);  
    imageView->loadTexture("cocosgui/scrollviewbg.png");  
    imageView->setSize(CCSizeMake(240, 130));  
    imageView->setPosition(ccp(layout->getSize().width / 2, layout->getSize().height / 2));  
    layout->addChild(imageView);  
      
    UILabel* label = UILabel::create();  
    label->setText(CCString::createWithFormat("page %d", (i + 1))->getCString());  
    label->setFontName("Marker Felt");  
    label->setFontSize(30);  
    label->setColor(ccc3(192, 192, 192));  
    label->setPosition(ccp(layout->getSize().width / 2, layout->getSize().height / 2));  
    layout->addChild(label);  
      
    pageView->addPage(layout);  
}  
pageView->addEventListenerPageView(this, pagevieweventselector(HelloWorld::pageViewEvent));  
  
m_pUiLayer->addWidget(pageView); 