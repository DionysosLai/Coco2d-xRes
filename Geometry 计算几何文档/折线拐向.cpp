折线段的拐向判断：

折线段的拐向判断方法可以直接由矢量叉积的性质推出。对于有公共端点的线段p0p1和p1p2，通过计算(p2 - p0) × (p1 - p0)的符号便可以确定折线段的拐向：

若(p2 - p0) × (p1 - p0) > 0,则p0p1在p1点拐向右侧后得到p1p2。

若(p2 - p0) × (p1 - p0) < 0,则p0p1在p1点拐向左侧后得到p1p2。

若(p2 - p0) × (p1 - p0) = 0,则p0、p1、p2三点共线。


double vectorProduct(double x1, double y1, double x2, double y2)  // 行列式  
{  
	return (x1*y2-x2*y1);	///< 叉积 （与数学叉积不一样）
}  

///@brief 判断折线的拐向，
///
///以线段P1为参考，；
///@param[in] p1--线段1，p2--线段2
///@pre P1、P1必须有公共端点
///@return 1--左边 0 -- 共线 -1---右边
///@retval 
///@post
///@author DionysosLai,906391500@qq.com等
///@version 1.0
///@data 2014-04-10
int LineDebug::PolyLineDerection(const CCPoint p1, const CCPoint p0, const CCPoint p2)
{
	float vectorProductResult = 0.0f;

	vectorProductResult = (float)vectorProduct(p1.x-p0.x, p1.y-p0.y, p2.x-p0.x, p2.y-p0.y);
	
	if (vectorProductResult < 1e-3 && vectorProductResult > -1e-3)
	{
		CCLOG("p1p0 and p2p0 is at the same dereciton.");
		return 0;
	}
	else if (vectorProductResult >= 1e-3)
	{
		CCLOG("p2p0 is at p1p0's left dereciotn.");
		return -1;
	}
	else if (vectorProductResult <= -1e-3)
	{
		CCLOG("p2p0 is at p1p0's right dereciotn.");
		return 1;
	}
}