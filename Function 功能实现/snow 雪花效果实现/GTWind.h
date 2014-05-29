#ifndef _GTWIND_H__
#define _GTWIND_H__

#include "cocos2d.h"

USING_NS_CC;

enum{
	tag_batch_node=0,
	tag_wind_none=1,
	tag_wind_left=2,
	tag_wind_right=3
};

class GTWind : public CCNode
{	
public:
	GTWind(void);
	~GTWind(void);
	void showWind(void);

	void changeWind(float dt);
	void updataWind(float dt);
	void upDataWindVelocity(float dt);
protected:

private:
	int		m_windDir;			///< ??????
	int		g;					///< ???;
	int		maxV;				///< ?????????
	int		maxCount;			///< ?????
	int		windVelocity;		///< ??(?????,????????);
	int		maxWindVelocity;	///< ????(????0)
	int		m_fSnowVelocityX;	///< ????(????????)
	CCSize	winSize;			///< ????
};
#endif