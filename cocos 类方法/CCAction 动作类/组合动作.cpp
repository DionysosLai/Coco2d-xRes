2014-02-19
1. CCSequence 动作序列
	可以使用动作CCArray数组；可以把所用动作作为参数传入create函数中，最后结尾使用NULL（空值）即可；
还可以把两个有限时间动作按顺序传入create函数中；

2. CCSpawn 动作合成
	使被合成的动作同时进行，定义方法和CCSquence一致：
	代码如下：
	    CCAction*  action = CCSpawn::create(
        CCJumpBy::create(2, ccp(300,0), 50, 4),
        CCRotateBy::create( 2,  720),
        NULL);

3. CCRepeat和CCRepeatForever
	是动作重复类，不同之处，在于CCRepeat第二个参数设置重复次数；