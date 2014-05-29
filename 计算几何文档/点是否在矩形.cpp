判断点是否在矩形
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