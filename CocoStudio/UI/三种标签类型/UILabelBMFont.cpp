UILabelBMFont	//labelBMFont示例  
UILabelBMFont* labelBMFont = UILabelBMFont::create();  
labelBMFont->setFntFile("Heiti16.fnt");  
labelBMFont->setText("03");  
labelBMFont->setPosition(ccp(100, 100));  
ul->addWidget(labelBMFont);  

注：这里的ul
UILayer* ul =UILayer::create();
ul->addWidget(equipe_root);
addChild(ul, 1);