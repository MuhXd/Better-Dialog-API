#pragma once
#include <Geode/modify/DialogObject.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <Geode/Geode.hpp>
#include <stack>
#include <sstream>
#include <string>

using namespace geode::prelude;
// m_fields but for ccobject scuffed method
// if someone wants to see this class then sure ig? i don't mind
namespace Viper_funnyutils {
    static CCSprite* IsSprite(std::string def) {
        const char* str = def.c_str();
        CCSprite* sprite = CCSprite::create(str);
        if (sprite) {
            return sprite;
        }
        sprite = CCSprite::createWithSpriteFrameName(str);
         if (sprite) {
            return sprite;
         }

        sprite = CCSprite::createWithSpriteFrameName("dialogIcon_001.png");
         if (sprite) {
            return sprite;
        }
        return nullptr;
        
    }
    /*
        example of the json file
        {
            "color": 2,
            "lines": [
                {
                    "portrait": 12,
                    "title": "Rick",
                    "message": "We're no strangers to love, {user}!"
                },
                {
                    "portrait": 13,
                    "title": "Rick",
                    "message": "You know the rules, and so do I!"
                },
            ]
        }
    */
   // this function is used to gen a background and text
    static std::pair<CCArray*, int> readjsonData(matjson::Value data);
    std::string getTextureName(CCSprite* sprite) {
     std::string texturePath = "";
     if (auto textureProtocol = typeinfo_cast<CCTextureProtocol*>(sprite)) {
        if (auto texture = textureProtocol->getTexture()) {
            auto* cachedTextures = CCTextureCache::sharedTextureCache()->m_pTextures;
            for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(cachedTextures)) {
                if (obj == texture) {
                    texturePath= key.c_str();
                    break;
                }
            }
        }
     }
    return texturePath;
};

 static std::vector<std::pair<DialogObject*, std::string>> FucktheM_fieldsInCCObjects_DialogObject;
    std::string spriteToString(CCSprite* sprite) {
        if (!sprite) return "";

        std::stringstream ss;

        ss << "SpriteStart\n";
        ss << "Texture=" << Viper_funnyutils::getTextureName(sprite) << "\n"; 
        ss << "TextureRect=" << sprite->getTextureRect().origin.x << "," << sprite->getTextureRect().origin.y << ","
        << sprite->getTextureRect().size.width << "," << sprite->getTextureRect().size.height << "\n";
        ss << "Position=" << sprite->getPositionX() << "," << sprite->getPositionY() << "\n";
        ss << "AnchorPoint=" << sprite->getAnchorPoint().x << "," << sprite->getAnchorPoint().y << "\n";
        ss << "RotationX=" << sprite->getRotationX() << "\n";
        ss << "RotationY=" << sprite->getRotationY() << "\n";
        ss << "ScaleX=" << sprite->getScaleX() << "\n";
        ss << "ScaleY=" << sprite->getScaleY() << "\n";
        ss << "Opacity=" << (int)sprite->getOpacity() << "\n";
        ss << "Color=" << (int)sprite->getColor().r << "," << (int)sprite->getColor().g << "," << (int)sprite->getColor().b << "\n";

        // Serialize children
        if (sprite->getChildrenCount() > 0) {
            ss << "ChildrenStart\n";
            CCArray* children = sprite->getChildren();
            for (unsigned int i = 0; i < children->count(); ++i) {
                CCNode* child = dynamic_cast<CCNode*>(children->objectAtIndex(i));
                CCSprite* childSprite = dynamic_cast<CCSprite*>(child);
                if (childSprite) {
                    ss << spriteToString(childSprite);
                }
            }
            ss << "ChildrenEnd\n";
        }

        ss << "SpriteEnd\n";
        return ss.str();
    }
    CCSprite* stringToSprite(const std::string& data) {
        std::istringstream ss(data);
        std::string line;
        CCSprite* sprite = nullptr;

        std::stack<CCSprite*> spriteStack;
        CCSprite* currentSprite = nullptr;

        while (std::getline(ss, line)) {
            if (line == "SpriteStart") {
                CCSprite* newSprite = CCSprite::create();
                if (currentSprite) {
                    currentSprite->addChild(newSprite);
                }
                spriteStack.push(newSprite);
                currentSprite = newSprite;
                if (!sprite) sprite = newSprite; 
            } else if (line == "SpriteEnd") {
                spriteStack.pop();
                currentSprite = spriteStack.empty() ? nullptr : spriteStack.top();
            } else if (line == "ChildrenStart" || line == "ChildrenEnd") {
                continue; 
            } else {
                size_t pos = line.find('=');
                if (pos == std::string::npos) continue;

                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                if (key == "Texture") {
                    currentSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(value.c_str(),false));
                } else if (key == "TextureRect") {
                    std::istringstream valStream(value);
                    float x, y, w, h;
                    char comma;
                    valStream >> x >> comma >> y >> comma >> w >> comma >> h;
                    currentSprite->setTextureRect(CCRect(x, y, w, h));
                } else if (key == "Position") {
                    std::istringstream valStream(value);
                    float x, y;
                    char comma;
                    valStream >> x >> comma >> y;
                    currentSprite->setPosition(ccp(x, y));
                } else if (key == "AnchorPoint") {
                    std::istringstream valStream(value);
                    float x, y;
                    char comma;
                    valStream >> x >> comma >> y;
                    currentSprite->setAnchorPoint(ccp(x, y));
                } else if (key == "RotationX") {
                    currentSprite->setRotationX(std::stof(value));
                } else if (key == "RotationY") {
                    currentSprite->setRotationY(std::stof(value));
                } else if (key == "ScaleX") {
                    currentSprite->setScaleX(std::stof(value));
                } else if (key == "ScaleY") {
                    currentSprite->setScaleY(std::stof(value));
                } else if (key == "Opacity") {
                    currentSprite->setOpacity(std::stoi(value));
                } else if (key == "Color") {
                    std::istringstream valStream(value);
                    int r, g, b;
                    char comma;
                    valStream >> r >> comma >> g >> comma >> b;
                    currentSprite->setColor(ccc3(r, g, b));
                }
            }
        }

        return sprite;
    }
}



class $modify(VP_DialogObject,DialogObject) {
    static DialogObject* create(
        gd::string character,
        gd::string text,
        CCSprite* characterFrame,
        float textScale,
        bool skippable,
        cocos2d::ccColor3B color
    ) {
        DialogObject* newInstance = DialogObject::create(character, text, 1, textScale, skippable, color);
        if (newInstance && characterFrame) {
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.emplace_back(newInstance, Viper_funnyutils::spriteToString(characterFrame) );
        }

        return newInstance;
    }
    void setSprite(CCSprite* x){
        auto it = std::find_if(
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [this](const std::pair<DialogObject*, std::string>& entry) {
                return entry.first == this;
            }
        );

        if (it !=  Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
             it->second = Viper_funnyutils::spriteToString(x);
        };
    }
    void byebye() {
        auto it = std::find_if(
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [this](const std::pair<DialogObject*, std::string>& entry) {
                return entry.first == this;
            }
        );

        if (it !=  Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.erase(it);
        }
    }
};

std::string getplrname() {
    // fix for coopeeo custom name
    if (Mod* coopeeo_customname = Loader::get()->getLoadedMod("coopeeo.customname")) {
        return coopeeo_customname->getSettingValue<std::string>("thename");
    }
    return GameManager::get()->m_playerName;
}
static std::pair<CCArray*, int> Viper_funnyutils::readjsonData(matjson::Value data) {
        std::pair<CCArray*, int> letsgo;
        letsgo.second = 2;
        letsgo.first = CCArray::create();
        if (!data.isArray() && !data.isObject()) {
            letsgo.first->addObject(DialogObject::create("uh oh","it seems like the json provided is invalid",2,1.0f,false,ccWHITE));
            return letsgo;
        }
        if (data.contains("color") && data["color"].isNumber()) {
            letsgo.second = data["color"].asInt().unwrapOr(2);
        }
         if (data.contains("lines") && data["lines"].isArray()) {
            for (const auto lols : data["lines"].asArray().unwrap()) {
                std::string title = "Unknown";
                if (lols.contains("title") && lols["title"].isString()) title = lols["title"].asString().unwrapOr("Unknown");
                gd::string message = "N/A";
                if (lols.contains("message") && lols["message"].isString()) message = lols["message"].asString().unwrapOr("N/A");

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

                if (lols.contains("portrait")) {
                    if (lols["portrait"].isNumber()) {
                        letsgo.first->addObject(DialogObject::create(title,message,lols["portrait"].asUInt().unwrapOr(1),1.0f,false,ccWHITE));
                    } else if (lols["portrait"].isString()) {
                        if (auto spr = Viper_funnyutils::IsSprite(lols["portrait"].asString().unwrapOr(""))) {
                            letsgo.first->addObject(VP_DialogObject::create(title,message,spr,1.0f,false,ccWHITE));
                        }
                    } else {
                         letsgo.first->addObject(DialogObject::create(title,message,1,1.0f,false,ccWHITE));
                    }
                } else {
                     letsgo.first->addObject(DialogObject::create(title,message,1,1.0f,false,ccWHITE));
                }
            }
        }
        log::debug("should obj");
        return letsgo;
};

class m_delegate_CallBackCustom : public DialogDelegate, public CCNode { // hacky fix to remove automaticly
    public:
        std::function<void()> m_callback;
        virtual void dialogClosed(DialogLayer* p0) {
            if (m_callback) {
                m_callback();
            }
        };
};

// main icon changer
class $modify(VP_DialogLayer, DialogLayer) {
    struct Fields {
        CCNode* m_cur_Customprofile;
    };
    void addCallbackCustom(std::function<void()> m_callback) {
        m_delegate_CallBackCustom* newdelgate = new m_delegate_CallBackCustom;
        this->addChild(newdelgate);
        newdelgate->autorelease();
        newdelgate->m_callback = m_callback;
        this->m_delegate = newdelgate;
    }
    void displayDialogObject(DialogObject* p0) {
        DialogLayer::displayDialogObject(p0);
        CCNode* parent = this;
        if (this->m_characterSprite) {
            parent = this->m_characterSprite->getParent();
            if (auto x = this->m_fields->m_cur_Customprofile) {
                x->removeFromParentAndCleanup(true);
            };
        }

         auto it = std::find_if(
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [p0](const std::pair<DialogObject*, std::string>& entry) {
                return entry.first == p0;
            }
        );

        if (it != Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
        if (CCSprite* sprite = Viper_funnyutils::stringToSprite(it->second)) {
                auto defaultPosition = this->m_characterSprite->getPosition();
                this->m_characterSprite->setVisible(false);
                this->m_characterSprite->setScale(0);
                sprite->setPosition(defaultPosition + sprite->getPosition());
                sprite->setID("customprofile"_spr);
                parent->addChild(sprite);
                this->m_fields->m_cur_Customprofile = sprite;
        }
    }
    };
};

