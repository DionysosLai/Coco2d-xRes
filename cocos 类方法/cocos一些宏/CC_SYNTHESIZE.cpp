2014-04-01
#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

CC_SYNTHESIZE(int, nTest, Test) 相当于干了下面这些事： 
 
protected: int nTest; 
public: virtual nTest getTest(void) const { return nTest; } 
public: virtual void setTest(int var){ nTest = var; }  