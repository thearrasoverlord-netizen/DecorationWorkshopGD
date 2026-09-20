#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(DecorationWorkshopMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        FLAlertLayer::create(
            "Decoration Workshop GD",
            "The mod is working!",
            "OK"
        )->show();

        return true;
    }
};
