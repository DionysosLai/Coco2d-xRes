判断点是否在线段上：
	设点为Q，线段为P1P2 ，判断点Q在该线段上的依据是：( Q - P1 ) × ( P2 - P1 ) = 0 
且 Q 在以 P1，P2为对角顶点的矩形内。前者保证Q点在直线P1P2上，后者是保证Q点不在
线段P1P2的延长线或反向延长线上

///@brief 判断点是否在线段上
///
///
///@param[in] p0--点，p1,p2--线段两个端点
///@pre 
///@return true---在线段上， false---不在线段上
///@retval 
///@post
///@author DionysosLai,906391500@qq.com等
///@version 1.0
///@data 2014-04-10
bool LineDebug::pointIsAtLine(const CCPoint p0, const CCPoint p1, const CCPoint p2)
{
	/// 先判断是否点在以p1 p2为对角线的矩形内
	if (pointIsAtRect(p0, p1, p2))
	{
		/// 判断p1p0, p2p0是否共线
		if (0 == PolyLineDerection(p0, p1, p2))
		{
			CCLOG("Point in at the line.");
			return true;
		}

		CCLOG("Point isn't at the line.");
		return false;
	}

	CCLOG("Point isn't at the line.");
	return false;
}

用到的函数：
///@brief 判断点是否在矩形内
///
///
///@param[in] p0--点，p1,p2--矩形的一条对角线端点
///@pre 
///@return true---在线段上， false---不在线段上
///@retval 
///@post
bool LineDebug::pointIsAtRect(const CCPoint p0, const CCPoint p1, const CCPoint p2)
{
	float xMax = 0, xMin, yMax = 0, yMin = 0;
	xMax = p1.x > p2.x ? p1.x : p2.x;
	xMin = p1.x > p2.x ? p2.x : p1.x;
	yMax = p1.y > p2.y ? p1.y : p2.y;
	yMin = p1.y > p2.y ? p2.y : p1.y;

	if ( p0.x >= xMin && p0.x <= xMax && p0.y >= yMin && p0.y <= yMax)
	{
		CCLOG("Point is at recangle.");
		return true;
	}
	CCLOG("Point isn't at recangle.");
	return false;
}

///@brief 判断折线的拐向，
///
///以线段P1为参考，；
///@param[in] p1--线段1，p2--线段2
///@pre P1、P1必须有公共端点
///@return 1--左边 0 -- 共线 -1---右边
///@retval 
///@post
int LineDebug::PolyLineDerection(const CCPoint p1, const CCPoint p0, const CCPoint p2)
{
	float vectorProductResult = 0.0f;

	vectorProductResult = (float)vectorProduct(p1.x-p0.x, p1.y-p0.y, p2.x-p0.x, p2.y-p0.y);
	
	if (vectorProductResult < 1e-3 && vectorProductResult > -1e-3)
	{
		CCLOG("p1p0 and p2p0 is at the same dereciton.");
		return 0;
	}
	
	if (vectorProductResult >= 1e-3)
	{
		CCLOG("p2p0 is at p1p0's left dereciotn.");
		return -1;
	}
	
	if (vectorProductResult <= -1e-3)
	{
		CCLOG("p2p0 is at p1p0's right dereciotn.");
		return 1;
	}
}

double LineDebug::vectorProduct(double x1, double y1, double x2, double y2)  // 行列式  
{  
	return (x1*y2-x2*y1);	///< 叉积 （与数学叉积不一样）
}  
