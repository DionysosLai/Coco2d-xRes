///@brief 判断线段与线段是否相交
///
///
///@param[in] aa,bb--线段1两个端点， cc,dd--线段1两个端点
///@pre 
///@return true---在线段上， false---不在线段上
///@retval 
///@post
bool LineMenu::segmentLineIsIntersect(CCPoint aa, CCPoint bb, CCPoint cc, CCPoint dd)  
{  
	/// 以两条线段形成的矩形不重合，说明两条线段必然不相交
	if (!isRectCollision(aa, bb, cc, dd))
	{
		return false;
	}
	
	/// 必须二者互相跨立 注意"="的情况。
	if (0 < determinant(aa.x-cc.x, aa.y-cc.y, dd.x-cc.x, dd.y-cc.y) * 
		determinant(dd.x-cc.x, dd.y-cc.y, bb.x-cc.x, bb.y-cc.y) &&
		0 < determinant(cc.x-aa.x, cc.y-aa.y, bb.x-aa.x, bb.y-aa.y) * 
		determinant(bb.x-aa.x, bb.y-aa.y, dd.x-aa.x, dd.y-aa.y))
	{
		return true;
	}
	return false;  
}  

double LineMenu::determinant(double x1, double y1, double x2, double y2)  // 行列式  
{  
	return (x1*y2-x2*y1);	///< 叉积 （与数学叉积不一样）
}  

///@brief 判断两个矩形是否相交
///
///
///@param[in] aa,bb--矩形1一个对角线端点， cc,dd--矩形2一个对角线端点
///@pre 
///@return true---相交， false---不相交
///@retval 
///@post
bool LineMenu::isRectCollision(CCPoint aa, CCPoint bb, CCPoint cc, CCPoint dd)
{
	CCPoint	centre1 = ccp((aa.x + bb.x)/2.0f, (aa.y + bb.y)/2.0f);	///< 获得中点值
	CCPoint	centre2 = ccp((cc.x + dd.x)/2.0f, (cc.y + dd.y)/2.0f);
	float	lengthX	= abs(centre1.x - centre2.x);	///< 获得两个矩形中心的距离 
	float	lengthY	= abs(centre1.y - centre2.y); 
	float	lengthRect1X	= abs(aa.x - bb.x);		///< 获得两个矩形长和宽
	float	lengthRect1Y	= abs(aa.y - bb.y);
	float	lengthRect2X	= abs(cc.x - dd.x);
	float	lengthRect2Y	= abs(cc.y - dd.y);

	/// 这里减去1是调整误差用的。
	return  (lengthX < (lengthRect1X + lengthRect2X)/2.0f-1 && lengthY < (lengthRect1Y + lengthRect2Y)/2.0f-1) ? true : false;
}