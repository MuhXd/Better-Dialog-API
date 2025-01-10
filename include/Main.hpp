#pragma once
#include "ccDialogHooks.hpp" 
#include <Geode/Geode.hpp>

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
    // shortcuts
   static VP_DialogLayer* createWithObjects(CCArray* objects, int background) {
        return reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createWithObjects(objects,background));
   };
   static VP_DialogLayer* create(DialogObject* object, int background) {
        return reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(object,background));
   };

  static VP_DialogLayer* create(DialogObject* object, int background, std::function<void()> m_callback) {
         VP_DialogLayer* mass = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(object,background));
         mass->addCallbackCustom(m_callback);
         return mass;
   };

   static VP_DialogLayer* create(DialogObject* object,CCArray* objects, int background, std::function<void()> m_callback) {
         VP_DialogLayer* mass = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createWithObjects(objects,background));
         mass->addCallbackCustom(m_callback);
         return mass;
   };

   static VP_DialogLayer* createWithCallback(DialogObject* object, int background, std::function<void()> m_callback) {
         VP_DialogLayer* mass = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(object,background));
         mass->addCallbackCustom(m_callback);
         return mass;
   };

   static VP_DialogLayer* createDialogLayer(DialogObject* object,CCArray* objects, int background) {
        return reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createDialogLayer(object,objects,background));
   };

   static VP_DialogLayer* createDialogLayer(DialogObject* object, int background, std::function<void()> m_callback) {
         VP_DialogLayer* mass = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(object,background));
         mass->addCallbackCustom(m_callback);
         return mass;
   };

    static VP_DialogLayer* createDialogLayer(CCArray* objects, int background, std::function<void()> m_callback) {
         VP_DialogLayer* mass = reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createWithObjects(objects,background));
         mass->addCallbackCustom(m_callback);
         return mass;
   };

    static VP_DialogLayer* createDialogLayer(DialogObject* object,CCArray* objects, int background, std::function<void()> m_callback) {
        VP_DialogLayer* mass =  reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createDialogLayer(object,objects,background));
        mass->addCallbackCustom(m_callback);
        return mass;
   };

   static VP_DialogLayer* createDialogLayer(DialogObject* object, int background) {
        return reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::create(object,background));
   };
   
    static VP_DialogLayer* createDialogLayer(CCArray* objects, int background) {
        return reinterpret_cast<VP_DialogLayer*>(VP_DialogLayer::createWithObjects(objects,background));
   };
    // som
   static VP_DialogObject* create(gd::string character,
            gd::string text,
            geode::prelude::CCSprite* characterFrame,
            float textScale,
            bool skippable,
            cocos2d::ccColor3B color
    );
    static VP_DialogObject* create(gd::string character,
            gd::string text,
            int characterFrame,
            float textScale,
            bool skippable,
            cocos2d::ccColor3B color
    );
};
