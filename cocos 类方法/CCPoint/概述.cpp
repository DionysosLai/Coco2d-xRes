2014-03-21

CCPoint 常用方法：
1. 相反值ccpAdd(const CCPoint& v1, const CCPoint& v2)
{
    return v1 + v2;
}

2. 相减值ccpSub(const CCPoint& v1, const CCPoint& v2)
{
    return v1 - v2;
}

3. 相乘值ccpMult(const CCPoint& v, const float s)
{
    return v * s;
}

4. 中点值ccpMidpoint(const CCPoint& v1, const CCPoint& v2)
{
    return (v1 + v2) / 2.f;
}

5. 点乘ccpDot(const CCPoint& v1, const CCPoint& v2)
{
    return v1.dot(v2);
}

6. 计算两点间距离float CC_DLL ccpDistance(const CCPoint& v1, const CCPoint& v2);

7. 计算原点到点之间的距离，float CC_DLL ccpLength(const CCPoint& v);

8、获得某点的单位向量ccpNormalize(const CCPoint& v)
{
    return v.normalize();
}

9. 获得两点之间的方向向量ccpNormalize(ccpSub(centerPoint, currentPoint))