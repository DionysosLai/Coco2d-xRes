/**Copyright (c) 2014, 广东小天才科技有限公司
*All rights reserved.
*@file ShaderNode.h
*@brief 添加Shader文件
*@author 赖武功，email: 906391500@qq.com
*@version 1.0
*@data 2014-9-5 16:34
*/
#include "ShaderNode.h"

USING_NS_CC;

enum 
{
	SIZE_X = 256,
	SIZE_Y = 256,
};

ShaderNode::ShaderNode()
	:m_center(vertex2(0.0f, 0.0f))
	,m_resolution(vertex2(0.0f, 0.0f))
	,m_time(0.0f)
	,m_uniformCenter(0)
	,m_uniformResolution(0)
	,m_uniformTime(0)
{
}

ShaderNode::~ShaderNode()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
}

ShaderNode* ShaderNode::shaderNodeWithVertex(const char *vert, const char *frag)
{
	ShaderNode *node = new ShaderNode();
	node->initWithVertex(vert, frag);
	node->autorelease();

	return node;
}

bool ShaderNode::initWithVertex(const char *vert, const char *frag)
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(ShaderNode::listenBackToForeground),
		EVENT_COME_TO_FOREGROUND,
		NULL);

	loadShaderVertex(vert, frag);

	m_time = 0;
	m_resolution = vertex2(SIZE_X, SIZE_Y);

	scheduleUpdate();

	setContentSize(CCSizeMake(SIZE_X, SIZE_Y));
	setAnchorPoint(ccp(0.5f, 0.5f));

	m_vertFileName = vert;
	m_fragFileName = frag;

	return true;
}

void ShaderNode::listenBackToForeground(CCObject *obj)
{
	this->setShaderProgram(NULL);
	loadShaderVertex(m_vertFileName.c_str(), m_fragFileName.c_str());
}

void ShaderNode::loadShaderVertex(const char *vert, const char *frag)
{
	CCGLProgram *shader = new CCGLProgram();
	shader->initWithVertexShaderFilename(vert, frag);

	shader->addAttribute("aVertex", kCCVertexAttrib_Position);
	shader->link();

	shader->updateUniforms();

	m_uniformCenter = glGetUniformLocation(shader->getProgram(), "center");
	m_uniformResolution = glGetUniformLocation(shader->getProgram(), "resolution");
	m_uniformTime = glGetUniformLocation(shader->getProgram(), "time");

	this->setShaderProgram(shader);

	shader->release();
}

void ShaderNode::update(float dt)
{
	m_time += dt;
}

void ShaderNode::setPosition(const CCPoint &newPosition)
{
	CCNode::setPosition(newPosition);
	CCPoint position = getPosition();
	m_center = vertex2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
}

void ShaderNode::draw()
{
	CC_NODE_DRAW_SETUP();

	float w = SIZE_X, h = SIZE_Y;
	GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};

	//
	// Uniforms
	//
	getShaderProgram()->setUniformLocationWith2f(m_uniformCenter, m_center.x, m_center.y);
	getShaderProgram()->setUniformLocationWith2f(m_uniformResolution, m_resolution.x, m_resolution.y);

	// time changes all the time, so it is Ok to call OpenGL directly, and not the "cached" version
/*	glUniform1f(m_uniformTime, m_time);*/

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	CC_INCREMENT_GL_DRAWS(1);
}