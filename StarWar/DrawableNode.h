#ifndef _STARWAR_DRAWABLENODE_H_
#define _STARWAR_DRAWABLENODE_H_

#include "Node.h"

class CDrawableNode : public CNode
{
public:
	virtual void Update() = 0;
	virtual bool InitPosition() = 0;
	virtual bool InitVertices() = 0;
	virtual bool InitTextures() = 0;
	virtual bool InitNormals() = 0;

private:
	virtual void Draw() = 0;
};

#endif