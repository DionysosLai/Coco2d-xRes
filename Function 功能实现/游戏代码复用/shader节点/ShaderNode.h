/**Copyright (c) 2014, 广东小天才科技有限公司
*All rights reserved.
*@file ShaderNode.h
*@brief 添加Shader文件
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-9-5 16:32
*/
#include "cocos2d.h"

class ShaderNode : public cocos2d::CCNode
{
public:
	ShaderNode();
	~ShaderNode();

	bool initWithVertex(const char *vert, const char *frag);
	void loadShaderVertex(const char *vert, const char *frag);
	void listenBackToForeground(cocos2d::CCObject *obj);

	virtual void update(float dt);
	virtual void setPosition(const cocos2d::CCPoint &newPosition);
	virtual void draw();

	static ShaderNode* shaderNodeWithVertex(const char *vert, const char *frag);

private:

	cocos2d::ccVertex2F m_center;
	cocos2d::ccVertex2F m_resolution;
	float      m_time;
	GLuint     m_uniformCenter, m_uniformResolution, m_uniformTime;
	std::string m_vertFileName;
	std::string m_fragFileName;
};
