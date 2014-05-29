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

///@brief 判断点是否在矩形中
///
///@param[in|out] p0--点 c1--圆中心点 radius--半径
///@pre 
///@return 说明
///@retval 
///@post 
///@author DionysosLai，906391500@qq.com
///@version 1.0
///@data 20014-05-05
bool BAMonkey::pointIsInRect( const CCPoint p0, const CCPoint r1, const float width, const float heigth )
{
	float xLeft = r1.x - width/2.f;
	float xRigth = r1.x + width/2.f;
	float yUp = r1.y + heigth/2.f;
	float yBottom = r1.y - heigth/2.f;

	if (p0.x >= xLeft && p0.x<= xRigth && p0.y >= yBottom && p0.y <= yUp)
	{
		return true;
	}

	return false;
}