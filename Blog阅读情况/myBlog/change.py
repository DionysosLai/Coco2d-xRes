#-*- encoding:utf-8 -*-   

import sys, os

def checkOption():
    argLen = len(sys.argv)
    targetPath = "./"
    if argLen == 2:
        targetPath = sys.argv[1]
        if not targetPath.endswith("/"):
            targetPath += "/"
    return targetPath
    
def replaceCocos2dxPath(path):
    filelists = os.listdir(path)
    for item in filelists:
        suffix = (os.path.splitext(item))[1]
        print(suffix)
        if suffix == ".sln":
            print('replace "..\..\..\" with %COCOS2DX_HOME% in *.sln')
            #replace "..\..\..\" with %COCOS2DX_HOME% in *.sln 
            content = open(path + item, 'r').read().replace("..\\..\\..\\", "%COCOS2DX_HOME%\\")
            file = open(path + item, 'w')
            file.write(content)
            file.close()
        if suffix == ".vcxproj":
            print('replace "$(ProjectDir)..\..\..\" with $(COCOS2DX_HOME)\ in *.vcxproj')
            #replace "$(ProjectDir)..\..\..\" with $(COCOS2DX_HOME)\ in *.vcxproj
            content = open(path + item, 'r').read().replace("$(ProjectDir)..\\..\\..\\", "$(COCOS2DX_HOME)\\")
            file = open(path + item, 'w')
            file.write(content)
            file.close()

if  __name__ == '__main__':
    targetPath = checkOption()
    replaceCocos2dxPath(targetPath)