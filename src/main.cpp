#include "../include/headers.h"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

std::string getplrname() {
	// fix for coopeeo custom name
	if (Mod *coopeeo_customname = Loader::get()->getLoadedMod("coopeeo.customname")) {
		return coopeeo_customname->getSettingValue<std::string>("thename");
	}
	return GameManager::get()->m_playerName;
}

CCSprite *IsSprite(std::string def) {
	const char *str = def.c_str();
	CCSprite *sprite = CCSprite::create(str);
	if (sprite) {
		return sprite;
	}
	sprite = CCSprite::createWithSpriteFrameName(str);
	if (sprite) {
		return sprite;
	}

	return nullptr;
}

VP_DialogObject *VP_DialogObject::create(std::string const &character, std::string const &text, int characterFrame, float textScale, bool skippable, cocos2d::ccColor3B color) {
	auto ret = new VP_DialogObject();
	if (ret->init(character, text, characterFrame, textScale, skippable, color)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

VP_DialogObject *VP_DialogObject::create(std::string const &character, std::string const &text, CCNode *characterFrame, float textScale, bool skippable, cocos2d::ccColor3B color) {
	auto ret = new VP_DialogObject();
	if (ret->init(character, text, 0, textScale, skippable, color)) {
		ret->autorelease();
		if (characterFrame)
			ret->CustomChar = characterFrame;
		return ret;
	}
	delete ret;
	return nullptr;
}

VP_DialogObject *VP_DialogObject::create(matjson::Value data) {
	std::string title = "Unknown";
	if (data.contains("title") && data["title"].isString())
		title = data["title"].asString().unwrapOr("Unknown");
	std::string message = "N/A";
	if (data.contains("message") && data["message"].isString())
		message = data["message"].asString().unwrapOr("N/A");

	// from https://github.com/FigmentBoy/DialogHandler/blob/master/DialogBox/Dialog.cpp?plain=1#L31
	int index;
	int pos = 0;
	std::string plrname;

	while ((index = message.find("{user}", pos)) != std::string::npos) {
		if (plrname.empty()) {
			plrname = getplrname();
		}
		message.replace(index, 6, plrname);

		pos = index + 1;
	}

	while ((index = title.find("{user}", pos)) != std::string::npos) {
		if (plrname.empty()) {
			plrname = getplrname();
		}
		title.replace(index, 6, plrname);

		pos = index + 1;
	}
	// end of copying

	if (data.contains("portrait")) {
		if (data["portrait"].isNumber()) {
			return VP_DialogObject::create(title, message, data["portrait"].asUInt().unwrapOr(1), 1.0f, false, ccWHITE);
		} else if (data["portrait"].isString()) {
			if (auto spr = IsSprite(data["portrait"].asString().unwrapOr(""))) {
				return VP_DialogObject::create(title, message, spr, 1.0f, false, ccWHITE);
			} else {
				return VP_DialogObject::create(title, message, 1, 1.0f, false, ccWHITE);
			}
		} else {
			return VP_DialogObject::create(title, message, 1, 1.0f, false, ccWHITE);
		}
	} else {
		return VP_DialogObject::create(title, message, 1, 1.0f, false, ccWHITE);
	}
	return VP_DialogObject::create("uh oh", "woops", 1, 1.0f, false, ccWHITE);
}

void VP_DialogObject::AddCallback(std::function<void()> callback) {
	this->callbackOnObject = callback;
};

void VP_DialogLayer::addCallbackCustom(std::function<void()> m_callback) {
	m_DelegateCallBack *newdelgate = new m_DelegateCallBack;
	this->addChild(newdelgate);
	newdelgate->autorelease();
	newdelgate->m_callback = m_callback;
	this->m_delegate = newdelgate;
}

void VP_DialogLayer::setBackground(std::string backgroundBG) {
	this->m_fields->m_BackgroundOverride = backgroundBG;
	if (!this->m_mainLayer)
		return;

	if (!this->m_fields->m_BackgroundSpr) {
		this->m_fields->m_BackgroundSpr = m_mainLayer->getChildByType<CCScale9Sprite *>(0);
	}
	if (this->m_fields->m_BackgroundSpr) {
		cocos2d::CCSprite *sprite = IsSprite(backgroundBG);
		if (sprite) {
			cocos2d::CCSize ContentSize = this->m_fields->m_BackgroundSpr->getContentSize();
			cocos2d::CCSpriteFrame *spriteFrame = sprite->displayFrame();
			this->m_fields->m_BackgroundSpr->setSpriteFrame(spriteFrame);
			this->m_fields->m_BackgroundSpr->setContentSize(ContentSize);
			this->m_fields->m_BackgroundAlreadySet = backgroundBG;
		}
	}
}

void VP_DialogLayer::setBackground(int backgroundBG) {
	std::string num = "";

	if (backgroundBG < 10) {
		num = fmt::format("0{}", backgroundBG);
	} else {
		num = backgroundBG;
	};

	VP_DialogLayer::setBackground(fmt::format("GJ_square{}.png", num));
};

void m_DelegateCallBack::dialogClosed(DialogLayer *p0) {
	if (this->m_callback) {
		this->m_callback();
	}
};
DialogLayer* VP_DialogLayer::create(matjson::Value data) {
        std::pair<CCArray*, int> letsgo;
        letsgo.second = 2;
        letsgo.first = CCArray::create();
        if (!data.isArray() && !data.isObject()) {
            letsgo.first->addObject(DialogObject::create("uh oh","it seems like the json provided is invalid",2,1.0f,false,ccWHITE));
            return VP_DialogLayer::createWithObjects(letsgo.first,letsgo.second);
        }
     if (data.contains("lines") && data["lines"].isArray()) {
            for (const auto lols : data["lines"].asArray().unwrap()) {
                letsgo.first->addObject(VP_DialogObject::create(lols));
            }
     }
    if (letsgo.first->count() < 1)  letsgo.first->addObject(DialogObject::create("uh oh","it seems lines is empty",2,1.0f,false,ccWHITE));
    
    if (data.contains("color")) {
        if ( data["color"].isNumber()) {
        letsgo.second = data["color"].asInt().unwrapOr(2);
        } else if (data["color"].isString()) {
            DialogLayer* x = VP_DialogLayer::createWithObjects(letsgo.first,letsgo.second);
            reinterpret_cast<VP_DialogLayer*>(x)->setBackground(data["color"].asString().unwrapOr("0"));
            return x;
        }
    }
    return VP_DialogLayer::createWithObjects(letsgo.first,letsgo.second);

}
bool VP_DialogLayer::init(DialogObject *p0, cocos2d::CCArray *p1, int p2) {
	if (!DialogLayer::init(p0, p1, p2))
		return false;

	this->m_fields->m_pfpPosFixer = CCNode::create();
	this->m_fields->m_pfpPosFixer->setPositionX(-143);
	m_mainLayer->addChild(this->m_fields->m_pfpPosFixer);

	return true;
}
void VP_DialogLayer::displayDialogObject(DialogObject *p0) {
	DialogLayer::displayDialogObject(p0);
	if (!this->m_fields->m_BackgroundOverride.empty() && !(this->m_fields->m_BackgroundAlreadySet == this->m_fields->m_BackgroundOverride)) {
		VP_DialogLayer::setBackground(this->m_fields->m_BackgroundOverride);
	};
	if (this->m_characterSprite) {
		if (auto x = this->m_fields->m_cur_Customprofile) {
			x->removeFromParent();
		};
	}
	if (VP_DialogObject *dialog = typeinfo_cast<VP_DialogObject *>(p0)) {
		if (dialog->CustomChar) {
			this->m_fields->m_cur_Customprofile = dialog->CustomChar;
			this->m_fields->m_pfpPosFixer->addChild(dialog->CustomChar);
		}
		if (dialog->callbackOnObject) {
			VP_DialogLayer::addCallbackCustom(dialog->callbackOnObject);
		}
	}
}

#ifndef GITHUB_ACTIONS
#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
		if (key == enumKeyCodes::KEY_F && down) {
			auto dialogLines = CCArray::create();
			dialogLines->addObject(VP_DialogObject::create(
			    "Example",
			    "Example",
			    1, 1.0f, false, ccWHITE));
			dialogLines->addObject(VP_DialogObject::create(
			    "Example",
			    "Example",
			    CCSprite::create("dialogIcon_001.png"), 1.0f, false, ccWHITE));
			auto dialog = DialogLayer::createWithObjects(dialogLines, 2);
			dialog->updateChatPlacement(DialogChatPlacement::Center);
			dialog->animateInRandomSide();
			dialog->addToMainScene();
		}
		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
	}
};
#endif;