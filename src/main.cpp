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

    void onWorkshopBack(CCObject*) {
        this->showWorkshopHome();
    }

    void onWorkshopCard(CCObject*) {
        auto panel = this->getChildByTag(98765);
        if (!panel)
            return;

        this->showWorkshopDetails(panel);
    }

    void keyDown(cocos2d::enumKeyCodes key, double timestamp) override {
        if (key == cocos2d::enumKeyCodes::KEY_Escape && this->getChildByTag(98765)) {
            this->onWorkshopClose(nullptr);
            return;
        }

        EditorUI::keyDown(key, timestamp);
    }

    void addWorkshopTitle(CCNode* parent, const char* text, float y, float scale = 0.55f) {
        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(scale);
        label->setAnchorPoint({0.5f, 0.5f});
        label->setPosition(parent->getContentSize().width / 2.0f, y);
        parent->addChild(label, 5);
    }

    void addWorkshopLabel(CCNode* parent, const char* text, float x, float y, float scale = 0.42f) {
        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(scale);
        label->setAnchorPoint({0.0f, 0.5f});
        label->setPosition(x, y);
        parent->addChild(label, 5);
    }

    CCNode* createPreview(CCSize size) {
        auto preview = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (!preview)
            return nullptr;

        preview->setContentSize(size);
        preview->setColor(ccc3(145, 195, 225));
        preview->setOpacity(255);

        // Simple block decoration preview placeholder.
        for (int i = 0; i < 7; ++i) {
            auto block = CCLayerColor::create(ccc4(95, 95, 95, 255), 34.0f, 18.0f);
            block->setPosition(
                28.0f + i * 38.0f,
                22.0f + (i % 2) * 30.0f
            );
            preview->addChild(block, 2);
        }

        return preview;
    }

    void showWorkshopHome() {
        auto panel = this->getChildByTag(98765);
        if (!panel)
            return;

        panel->removeAllChildrenWithCleanup(true);

        auto contentSize = panel->getContentSize();

        auto title = CCLabelBMFont::create("Decoration Workshop", "bigFont.fnt");
        title->setScale(0.55f);
        title->setPosition(contentSize.width / 2.0f, contentSize.height - 32.0f);
        panel->addChild(title, 5);

        auto card = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (!card)
            return;

        card->setContentSize({
            contentSize.width * 0.72f,
            contentSize.height * 0.72f
        });
        card->setPosition(
            contentSize.width / 2.0f,
            contentSize.height / 2.0f - 5.0f
        );
        card->setColor(ccc3(65, 58, 55));

        auto cardButton = CCMenuItemSpriteExtra::create(
            card,
            this,
            menu_selector(DecorationWorkshopEditorUI::onWorkshopCard)
        );

        auto menu = CCMenu::create();
        menu->setPosition(0, 0);
        cardButton->setPosition(
            contentSize.width / 2.0f,
            contentSize.height / 2.0f - 5.0f
        );
        menu->addChild(cardButton);
        panel->addChild(menu, 2);

        auto preview = this->createPreview({
            card->getContentSize().width - 24.0f,
            card->getContentSize().height * 0.45f
        });
        if (preview) {
            preview->setPosition(
                cardButton->getPositionX(),
                cardButton->getPositionY() + 35.0f
            );
            panel->addChild(preview, 4);
        }

        auto cardTitle = CCLabelBMFont::create("MODERN DECORATION", "bigFont.fnt");
        cardTitle->setScale(0.38f);
        cardTitle->setPosition(
            cardButton->getPositionX(),
            cardButton->getPositionY() - 18.0f
        );
        panel->addChild(cardTitle, 5);

        auto author = CCLabelBMFont::create("BY: CREATORNAME", "bigFont.fnt");
        author->setScale(0.30f);
        author->setPosition(
            cardButton->getPositionX(),
            cardButton->getPositionY() - 38.0f
        );
        panel->addChild(author, 5);

        auto downloads = CCLabelBMFont::create("↓ 1,935", "bigFont.fnt");
        downloads->setScale(0.30f);
        downloads->setAnchorPoint({0.0f, 0.5f});
        downloads->setPosition(
            cardButton->getPositionX() - card->getContentSize().width / 2.0f + 18.0f,
            cardButton->getPositionY() - card->getContentSize().height / 2.0f + 32.0f
        );
        panel->addChild(downloads, 5);

        auto rating = CCLabelBMFont::create("★★★★★ 4.5 (276)", "bigFont.fnt");
        rating->setScale(0.27f);
        rating->setAnchorPoint({1.0f, 0.5f});
        rating->setPosition(
            cardButton->getPositionX() + card->getContentSize().width / 2.0f - 18.0f,
            cardButton->getPositionY() - card->getContentSize().height / 2.0f + 32.0f
        );
        panel->addChild(rating, 5);

        auto description = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (description) {
            description->setContentSize({
                card->getContentSize().width - 32.0f,
                42.0f
            });
            description->setColor(ccc3(55, 38, 30));
            description->setOpacity(210);
            description->setPosition(
                cardButton->getPositionX(),
                cardButton->getPositionY() - card->getContentSize().height / 2.0f + 75.0f
            );

            auto descText = CCLabelBMFont::create(
                "Awesome description of my decoration template!",
                "goldFont.fnt"
            );
            descText->setScale(0.25f);
            descText->setPosition(
                description->getContentSize().width / 2.0f,
                description->getContentSize().height / 2.0f
            );
            description->addChild(descText);
            panel->addChild(description, 5);
        }

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
                0.0f,
                panel->getContentSize().height
            );
            closeMenu->addChild(closeButton);
            panel->addChild(closeMenu, 10);
        }
    }

    void showWorkshopDetails(CCNode* panel) {
        panel->removeAllChildrenWithCleanup(true);

        auto contentSize = panel->getContentSize();

        auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_001.png");
        if (backSprite) {
            backSprite->setScale(0.65f);

            auto backButton = CCMenuItemSpriteExtra::create(
                backSprite,
                this,
                menu_selector(DecorationWorkshopEditorUI::onWorkshopBack)
            );

            auto backMenu = CCMenu::create();
            backMenu->setPosition(30.0f, contentSize.height - 30.0f);
            backMenu->addChild(backButton);
            panel->addChild(backMenu, 10);
        }

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        if (closeSprite) {
            closeSprite->setScale(0.65f);

            auto closeButton = CCMenuItemSpriteExtra::create(
                closeSprite,
                this,
                menu_selector(DecorationWorkshopEditorUI::onWorkshopClose)
            );

            auto closeMenu = CCMenu::create();
            closeMenu->setPosition(0.0f, contentSize.height);
            closeMenu->addChild(closeButton);
            panel->addChild(closeMenu, 10);
        }

        addWorkshopTitle(panel, "MODERN DECORATION", contentSize.height - 45.0f, 0.48f);
        addWorkshopLabel(panel, "BY: CREATORNAME", 42.0f, contentSize.height - 78.0f, 0.30f);

        auto preview = this->createPreview({
            contentSize.width * 0.68f,
            contentSize.height * 0.34f
        });
        if (preview) {
            preview->setPosition(
                contentSize.width / 2.0f,
                contentSize.height * 0.57f
            );
            panel->addChild(preview, 3);
        }

        addWorkshopLabel(panel, "Downloads: 1,935", 42.0f, contentSize.height * 0.34f, 0.32f);

        auto rating = CCLabelBMFont::create("★★★★★ 4.5 (276)", "bigFont.fnt");
        rating->setScale(0.30f);
        rating->setAnchorPoint({1.0f, 0.5f});
        rating->setPosition(
            contentSize.width - 42.0f,
            contentSize.height * 0.34f
        );
        panel->addChild(rating, 5);

        auto description = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (description) {
            description->setContentSize({
                contentSize.width * 0.76f,
                55.0f
            });
            description->setColor(ccc3(55, 38, 30));
            description->setOpacity(210);
            description->setPosition(
                contentSize.width / 2.0f,
                contentSize.height * 0.22f
            );

            auto descText = CCLabelBMFont::create(
                "Awesome description of my decoration template!",
                "goldFont.fnt"
            );
            descText->setScale(0.28f);
            descText->setPosition(
                description->getContentSize().width / 2.0f,
                description->getContentSize().height / 2.0f
            );
            description->addChild(descText);
            panel->addChild(description, 5);
        }

        auto useButtonSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn03_001.png");
        if (useButtonSprite) {
            auto label = CCLabelBMFont::create("USE TEMPLATE", "bigFont.fnt");
            label->setScale(0.42f);
            label->setPosition(
                useButtonSprite->getContentSize().width / 2.0f,
                useButtonSprite->getContentSize().height / 2.0f
            );
            useButtonSprite->addChild(label);

            auto useButton = CCMenuItemSpriteExtra::create(
                useButtonSprite,
                this,
                nullptr
            );

            auto useMenu = CCMenu::create();
            useMenu->setPosition(
                contentSize.width / 2.0f,
                55.0f
            );
            useMenu->addChild(useButton);
            panel->addChild(useMenu, 8);
        }
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

        this->showWorkshopHome();

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
