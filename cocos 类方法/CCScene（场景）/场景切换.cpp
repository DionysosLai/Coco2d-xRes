1.使用方法：
正常的无过渡场景的如果尚未建立场景（即游戏中的第一个场景），就是用pDirector->runWithScene(pScene);即可以使用相应场景，如果是替换场景，则使用CCDirector::sharedDirector()->replaceScene(this);替换相应的场景即可，而如果要使用场景间的切换效果来切换场景，则需要使用相应的切换方法即transitionWithDuration（当然不同效果的使用方法略有不同）生成相应场景，然后再通过CCDirector::sharedDirector()->replaceScene(this)来启动场景，也就是说这个给这个场景加了一个外包装，然后再启动，那么，这个场景就不是直接显示了，而是在场景的效果使用完了以后进入场景，起到过渡的效果。
一般此函数有两个参数，第一个是特效的切换时间，直接生成一个CCTime即可，例子中设定的时间是1.2s，对于很多场景的显示都很舒服，第二个是要进入的场景，有的会有第三个参数，会在下面介绍
2.test中的效果总结
CCTransitionJumpZoom::transitionWithDuration(t, s);//跳跃式，本场景先会缩小，然后跳跃进来
CCTransitionFade::transitionWithDuration(t, s);//淡出淡入，原场景淡出，新场景淡入
CCTransitionFade::transitionWithDuration(t, s, ccWHITE);//如果上一个的函数，带3个参数，则第三个参数就是淡出淡入的颜色
CCTransitionFlipX::transitionWithDuration(t, s, kOrientationLeftOver);//x轴左翻
CCTransitionFlipX::transitionWithDuration(t, s, kOrientationRightOver);//x轴右翻
CCTransitionFlipY::transitionWithDuration(t, s, kOrientationUpOver);//y轴上翻
CCTransitionFlipY::transitionWithDuration(t, s, kOrientationDownOver);//y轴下翻
CCTransitionFlipAngular::transitionWithDuration(t, s, kOrientationLeftOver);//有角度转的左翻
CCTransitionFlipAngular::transitionWithDuration(t, s, kOrientationRightOver);//有角度转的右翻
CCTransitionZoomFlipX::transitionWithDuration(t, s, kOrientationLeftOver);//带缩放效果x轴左翻
CCTransitionZoomFlipX::transitionWithDuration(t, s, kOrientationRightOver);//带缩放效果x轴右翻
CCTransitionZoomFlipY::transitionWithDuration(t, s, kOrientationUpOver);//带缩放效果y轴上翻
CCTransitionZoomFlipY::transitionWithDuration(t, s, kOrientationDownOver);//带缩放效果y轴下翻 
CCTransitionZoomFlipAngular::transitionWithDuration(t, s, kOrientationLeftOver);//带缩放效果/有角度转的左翻
CCTransitionZoomFlipAngular::transitionWithDuration(t, s, kOrientationRightOver);//带缩放效果有角度转的右翻
CCTransitionShrinkGrow::transitionWithDuration(t, s);//交错换
CCTransitionRotoZoom::transitionWithDuration(t, s);//转角换
CCTransitionMoveInL::transitionWithDuration(t, s);//新场景从左移入覆盖
CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖
CCTransitionMoveInT::transitionWithDuration(t, s);//新场景从上移入覆盖
CCTransitionMoveInB::transitionWithDuration(t, s);//新场景从下移入覆盖
CCTransitionSlideInL::transitionWithDuration(t, s);//场景从左移入推出原场景
CCTransitionSlideInR::transitionWithDuration(t, s);//场景从右移入推出原场景
CCTransitionSlideInT::transitionWithDuration(t, s);//场景从上移入推出原场景
CCTransitionSlideInB::transitionWithDuration(t, s);//场景从下移入推出原场景
以下三个需要检测opengl版本是否支持CCConfiguration::sharedConfiguration()->getGlesVersion() <= GLES_VER_1_0如果为真则为不支持
CCTransitionCrossFade::transitionWithDuration(t,s);//淡出淡入交叉，同时进行
CCTransitionRadialCCW::transitionWithDuration(t,s);//顺时针切入
CCTransitionRadialCW::transitionWithDuration(t,s);//逆时针切入
以下两个需要先设置摄像机，使用CCDirector::sharedDirector()->setDepthTest(true);
CCTransitionPageTurn::transitionWithDuration(t, s, false);//翻页，前翻
CCTransitionPageTurn::transitionWithDuration(t, s, true);//翻页，后翻
CCTransitionFadeTR::transitionWithDuration(t, s);//向右上波浪
CCTransitionFadeBL::transitionWithDuration(t, s);//向左下波浪
CCTransitionFadeUp::transitionWithDuration(t, s);//向上百叶窗
CCTransitionFadeDown::transitionWithDuration(t, s);//向下百叶窗
CCTransitionTurnOffTiles::transitionWithDuration(t, s);//随机小方块
CCTransitionSplitRows::transitionWithDuration(t, s);//按行切
CCTransitionSplitCols::transitionWithDuration(t, s);//按列切