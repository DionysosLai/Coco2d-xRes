/**
*@file 
*@brief 
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-7-28 16:26
*/
#include "Geometry.h"

USING_NS_CC;

double vectorProduct( const double& x1, const double& y1, const double& x2, const double& y2 )
{
	return (x1*y2-x2*y1);	///< 叉积 （与数学叉积不一样）
}

///@brief 判断折线的拐向，
///以线段P1为参考，；
///@param[in] p1--线段1，p2--线段2
///@pre P1、P1必须有公共端点
///@return 1--左边 0 -- 共线 -1---右边
///@retval 
///@post
///@author DionysosLai,906391500@qq.com等
///@version 1.0
///@data 2014-04-10
int polyLineDerection( const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p2 )
{
	float vectorProductResult = 0.0f;

	vectorProductResult = (float)vectorProduct(p1.x-p0.x, p1.y-p0.y, p2.x-p0.x, p2.y-p0.y);

	if (vectorProductResult < 1 && vectorProductResult > -1)
	{
		/*		CCLOG("p1p0 and p2p0 is at the same dereciton.");*/
		return 0;
	}

	if (vectorProductResult >= 1)
	{
		/*		CCLOG("p2p0 is at p1p0's left dereciotn.");*/
		return -1;
	}

	if (vectorProductResult <= -1)
	{
		/*		CCLOG("p2p0 is at p1p0's right dereciotn.");*/
		return 1;
	}
}

bool straightLineIsIntersect( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& q0, const cocos2d::CCPoint& q1 )
{
	/// 先判断q0与q1能否组成一条直线
	if (!q0.equals(q1))
	{
		/// 当q0 == p0 q1 == p1时，结果为0。 只需要判断线段是否跨立直线即可。
		if (0 <= vectorProduct(p0.x-q0.x, p0.y-q0.y, q1.x-q0.x, q1.y-q0.y) * 
			vectorProduct(q1.x-q0.x, q1.y-q0.y, p1.x-q0.x, p1.y-q0.y))
		{
/*			CCLOG("straight line and segment line is intesect!");*/
			return false;
		}

/*		CCLOG("straight line and segment line isn't intesect!");*/
		return false;
	}

/*	CCLOG("straigth line cannnot be make up of q0 and q1!");*/
	return false;
}

///@brief 判断点是否在矩形中s
///@param[in|out] p0--点 r1--矩形中心 width--矩形宽 heigth---矩形高
///@pre 
///@return true--在矩形内， false---在矩形外
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 16:48
bool pointIsInRect( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& r1, const float& width, const float& heigth )
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

///@brief 判断点是否在矩形内
///@param[in] p0--点，p1,p2--矩形的一条对角线端点
///@pre 
///@return true---在线段上， false---不在线段上
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 16:54
bool pointIsInRect( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2 )
{
	float xMax = 0, xMin, yMax = 0, yMin = 0;
	xMax = p1.x > p2.x ? p1.x : p2.x;
	xMin = p1.x > p2.x ? p2.x : p1.x;
	yMax = p1.y > p2.y ? p1.y : p2.y;
	yMin = p1.y > p2.y ? p2.y : p1.y;

	if ( p0.x >= xMin && p0.x <= xMax && p0.y >= yMin && p0.y <= yMax)
	{
/*		CCLOG("Point is at recangle.");*/
		return true;
	}
/*	CCLOG("Point isn't at recangle.");*/
	return false;
}

///@brief 点到线段最近一个点
///@param[in] p0--要判断点， s0，s1--线段两个端点
///@return crosspoint---最近的点
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 16:58
cocos2d::CCPoint nearestPointToSegmentLine( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& s0, const cocos2d::CCPoint& s1 )
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

///@brief 判断两个矩形是否相交
///@param[in] aa,bb--矩形1一个对角线端点， cc,dd--矩形2一个对角线端点
///@pre 
///@return true---相交， false---不相交
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 17:03
bool isRectsInterSect( const cocos2d::CCPoint& aa, const cocos2d::CCPoint& bb, const cocos2d::CCPoint& cc, const cocos2d::CCPoint& dd )
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

///@brief 判断点是否在线段上
///@param[in] p0--点，p1,p2--线段两个端点
///@pre 
///@return true---在线段上， false---不在线段上
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 17:14
bool pointIsAtLine( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2 )
{
	/// 先判断是否点在以p1 p2为对角线的矩形内
	if (pointIsInRect(p0, p1, p2))
	{
		/// 判断p1p0, p2p0是否共线
		if (0 == polyLineDerection(p0, p1, p2))
		{
			/*			CCLOG("Point in at the line.");*/
			return true;
		}

		/*		CCLOG("Point isn't at the line.");*/
		return false;
	}

	// 	CCLOG("Point isn't at the line.");
	return false;
}

///@brief 判断线段与线段是否相交
///@param[in] aa,bb--线段1两个端点， cc,dd--线段1两个端点
///@pre 
///@return true---在线段上， false---不在线段上
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 17:35
bool segmentLineIsIntersect( const cocos2d::CCPoint& aa, const cocos2d::CCPoint& bb, const cocos2d::CCPoint& cc, const cocos2d::CCPoint& dd )
{
	/// 以两条线段形成的矩形不重合，说明两条线段必然不相交
	if (!isRectsInterSect(aa, bb, cc, dd))
	{
		return false;
	}

	/// 必须二者互相跨立 注意"="的情况。
	if (0 < vectorProduct(aa.x-cc.x, aa.y-cc.y, dd.x-cc.x, dd.y-cc.y) * 
		vectorProduct(dd.x-cc.x, dd.y-cc.y, bb.x-cc.x, bb.y-cc.y) &&
		0 < vectorProduct(cc.x-aa.x, cc.y-aa.y, bb.x-aa.x, bb.y-aa.y) * 
		vectorProduct(bb.x-aa.x, bb.y-aa.y, dd.x-aa.x, dd.y-aa.y))
	{
		return true;
	}
	return false; 
}

///@brief 判断点是否在多边形（包括点在边上）
///@param[in] p0--要判断点， poly--多边形点集合， numberOfPoints--多边形点数量
///@return true---点在多边形内， false---点不在多边形内
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 17:07
bool pointIsInPolygon( const cocos2d::CCPoint& p0, cocos2d::CCPoint* poly, const unsigned int& numberOfPoints )
{
	unsigned int count  = 0;		///< 用来标记射线L与多边形的交点数；
	cocos2d::CCSize	winsize = CCDirector::sharedDirector()->getWinSize();
	/// 已点p0向左向右做一条射线L；
 	CCPoint leftPoint = ccp(-100.f, p0.y);
 	CCPoint rightPoint = p0;


	/// 判断每条边
	for (unsigned int i = 0; i < numberOfPoints-1; i++)
	{
		/// 先判断点p0是否在边s上；
		if (pointIsAtLine(p0, poly[i], poly[(i+1)%(numberOfPoints)]))
		{
/*			CCLOG("Point is at the %dth line", i);*/

			return true;
		}

		/// 判断边s是否是平行线；
		if (poly[i].y != poly[(i+1)%(numberOfPoints)].y)
		{		
			do 
			{
				/// 判断边s的是否有端点在L上 同时 再判断该点是否是边s纵坐标较大的一个点
				if (pointIsAtLine(poly[i], leftPoint, rightPoint))
				{
					if (poly[i].y > poly[(i+1)%(numberOfPoints)].y)
					{
						count += 1;
					}
					break;
				}	
				if (pointIsAtLine(poly[(i+1)%(numberOfPoints)], leftPoint, rightPoint))
				{
					if (poly[i].y < poly[(i+1)%(numberOfPoints)].y)
					{
						count += 1;
					}

					break;
				}	

				/// 如果边s没有端点在L上，则判断s与L是否相交
				if (segmentLineIsIntersect(leftPoint, rightPoint, poly[i], poly[(i+1)%(numberOfPoints)]))
				{
					count += 1;
				}	
			} while (0);
		}
	}
	if (count%2 == 1)
	{
//		CCLOG("true");
		return true;
	}
	else
	{
//		CCLOG("false");
		return false;
	}

// 	if (1 == count%2)
// 	{
// /*		CCLOG("Point is in polygon!");*/
// 		return true;
// 	}
// 	else
// 	{
// /*		CCLOG("Point is in polygon!");*/
// 		return false;
// 	}
}

///@brief 判断点是否在园内
///@param[in/out] 
///@pre 前者条件 
///@return true---点在园内， false--点在多边形内
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-7-28 17:29
bool pointInInCircle( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& r0, const float& radius )
{
	if (ccpDistance(p0, r0) < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void onWordVisible( cocos2d::CCSprite* node )
{
	CCActionInterval*  action = CCFadeOut::create(1.f);
	node->stopAllActions();
	node->setVisible(true);
	node->setOpacity(255);
	node->runAction( CCSequence::create( CCDelayTime::create(1.0f), action,  NULL));
}
