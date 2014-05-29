CardSprite* card = CardSprite::create("1.png", "2.png", 2);  

card->setPosition(ccp(visibleSize.width * .5f, visibleSize.height * .5f));  

addChild(card);  

card->openCard();


static CCOrbitCamera* create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);

参数分别是：

旋转的时间、起始半径、半径差、起始z角、旋转z角差、起始x角、旋转x角差