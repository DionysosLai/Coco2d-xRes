//////////////////////////////////////////////////////////////////////////
// 随机思想：、
// 
// 
// Demand：有几张卡片，卡片内容是两两一对，要求我们随机摆放卡片
// Idea：  将这几个卡片，按顺序摆放成一个数组，如下，然后每次打乱数组，再按数组顺序摆放卡片内容；
//////////////////////////////////////////////////////////////////////////
// 洗牌---打乱数组
static GT_VOID HZ2HfcGInitCard(GT_S16 status)
{
	GT_S16 i, temp, index;
	GT_S16 array[8] = {0, 1, 2, 3, 0, 1, 2, 3};
	
	for (i = HZ2_HFCG_EASY_CARD_NUM -1 ; i > 0; i-- )	// 洗牌
	{
		index = gam_rand() % i;
		temp = array[i];
		array[i] = array[index];
		array[index] = temp;
	}
}

//////////////////////////////////////////////////////////////////////////
// 如何得到一个分布在（0,1）的随机数
/////////////////////////////////////////////////////////////////////////
static GT_FLT fRand()
{
	return  ((GT_FLT)gam_rand() ) / RAND_MAX;
}

//////////////////////////////////////////////////////////////////////////
// 随机选取数据；
// Demand：假设我们有一个集合A(a_1,…,a_n), 对于数m，0≤m≤n, 如何从集合A中等概率地选取m个元素呢？
// Demand1：1.1 假设集合A中的元素在各个位置上不具有随机性， 比如已经按某种方式排序了，
//           那么我们可以遍历集合A中的每一个元素a_i, 0<=n 根据一定的概率选取ai。如何选择这个概率呢？
// Idea：设m’为还需要从A中选取的元素个数， n’为元素a_i及其右边的元素个数， 也即n’=(n-i+1)。那么选取元素a_i的概率为 m’/n’。
//////////////////////////////////////////////////////////////////////////
bool getRand(const vector<T> vecData, int m, vector<T>& vecRand)
{
	GT_S16 nSize = ArrLen(vecData);	// 获取数组的长度
	GT_S16 i, iSize;
	GT_FLT fRandom;

	if(nSize  < m || m < 0)
		return false;

	for(i = 0, isize = nSize; i < isize ; i++)
	{
		fRandom = fRand();
		if(fRand <=(float)(m)/nSize)
		{
			vecRand.push_back(vecData[i]);
			m--;
		}
		nSize --;
	}
	return true;
}

// Demand2: 在有些情况下，我们不能直接得到A的元素个数。( 我们小机实现不了)
// Idea2: 1) 取前m个元素放在集合A’中。
//		  2) 对于第i个元素(i>m), 使i在 m/i的概率下， 等概率随机替换A’中的任意一个元素。直到遍历完集合。	
//		  3) 返回A’
public void reduce(TextPair key, Iterator value, OutputCollector collector, int m)
{
	Text[] vecData = new Text[m];
	int nCurrentIndex = 0;
	while(value.hasNext())
	{
		Text tValue = value.next();
		if(nCurrentIndex < m)
		{
			vecData[nCurrentIndex] = tValue;	// 取前m个元素放在集合A’中。
		}
		else if(frand() < (float)m / (nCurrentIndex+1)) 
		{
			int nReplaceIndex = (int)(frand() * m);		// 对于第i个元素(i>m), 使i在 m/i的概率下， 等概率随机替换A’中的任意一个元素
			vecData[nReplaceIndex] = tValue;
		}
		nCurrentIndex ++;
	}
}


	