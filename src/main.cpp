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

        auto winSize = this->getContentSize();

        auto texture = CCTextureCache::sharedTextureCache()->addImage("GJ_square01-uhd.png");
        if (!texture)
            return;

        auto panel = CCSprite::createWithTexture(texture);
        if (!panel)
            return;

        panel->setTag(98765);

        auto panelWidth = winSize.width * 0.82f;
        auto panelHeight = winSize.height * 0.82f;

        auto textureSize = panel->getContentSize();
        panel->setScaleX(panelWidth / textureSize.width);
        panel->setScaleY(panelHeight / textureSize.height);
        panel->setPosition(winSize.width / 2.0f, winSize.height / 2.0f);
        panel->setAnchorPoint({0.5f, 0.5f});
        panel->setScale(0.0f);
        this->addChild(panel, 100);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite,
            this,
            menu_selector(DecorationWorkshopEditorUI::onWorkshopClose)
        );

        auto closeMenu = CCMenu::create();
        closeMenu->setPosition(
            24.0f / panel->getScaleX(),
            panel->getContentSize().height - 24.0f / panel->getScaleY()
        );
        closeMenu->addChild(closeButton);
        panel->addChild(closeMenu, 1);

        panel->runAction(
            CCEaseElasticOut::create(
                CCScaleTo::create(0.55f, 1.0f),
                0.8f
            )
        );
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        alpha::editor_tabs::addTab(
            "decoration-workshop"_spr,
            alpha::editor_tabs::BUILD,
            [this] {
                std::vector<Ref<CCNode>> nodes;

                auto openSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn03_001.png");
                if (!openSprite)
                    return alpha::editor_tabs::createEditButtonBar(nodes);

                auto label = CCLabelBMFont::create("Open Workshop", "bigFont.fnt");
                label->setScale(0.55f);
                label->setPosition(
                    openSprite->getContentSize().width / 2.0f,
                    openSprite->getContentSize().height / 2.0f
                );
                openSprite->addChild(label);

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
