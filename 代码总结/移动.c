//////////////////////////////////////////////////////////////////////////
///
///  @brief 移动调试；
///  @param 无
///  @return 0--再移动中， 1-- 到达目标位置
///
///  @par 修改历史：
///  @code
///  姓名        日期          说明
///  --------    ----------    ------------------------------------------
///  曾志坚、Dionysos     2013.12.23    完成基本功能
///  @endcode
///
//////////////////////////////////////////////////////////////////////////
static GT_BOOL HZ2HfcGRoseFairyFly(GEI_S_OBJECT *pObj, GT_S16 sourceX, GT_S16 sourceY, 
	GT_S16 destX, GT_S16 destY, GT_S16 speed)
{
	GT_S16 tx, ty;
	GT_S16 lx, ly, len;
	GT_S16 Vx, Vy;
	GT_S16 V = speed;
	//GT_S16 V = HZ2_PRG_FALL_SPEED;
	/// 获得目标位置与当前位置偏移量；
	lx = destX - sourceX;		
	ly = destY - sourceY;
	/// 得到偏移长度
	len = sqrt((float)(lx*lx+ly*ly));
	if (0 != len)
	{
		/// 得到偏移速度
		Vx = V*lx/len;	
		Vy = V*ly/len;
	}
	else
	{
		Vx = 0;
		Vy = 0;
	}
	if (0 == Vy)
	{
		/// 调整偏移
		if (0 > ly)
		{
			Vy = -1;
		}
		else if (0 < ly)
		{
			Vy = 1;
		}
	}
	if (0 == Vx)
	{
		/// 调整偏移
		if (0 > lx)
		{
			Vx = -1;
		}
		else
		{
			Vx = 1;
		}
	}
	/// 得到当前位置
	tx = GEI_ObjectGetXPos(pObj);
	ty = GEI_ObjectGetYPos(pObj);
	tx += Vx;
	ty += Vy;
	if (0 > Vx)
	{
		if (destX>= tx)
		{
			tx = destX;
		}
	}
	else
	{
		if (destX <= tx)
		{
			tx = destX;
		}
	}
	/// 设置目标位置
	GEI_ObjectSetPosWithX(pObj, tx);
	if (0 > Vy)
	{
		if (destY >= ty)
		{
			ty = destY;
		}
	}
	else
	{
		if (destY <= ty)
		{
			ty = destY;
		}
	}
	/// 设置目标位置
	GEI_ObjectSetPosWithY(pObj, ty);
	if (destX == tx && destY == ty)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}