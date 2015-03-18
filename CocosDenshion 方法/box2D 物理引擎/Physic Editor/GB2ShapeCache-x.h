//
//  GB2ShapeCache-x.cpp
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef GB2ShapeCache_x_h
#define GB2ShapeCache_x_h

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class BodyDef;
class b2Body;

namespace cocos2d {		
	struct Vec
	{
		b2Vec2 vec[8];
		unsigned int num;
	};
	class GB2ShpaePrimalCache
	{
	public:
		unsigned int m_uiShapeType;		///< 形状类型
		unsigned short m_usCategoryBits;
		unsigned short m_usMaskBits;
		signed short m_sGroupIndex;
		float m_fFriction;
		float m_fDensity;
		float m_fRestitution;
		bool m_bisSensor;
		std::vector<Vec*> vertices;		///< 多边形数据
		float m_radius;						///< 圆形数据
		b2Vec2 m_p;
		int m_iCallbackData;				///< 非box2d数据，自定义数据
	};

	class GB2ShapeCache 
	{
	public:
		// Static interface
		static GB2ShapeCache* sharedGB2ShapeCache(void);
		
	public:
		bool init();			
		void addShapesWithFile(const std::string &plist);
		void addFixturesToBody(b2Body *body, const std::string &shape);
///@brief 绑定对象的物理模型
///@param[in] scale---物理模型的比例
///@return 
		void addFixturesToBody(b2Body *body, const std::string &shape, const float& scale);
		cocos2d::CCPoint anchorPointForShape(const std::string &shape);
		void reset();
		float getPtmRatio() { return ptmRatio; }
		~GB2ShapeCache() {}
		
	private:
		std::map<std::string, BodyDef *> shapeObjects;
		std::map<std::string, GB2ShpaePrimalCache *> shapePrimalCaches;
		GB2ShapeCache(void) {}
		float ptmRatio;
	};
}
#endif
