#pragma once
#include <Geode/modify/DialogObject.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;
// m_fields but for ccobject scuffed method
// if someone wants to see this class then sure ig? i don't mind
namespace Viper_funnyutils {
 static std::vector<std::pair<DialogObject*, CCSprite*>> FucktheM_fieldsInCCObjects_DialogObject;
 static CCSprite* deepCopySprite(CCSprite* originalSprite) {
        auto newSprite = CCSprite::createWithTexture(originalSprite->getTexture());
        newSprite->setTextureRect(originalSprite->getTextureRect());
        newSprite->setPosition(originalSprite->getPosition());
        newSprite->setAnchorPoint(originalSprite->getAnchorPoint());

        newSprite->setRotationX(originalSprite->getRotationX());
        newSprite->setRotationY(originalSprite->getRotationY());

        newSprite->setScaleX(originalSprite->getScaleX());
        newSprite->setScaleY(originalSprite->getScaleY());

        newSprite->setOpacity(originalSprite->getOpacity());
        newSprite->setColor(originalSprite->getColor());
        if (originalSprite->getChildrenCount() > 0) {
            CCArray* children = originalSprite->getChildren();
            for (unsigned int i = 0; i < children->count(); ++i) {
                CCNode* child = dynamic_cast<CCNode*>(children->objectAtIndex(i)); 
                CCSprite* childSprite = dynamic_cast<CCSprite*>(child);
                if (childSprite) {
                    CCSprite* childCopy = deepCopySprite(childSprite);
                    newSprite->addChild(childCopy);
                }
            }
        }
        newSprite->retain();
        return newSprite;
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
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.emplace_back(newInstance, Viper_funnyutils::deepCopySprite(characterFrame) );
        }

        return newInstance;
    }

    CCSprite* getcurrenspr() {
         auto it = std::find_if(
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [this](const std::pair<DialogObject*, CCSprite*>& entry) {
                return entry.first == this;
            }
        );

        if (it !=  Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
            return it->second;
        }
        return nullptr;
    }

    void byebye() {
        auto it = std::find_if(
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [this](const std::pair<DialogObject*, CCSprite*>& entry) {
                return entry.first == this;
            }
        );

        if (it !=  Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
             Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.erase(it);
        }
    }
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
        CCNode* m_InvisibleSprites;
    };
    void addCallbackCustom(std::function<void()> m_callback) {
        m_delegate_CallBackCustom* newdelgate = new m_delegate_CallBackCustom;
        this->addChild(newdelgate);
        newdelgate->autorelease();
        newdelgate->m_callback = m_callback;
        this->m_delegate = newdelgate;
    }
    void displayDialogObject(DialogObject* p0) {
        if (!this->m_fields->m_InvisibleSprites) {
            this->m_fields->m_InvisibleSprites = CCMenu::create();
            this->m_fields->m_InvisibleSprites->setVisible(false);
            this->m_fields->m_InvisibleSprites->setID("The-Doom-And-Gloom"_spr);
            this->addChild( this->m_fields->m_InvisibleSprites);
        }
        DialogLayer::displayDialogObject(p0);
        auto parent = this->m_characterSprite->getParent();
        if (auto x = parent->getChildByIDRecursive("customprofile"_spr)) {
            x->removeFromParentAndCleanup(false);
            this->m_fields->m_InvisibleSprites->addChild(x);
        };

         auto it = std::find_if(
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.begin(),
            Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end(),
            [p0](const std::pair<DialogObject*, CCSprite*>& entry) {
                return entry.first == p0;
            }
        );

        if (it != Viper_funnyutils::FucktheM_fieldsInCCObjects_DialogObject.end()) {
        if (CCSprite* sprite = it->second) {
                auto defaultPosition = this->m_characterSprite->getPosition();
                this->m_characterSprite->setVisible(false);
                this->m_characterSprite->setScale(0);
                auto sprParent = sprite->getParent();
                if (sprParent == this->m_fields->m_InvisibleSprites) {
                    sprite->removeFromParentAndCleanup(false);
                    return parent->addChild(sprite);
                }
                if (sprParent != parent) {
                    sprite->setPosition(defaultPosition + sprite->getPosition());
                    sprite->setID("customprofile"_spr);
                    parent->addChild(sprite);
                } 
                
        }
    }
    };
};

