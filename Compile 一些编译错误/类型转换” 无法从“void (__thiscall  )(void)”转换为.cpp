1.提示为
error C2440: “类型转换”: 无法从“void (__thiscall xx::* )(void)”转换为“cocos2d::SEL_CallFuncN”
提示前 调用语句为
callfuncN_selector(GameMenu::menuEnter)
menuEnter这个方法里边是没有形参的 导致了这个问题出现
修改办法为
将menuEnter这个方法改成menuEnter(CCNode * node) 加入这个形参即可
2.提示为
error C2440: “类型转换”: 无法从“void (__thiscall xx::* )(void)”转换为“cocos2d::SEL_SCHEDULE”
解决办法同上 但是需要将方法的形参换成 float dt 就可以了
