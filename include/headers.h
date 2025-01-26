#pragma once
#include <Geode/modify/DialogLayer.hpp>
#include <Geode/modify/DialogObject.hpp>

class VP_DialogObject : public DialogObject {
  public:
	static VP_DialogObject *create(std::string const &character, std::string const &text, int characterFrame, float textScale, bool skippable, cocos2d::ccColor3B color);
	static VP_DialogObject *create(std::string const &character, std::string const &text, cocos2d::CCNode *characterFrame, float textScale, bool skippable, cocos2d::ccColor3B color);
	static VP_DialogObject *create(matjson::Value data);

	void AddCallback(std::function<void()> callback);

	geode::prelude::Ref<cocos2d::CCNode> CustomChar;
	std::function<void()> callbackOnObject;
};

class m_DelegateCallBack : public DialogDelegate, public cocos2d::CCNode {
  public:
	std::function<void()> m_callback;
	virtual void dialogClosed(DialogLayer *p0);
};

class $modify(VP_DialogLayer, DialogLayer) {
	struct Fields {
		CCNode *m_cur_Customprofile;
		geode::prelude::CCScale9Sprite *m_BackgroundSpr;
		std::string m_BackgroundOverride;
		CCNode *m_pfpPosFixer;
		std::string m_BackgroundAlreadySet;
	};
    static DialogLayer* create(matjson::Value data);
	bool init(DialogObject *p0, cocos2d::CCArray *p1, int p2);
	void addCallbackCustom(std::function<void()> m_callback);
	void setBackground(std::string backgroundBG);
	void setBackground(int backgroundBG);
	void displayDialogObject(DialogObject *p0);
};