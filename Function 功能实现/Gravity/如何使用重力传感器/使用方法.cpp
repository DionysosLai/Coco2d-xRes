在头文件中重载函数：
	virtual void didAccelerate(CCAcceleration* pAccelerationValue);
在源文件中：
1.使能重力传感器：	setAccelerometerEnabled(true);
2.重写didAccelerate函数：
void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
	CCLog("%f, %f, %f", pAccelerationValue->x, pAccelerationValue->y, pAccelerationValue->z);
}