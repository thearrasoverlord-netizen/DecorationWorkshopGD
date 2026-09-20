#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

using namespace geode::prelude;

class $modify(DecorationWorkshopEditorUI, EditorUI) {
    void onDecorationWorkshopInfo(CCObject*) {
        FLAlertLayer::create(
            "Decoration Workshop GD",
            "Decoration Workshop GD is an editor tool focused on making level decoration faster and easier.",
            "OK"
        )->show();
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        alpha::editor_tabs::addTab(
            "decoration-workshop"_spr,
            alpha::editor_tabs::BUILD,
            [this] {
                auto infoSprite = CCLabelBMFont::create("i", "bigFont.fnt");
                infoSprite->setScale(0.8f);

                auto infoButton = CCMenuItemSpriteExtra::create(
                    infoSprite,
                    this,
                    menu_selector(DecorationWorkshopEditorUI::onDecorationWorkshopInfo)
                );

                return alpha::editor_tabs::createEditButtonBar({infoButton});
            },
            [] {
                return CCLabelBMFont::create("DW", "bigFont.fnt");
            }
        );

        return true;
    }
};
