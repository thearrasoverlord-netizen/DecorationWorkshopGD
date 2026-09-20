#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

using namespace geode::prelude;

class $modify(DecorationWorkshopEditorUI, EditorUI) {
    void onWorkshopClose(CCObject*) {
        if (auto panel = this->getChildByTag(98765)) {
            panel->removeFromParent();
        }
    }

    void onWorkshopOpen(CCObject*) {
        if (this->getChildByTag(98765))
            return;

        auto panel = CCLayerColor::create(ccc4(125, 82, 45, 255), 520.0f, 320.0f);
        panel->setTag(98765);
        panel->setPosition(
            (this->getContentSize().width - panel->getContentSize().width) / 2.0f,
            (this->getContentSize().height - panel->getContentSize().height) / 2.0f
        );
        this->addChild(panel, 100);

        auto closeSprite = CCLabelBMFont::create("X", "bigFont.fnt");
        closeSprite->setScale(0.65f);

        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(DecorationWorkshopEditorUI::onWorkshopClose)
        );

        auto closeMenu = CCMenu::create();
        closeMenu->setPosition(24.0f, panel->getContentSize().height - 24.0f);
        closeMenu->addChild(closeButton);
        panel->addChild(closeMenu);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        alpha::editor_tabs::addTab(
            "decoration-workshop"_spr,
            alpha::editor_tabs::BUILD,
            [this] {
                std::vector<Ref<CCNode>> nodes;

                auto openSprite = CCLabelBMFont::create("[Open Workshop]", "bigFont.fnt");
                openSprite->setScale(0.45f);

                auto openButton = CCMenuItemSpriteExtra::create(
                    openSprite,
                    this,
                    menu_selector(DecorationWorkshopEditorUI::onWorkshopOpen)
                );

                nodes.push_back(openButton);

                return alpha::editor_tabs::createEditButtonBar(nodes);
            },
            [] {
                auto icon = CCLabelBMFont::create("DW", "bigFont.fnt");
                icon->setScale(0.6f);
                return icon;
            }
        );

        return true;
    }
};
