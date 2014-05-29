2014-02-25
	在游戏中，有时候为了获得用户账号、密码，需要调用虚拟键盘来实现。
在coco2d-x中，通过调用继承输入法代理类CCIMEDelegate和其字体标签类CCLableTTF
的字体输入框类CCTextFileTTF来实现虚拟键盘功能。
class CC_DLL CCIMEDelegate
{
public:
    virtual ~CCIMEDelegate();
	/// 打开键盘并允许输入
    virtual bool attachWithIME();
	/// 关闭键盘并停止输入
    virtual bool detachWithIME();

protected:
    friend class CCIMEDispatcher;

    /**
    @brief    Decide if the delegate instance is ready to receive an IME message.
	决定是否准备好接收键盘输入；
    Called by CCIMEDispatcher.
    */
    virtual bool canAttachWithIME() { return false; }
    /**
    @brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
	当准备好接收输入是被调用
    */
    virtual void didAttachWithIME() {}

    /**
    @brief    Decide if the delegate instance can stop receiving IME messages.
	决定是否准备好结束键盘输入
    */
    virtual bool canDetachWithIME() { return false; }

    /**
    @brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
	当准备好结束输入时别调用
    */
    virtual void didDetachWithIME() {}

    /**
    @brief    Called by CCIMEDispatcher when text input received from the IME.
	当输入文字时被调用
    */
    virtual void insertText(const char * text, int len) {CC_UNUSED_PARAM(text);CC_UNUSED_PARAM(len);}

    /**
    @brief    Called by CCIMEDispatcher after the user clicks the backward key.
	当按下回格（Backspace）键是被调用
    */
    virtual void deleteBackward() {}

    /**
    @brief    Called by CCIMEDispatcher for text stored in delegate.
	获得当前文字
    */
    virtual const char * getContentText() { return 0; }

    //////////////////////////////////////////////////////////////////////////
    // keyboard show/hide notification
    //////////////////////////////////////////////////////////////////////////
	/// 键盘将要显示时被调用
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
	/// 键盘已经显示时被调用
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
	/// 键盘将要隐藏时被调用
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
	/// 键盘已经隐藏时被调用
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}

protected:
    CCIMEDelegate();
};

class CC_DLL CCTextFieldTTF : public CCLabelTTF, public CCIMEDelegate
{
public:
    /**
     *  @lua NA
     */
    CCTextFieldTTF();
    /**
     *  @lua NA
     */
    virtual ~CCTextFieldTTF();

    //char * description();

    /** creates a CCTextFieldTTF from a fontname, alignment, dimension and font size */
    static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    /** creates a CCLabelTTF from a fontname and font size */
    static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
	/** initializes the CCTextFieldTTF with a font name, alignment, dimension and font size 
	* 初始化字体等信息；	
	*/
    bool initWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    /** initializes the CCTextFieldTTF with a font name and font size */
    bool initWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

    /**
    @brief    Open keyboard and receive input text. 打开键盘并允许输入；
    */
    virtual bool attachWithIME();

    /**
    @brief    End text input and close keyboard. 关闭键盘并停止输入；
    */
    virtual bool detachWithIME();

    //////////////////////////////////////////////////////////////////////////
    // properties
    //////////////////////////////////////////////////////////////////////////

    CC_SYNTHESIZE(CCTextFieldDelegate *, m_pDelegate, Delegate);
    CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
	/// 获得动画显示字体颜色
    virtual const ccColor3B& getColorSpaceHolder();
	/// 设置动画显示字体颜色
    virtual void setColorSpaceHolder(const ccColor3B& color);

    // input text property
public:
	/// 设置将要显示的文字
    virtual void setString(const char *text);
	/// 获得显示的文字；
    virtual const char* getString(void);
protected:
    std::string * m_pInputText;

    // place holder text property
    // place holder text displayed when there is no text in the text field.
public:
	/// 设置默认字符
    virtual void setPlaceHolder(const char * text);
	/// 获得默认字符
    virtual const char * getPlaceHolder(void);
protected:
    std::string * m_pPlaceHolder;
    ccColor3B m_ColorSpaceHolder;
public:
    virtual void setSecureTextEntry(bool value);
    virtual bool isSecureTextEntry();
protected:
    bool m_bSecureTextEntry;
protected:
	/// 绘制
    virtual void draw();

    //////////////////////////////////////////////////////////////////////////
    // CCIMEDelegate interface
    //////////////////////////////////////////////////////////////////////////
	/// 决定是否已经准备好接收输入；
    virtual bool canAttachWithIME();
	/// 决定是否已经准备好结束输入；
    virtual bool canDetachWithIME();
	/// 当输入文字时被调用
    virtual void insertText(const char * text, int len);
	/// 当回格键被按下被调用
    virtual void deleteBackward();
	/// 获得当前文字；
    virtual const char * getContentText();
private:
    class LengthStack;
    LengthStack * m_pLens;
};


	获得用户输入，首先要在场景中加入一个继承自CCIMEDelegate的可显示的节点，在TextInput文件夹下，
使用的是KeyboardNotificationLayer类，继承自CCLayer类。当这个输入框被“触摸”时，调用onClickTrackNode函数
开启输入框；


void TextFieldTTFDefaultTest::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)m_pTrackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFDefaultTest:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFDefaultTest:CCTextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}