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