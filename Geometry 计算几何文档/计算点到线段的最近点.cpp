计算点到线段的最近点：
如果该线段平行于X轴（Y轴），则过点point作该线段所在直线的垂线，垂足很容易求得，然后计算出垂足，
如果垂足在线段上则返回垂足，否则返回离垂足近的端点；如果该线段不平行于X轴也不平行于Y轴，则斜率
存在且不为0。设线段的两端点为pt1和pt2，斜率为：k = ( pt2.y - pt1. y ) / (pt2.x - pt1.x );该直线
方程为：y = k* ( x - pt1.x) + pt1.y。其垂线的斜率为 - 1 / k，垂线方程为：y = (-1/k) * (x - point.x) + point.y 。
联立两直线方程解得：x = ( k^2 * pt1.x + k * (point.y - pt1.y ) + point.x ) / ( k^2 + 1) ，y = k * ( x - pt1.x) + pt1.y;
然后再判断垂足是否在线段上，如果在线段上则返回垂足；如果不在则计算两端点到垂足的距离，选择距离垂足较近的端点返回。

///@brief 点到线段最近一个点
///@param[in] p0--要判断点， s0，s1--线段两个端点
///@return crosspoint---最近的点
///@author DionysosLai,906391500@qq.com
///@retval 
///@post
///@version 1.0
///@data 2014-04-22
cocos2d::CCPoint HelloWorld::nearestPointToSegmentLine( const CCPoint p0, const CCPoint s0, const CCPoint s1 )
{
	/// 判断线段是否是一个点
	if (s0.equals(s1))
	{
		return s0;
	}

	/// 初始垂足为0；
	CCPoint crossPoint = CCPointZero;
	do 
	{
		/// 判断线段是否平行于x轴
		if (s0.y == s1.y)
		{
			crossPoint = ccp(p0.x, s0.y);
			break;
		}
		/// 判断线段是否平行于y轴
		if (s0.x == s1.x)
		{
			crossPoint = ccp(s0.x, p0.y);
			break;
		}
		/// 如果线段不是特殊情况，则只能采用直线方程方式联立求解
		float k = (s1.y - s0.y)/(s1.x - s0.x);		///< 求得斜率
		/// 线段直线方程：	y = k* ( x - s0.x) + s0.y
		/// 垂线方程为：	y = (-1/k) * (x - p0.x) + p0.y 。
		/// 联立两直线方程解得
		float x = ( k*k * s0.x + k * (p0.y - s0.y ) + p0.x ) / ( k*k + 1);
		float y = k * ( x - s0.x) + s0.y;
		crossPoint = ccp(x, y);
		break;
	} while (0);
	
	/// 判断垂直是否在线段上
	if (pointIsAtLine(crossPoint, s0, s1))
	{
		return crossPoint;
	}
	else
	{
		/// 如果不在则计算两端点到垂足的距离，选择距离垂足较近的端点返回。
		float distance1 = ccpDistance(crossPoint, s0);
		float distance2 = ccpDistance(crossPoint, s1);
		if (distance1 < distance2)
		{
			return s0;
		}
		else
		{
			return s1;
		}
	}
}