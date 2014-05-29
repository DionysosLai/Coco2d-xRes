UILabel* m_pDisplayValueLabel = UILabel::create();  
m_pDisplayValueLabel->setText("No Event");  
m_pDisplayValueLabel->setFontName("Marker Felt");  
m_pDisplayValueLabel->setAnchorPoint(ccp(0.5f, -1));  
m_pDisplayValueLabel->setPosition(ccp(ul->getContentSize().width/2.f, ul->getContentSize().height/1.5f));  
m_pDisplayValueLabel->setColor(ccc3(159, 168, 176));  
ul->addWidget(m_pDisplayValueLabel); 

注：这里的ul
UILayer* ul =UILayer::create();
ul->addWidget(equipe_root);
addChild(ul, 1);