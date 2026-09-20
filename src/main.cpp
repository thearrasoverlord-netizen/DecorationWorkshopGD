#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(DecorationWorkshopMenuLayer, MenuLayer) {
    void onDecorationWorkshopPopup(float) {
        FLAlertLayer::create(
            "Decoration Workshop GD",
            "The mod is working!",
            "OK"
        )->show();
    }

    bool init() {
        if (!MenuLayer::init())
            return false;

        this->scheduleOnce(
            schedule_selector(DecorationWorkshopMenuLayer::onDecorationWorkshopPopup),
            0.5f
        );

        return true;
    }
};
