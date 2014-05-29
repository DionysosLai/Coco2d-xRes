///@brief 判断点是否在矩形内(点在边界也算)
///
///只要判断该点的横坐标和纵坐标是否夹在矩形的左右边和上下边之间即可。
///@param[in] p0 --- 要判断的点 r0,r1矩形对角线的两个端点 
///@pre r0,r1必须可以形成一个矩形
///@return true---在矩形内 false ---不在矩形内；
///@retval 
///@post 
///@author DionysosLai,906391500@qq.com等
///@version 1.0
///@data 2014-04-10
bool HelloWorld::pointIsInRect(const CCPoint p0, const CCPoint r0, const CCPoint r1)
{
	/// 首先判断r0 r1 是否可以组成一个矩形；----这个不用判断，不然点在线上就判断不出来
// 	if (r0.x == r1.x || r0.y == r1.y)
// 	{
// 		CCLOG("Rectangle cannot be maken up by the two points!");
// 		return false;
// 	}

	/// 获取矩形的4条边界
	float minX = r0.x > r1.x ? r1.x : r0.x;
	float maxX = r0.x > r1.x ? r0.x : r1.x;
	float minY = r0.y > r1.y ? r1.y : r0.y;
	float maxY = r0.y > r1.y ? r0.y : r1.y;

	/// 判断点是否在矩形左右、上下边界内
	if ((p0.x >= minX && p0.x <= maxX) &&
		(p0.y >= minY && p0.y <= maxY))
	{
		CCLOG("Point is in rectangle1");
		return true;
	}
	else
	{
		CCLOG("Point is not in rectangle!");
		return false;
	}
}