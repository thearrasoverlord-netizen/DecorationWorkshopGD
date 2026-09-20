#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

using namespace geode::prelude;

class WorkshopBlocker : public CCLayerColor {
public:
    static WorkshopBlocker* create(CCSize size) {
        auto ret = new WorkshopBlocker();
        if (ret && ret->initWithColor(ccc4(0, 0, 0, 150), size.width, size.height)) {
            ret->setTouchEnabled(true);
            ret->setTouchMode(kCCTouchesOneByOne);
            ret->setTouchPriority(-500);
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    bool ccTouchBegan(CCTouch*, CCEvent*) override {
        return true;
    }
};

class $modify(DecorationWorkshopEditorUI, EditorUI) {
    void onWorkshopClose(CCObject*) {
        if (auto panel = this->getChildByTag(98765))
            panel->removeFromParent();

        if (auto blocker = this->getChildByTag(98766))
            blocker->removeFromParent();
    }

    void keyDown(cocos2d::enumKeyCodes key, double timestamp) override {
        if (key == cocos2d::enumKeyCodes::KEY_Escape && this->getChildByTag(98765)) {
            this->onWorkshopClose(nullptr);
            return;
        }

        EditorUI::keyDown(key, timestamp);
    }

    void onWorkshopOpen(CCObject*) {
        if (this->getChildByTag(98765))
            return;

        auto winSize = this->getContentSize();

        auto blocker = WorkshopBlocker::create(winSize);
        if (!blocker)
            return;

        blocker->setTag(98766);
        this->addChild(blocker, 99);

        auto panel = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (!panel) {
            blocker->removeFromParent();
            return;
        }

        panel->setTag(98765);
        panel->setContentSize({
            winSize.width * 0.82f,
            winSize.height * 0.82f
        });
        panel->setAnchorPoint({0.5f, 0.5f});
        panel->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
        panel->setScale(0.0f);
        this->addChild(panel, 100);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        if (closeSprite) {
            closeSprite->setScale(0.65f);

            auto closeButton = CCMenuItemSpriteExtra::create(
                closeSprite,
                this,
                menu_selector(DecorationWorkshopEditorUI::onWorkshopClose)
            );

            auto closeMenu = CCMenu::create();
            closeMenu->setPosition(
                panel->getContentSize().width - 24.0f,
                panel->getContentSize().height - 24.0f
            );
            closeMenu->addChild(closeButton);
            panel->addChild(closeMenu, 1);
        }

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
