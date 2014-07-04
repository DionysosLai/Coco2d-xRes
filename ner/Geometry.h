/**
*@file Geometry.h
*@brief 一些常用几何算法
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-7-28 16:24
*/

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "cocos2d.h"

///@brief 叉积
///@param[in/out] 
///@pre 前者条件 
///@return 
double vectorProduct(const double& x1, const double& y1, const double& x2, const double& y2);  // 行列式  

///@brief 判断折线的拐向，
///以线段P1为参考，；
///@param[in] p1--线段1，p2--线段2
///@pre P1、P1必须有公共端点
///@return 1--左边 0 -- 共线 -1---右边
int polyLineDerection(const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p2);

///@brief 判断线段与直线是否相交
///@param[in] p0,p1--线段两个端点， q0,q1--直线两个点值
///@pre 
///@return true---在线段上， false---不在线段上
bool straightLineIsIntersect(const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& q0, const cocos2d::CCPoint& q1);

///@brief 判断点是否在矩形中s
///@param[in|out] p0--点 r1--矩形中心 width--矩形宽 heigth---矩形高
///@pre 
///@return true--在矩形内， false---在矩形外
bool pointIsInRect( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& r1, const float& width, const float& heigth );

///@brief 判断点是否在矩形内
///@param[in] p0--点，p1,p2--矩形的一条对角线端点
///@pre 
///@return true---在线段上， false---不在线段上
bool pointIsInRect( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2 );

///@brief 点到线段最近一个点
///@param[in] p0--要判断点， s0，s1--线段两个端点
///@return crosspoint---最近的点
cocos2d::CCPoint nearestPointToSegmentLine( const cocos2d::CCPoint& p0, const cocos2d::CCPoint& s0, const cocos2d::CCPoint& s1 );

///@brief 判断两个矩形是否相交
///@param[in] aa,bb--矩形1一个对角线端点， cc,dd--矩形2一个对角线端点
///@pre 
///@return true---相交， false---不相交
bool isRectsInterSect(const cocos2d::CCPoint& aa, const cocos2d::CCPoint& bb, const cocos2d::CCPoint& cc, const cocos2d::CCPoint& dd);

///@brief 判断点是否在线段上
///@param[in] p0--点，p1,p2--线段两个端点
///判断点是否在线段上：
///设点为Q，线段为P1P2 ，判断点Q在该线段上的依据是：( Q - P1 ) × ( P2 - P1 ) = 0 
///且 Q 在以 P1，P2为对角顶点的矩形内。前者保证Q点在直线P1P2上，后者是保证Q点不在
///线段P1P2的延长线或反向延长线上
///@return true---在线段上， false---不在线段上
bool pointIsAtLine(const cocos2d::CCPoint& p0, const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2);

///@brief 判断点是否在多边形（包括点在边上）///@param[in] p0--要判断点， poly--多边形点集合， numberOfPoints--多边形点数量///@return true---点在多边形内， false---点不在多边形内
bool pointIsInPolygon(const cocos2d::CCPoint& p0, cocos2d::CCPoint* poly, const unsigned int& numberOfPoints);

///@brief 判断点是否在园内
///@param[in/out] 
///@pre 前者条件 
///@return true---点在园内， false--点在多边形内
bool pointInInCircle(const cocos2d::CCPoint& p0, const cocos2d::CCPoint& r0, const float& radius);

///@brief 判断线段与线段是否相交
///@param[in] aa,bb--线段1两个端点， cc,dd--线段1两个端点
///@pre 
///@return true---在线段上， false---不在线段上
bool segmentLineIsIntersect(const cocos2d::CCPoint& aa, const cocos2d::CCPoint& bb, const cocos2d::CCPoint& cc, const cocos2d::CCPoint& dd);

#endif	///<(__GEOMETRY_H__)