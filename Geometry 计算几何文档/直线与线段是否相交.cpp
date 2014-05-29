直线与线段是否相交.cpp
///@brief 判断线段与直线是否相交
///
///
///@param[in] p0,p1--线段两个端点， q0,q1--直线两个点值
///@pre 
///@return true---在线段上， false---不在线段上
///@retval 
///@post
bool LineMenu::straightLineIsIntersect(const CCPoint p0, const CCPoint p1, const CCPoint q0, const CCPoint q1)
{
	/// 先判断q0与q1能否组成一条直线
	if (!q0.equals(q1))
	{
		/// 当q0 == p0 q1 == p1时，结果为0。 只需要判断线段是否跨立直线即可。
		if (0 <= determinant(p0.x-q0.x, p0.y-q0.y, q1.x-q0.x, q1.y-q0.y) * 
			determinant(q1.x-q0.x, q1.y-q0.y, p1.x-q0.x, p1.y-q0.y))
		{
			CCLOG("straight line and segment line is intesect!");
			return false;
		}

		CCLOG("straight line and segment line isn't intesect!");
		return false;
	}
	
	CCLOG("straigth line cannnot be make up of q0 and q1!");
	return false;
}


double LineMenu::determinant(double x1, double y1, double x2, double y2)  // 行列式  
{  
	return (x1*y2-x2*y1);	///< 叉积 （与数学叉积不一样）
}  