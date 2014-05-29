在cocos2d中

起点坐标(x=0, y=0)位于左下角，这意味着屏幕位于
X轴从屏幕最左边开始，由左向右渐增
Y轴坐标从屏幕最下方开始，由下向上渐增
[sprite setAnchorPoint:ccp(0.5,0.5)] 锚点：中心 默认

[sprite setAnchorPoint:ccp(0,0)] 锚点：左下角

[sprite setAnchorPoint:ccp(0,1)] 锚点：左上角

[sprite setAnchorPoint:ccp(1,0)] 锚点：右下角

[sprite setAnchorPoint:ccp(1,1)] 锚点：右上角

setPosition为外层锚点与本层锚点的距离