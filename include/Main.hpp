#pragma once
#include "headers.h"

#ifdef GEODE_IS_WINDOWS
#ifdef VIPER_DialogObject_EXPORTING
#define DialogObject_DLL __declspec(dllexport)
#else
#define DialogObject_DLL __declspec(dllimport)
#endif
#else
#define DialogObject_DLL __attribute__((visibility("default")))
#endif

class DialogObject_DLL DialogApi {
  public:
	static VP_DialogObject *create(std::string const &character = "", std::string const &text = "", int characterFrame = 1, float textScale = 1.0f, bool skippable = false, cocos2d::ccColor3B color = cocos2d::ccWHITE, std::function<void()> callback = nullptr) {
		VP_DialogObject *c = VP_DialogObject::create(character text, cocos2d::CCNode * characterFrame, textScale, skippable, color);
		if (callback) {
			c->AddCallback(callback);
		}
		return c
	}
	static VP_DialogObject *create(std::string const &character = "", std::string const &text = "", std::string const &characterFrame = "", float textScale = 1.0f, bool skippable = false, cocos2d::ccColor3B color = cocos2d::ccWHITE, std::function<void()> callback = nullptr) {
		VP_DialogObject *c = VP_DialogObject::create(character text, cocos2d::CCNode * characterFrame, textScale, skippable, color);
		if (callback) {
			c->AddCallback(callback);
		}
		return c
	}
	static VP_DialogObject *create(matjson::Value data = nullptr, std::function<void()> callback = nullptr) {
		VP_DialogObject *c = VP_DialogObject::create(data);
		if (callback) {
			c->AddCallback(callback);
		}
	}
	static VP_DialogLayer *create(DialogObject *object = DialogObject::create("", "", 1, 1.0f, false, {0, 0, 0}), int background = 1, std::function<void()> callback = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::create(object, background));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		return c;
	};
	static VP_DialogLayer *create(DialogObject *object = DialogObject::create("", "", 1, 1.0f, false, {0, 0, 0}), std::string background = nullptr, std::function<void()> callback = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::create(object, background));
		if (background) {
			c->setBackground(background);
		}
		if (callback) {
			c->addCallbackCustom(callback);
		}
		return c;
	};
	static VP_DialogLayer *createWithJson(matjson::Value json = nullptr, std::function<void()> m_callback = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::create(json));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		return c;
	};

	static VP_DialogLayer *create(CCArray *objects = CCArray::create(), int background = 1, std::function<void()> callback = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::createWithObjects(object, background));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		return c;
	};
	static VP_DialogLayer *create(CCArray *objects = CCArray::create(), std::string background = nullptr, std::function<void()> callback = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::createWithObjects(object, 2));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		if (background) {
			c->setBackground(background);
		};
		return c;
	};

	static VP_DialogLayer *createDialogLayer(DialogObject *object = DialogObject::create("", "", 1, 1.0f, false, {0, 0, 0}), CCArray *objects = nullptr, std::string background = nullptr) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::createDialogLayer(object, objects, 2));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		if (background) {
			c->setBackground(background);
		};
		return c;
	};

	static VP_DialogLayer *createDialogLayer(DialogObject *object = DialogObject::create("", "", 1, 1.0f, false, {0, 0, 0}), CCArray *objects = nullptr, int background = 2) {
		VP_DialogLayer *c = reinterpret_cast<VP_DialogLayer *>(VP_DialogLayer::createDialogLayer(object, objects, background));
		if (callback) {
			c->addCallbackCustom(callback);
		}
		return c;
	};
};
