#include <Geode/Geode.hpp>
#include <Geode/modify/CCObject.hpp>
#include "../include/Main.hpp"
#include "../include/ccDialogHooks.hpp" 
using namespace geode::prelude;
// add it to the api
VP_DialogObject* DialogApi::create(
        gd::string character,
        gd::string text,
        CCSprite* characterFrame,
        float textScale,
        bool skippable,
        cocos2d::ccColor3B color) {
        return reinterpret_cast<VP_DialogObject*>(VP_DialogObject::create(
				character,
				text,
				characterFrame, textScale, skippable, color
		));
};

VP_DialogObject* DialogApi::create(
        gd::string character,
        gd::string text,
        int characterFrame,
        float textScale,
        bool skippable,
        cocos2d::ccColor3B color) {
        return reinterpret_cast<VP_DialogObject*>(DialogObject::create(
				character,
				text,
				characterFrame, textScale, skippable, color
		));
};


// removes nodes and removes from vector table scuffed way
class $modify(FakYou,CCObject) {
    void release() {
            if (DialogObject* bfix = typeinfo_cast<DialogObject*>(this)) {
                if (this->retainCount() <= 1) {
                    //log::debug("VP_DialogObject being released: {}", bfix);
                    reinterpret_cast<VP_DialogObject*>(bfix)->byebye();
                }
            }
            CCObject::release(); 
        };
};

/*
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class $modify(CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
        if (key == enumKeyCodes::KEY_F && down) {
                auto dialog = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(VP_DialogObject::create(
                                "Example",
                                "Example",
                                CCSprite::createWithSpriteFrameName("checkpoint_01_001.png"), 1.0f, false, ccWHITE
                            ), 2));
                dialog->setBackground(3);
                dialog->updateChatPlacement(DialogChatPlacement::Center);
                dialog->animateInRandomSide();
                dialog->addToMainScene();
            }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key,down,repeat);
    }
};
*/
