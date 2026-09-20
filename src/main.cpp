#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(DecorationWorkshopMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        this->scheduleOnce(
            [](float) {
                FLAlertLayer::create(
                    "Decoration Workshop GD",
                    "The mod is working!",
                    "OK"
                )->show();
            },
            0.5f,
            "decoration-workshop-popup"
        );

        return true;
    }
};
