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

        // Placeholder structure made from textured GD panels instead of flat gray rectangles.
        for (int i = 0; i < 5; ++i) {
            auto block = CCSprite::createWithSpriteFrameName("GJ_square01.png");
            if (!block)
                continue;

            block->setScale(0.24f);
            block->setColor(ccc3(95, 95, 95));
            block->setPosition(ccp(
                32.0f + i * (size.width - 64.0f) / 4.0f,
                size.height * (i % 2 == 0 ? 0.34f : 0.58f)
            ));
            preview->addChild(block, 2);
        }

        return preview;
    }

    void addDownloadInfo(CCNode* parent, float x, float y) {
        auto icon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
        if (icon) {
            icon->setScale(0.38f);
            icon->setPosition(ccp(x, y));
            parent->addChild(icon, 6);
        }

        auto downloads = CCLabelBMFont::create("1,935", "bigFont.fnt");
        downloads->setScale(0.30f);
        downloads->setAnchorPoint({0.0f, 0.5f});
        downloads->setPosition(x + 12.0f, y);
        parent->addChild(downloads, 6);
    }

    void addRating(CCNode* parent, float x, float y) {
        float starSpacing = 13.0f;
        float startX = x - starSpacing * 2.0f;

        for (int i = 0; i < 5; ++i) {
            auto star = CCSprite::createWithSpriteFrameName("GJ_bigStar_noShadow_001.png");
            if (!star)
                continue;

            star->setScale(0.12f);
            star->setPosition(ccp(startX + i * starSpacing, y));
            parent->addChild(star, 6);
        }

        auto rating = CCLabelBMFont::create("4.5 (276)", "bigFont.fnt");
        rating->setScale(0.27f);
        rating->setAnchorPoint({0.0f, 0.5f});
        rating->setPosition(startX + starSpacing * 2.7f, y);
        parent->addChild(rating, 6);
    }

    void showWorkshopHome() {
        auto panel = this->getChildByTag(98765);
        if (!panel)
            return;

        panel->removeAllChildrenWithCleanup(true);

        auto contentSize = panel->getContentSize();

        // Infinite loading screen while the workshop is receiving its pizza.
        auto loadingCircle = CCSprite::create("loadingCircle-uhd.png");
        if (loadingCircle) {
            loadingCircle->setPosition(ccp(
                contentSize.width / 2.0f,
                contentSize.height / 2.0f + 22.0f
            ));
            loadingCircle->setScale(0.5f);
            loadingCircle->runAction(
                CCRepeatForever::create(
                    CCRotateBy::create(1.0f, 360.0f)
                )
            );
            panel->addChild(loadingCircle, 5);
        }

        const char* chatFont = "chatFont.fnt";
        if (CCDirector::sharedDirector()->getContentScaleFactor() > 1.0f &&
            CCFileUtils::sharedFileUtils()->isFileExist("chatFont-hd.fnt")) {
            chatFont = "chatFont-hd.fnt";
        }

        auto loadingText = CCLabelBMFont::create(
            "Loading... We're trying to receive the pizza",
            chatFont
        );
        if (loadingText) {
            loadingText->setScale(0.55f);
            loadingText->setPosition(
                contentSize.width / 2.0f,
                contentSize.height / 2.0f - 28.0f
            );
            loadingText->limitLabelWidth(contentSize.width * 0.72f, 0.55f, 0.0f);
            panel->addChild(loadingText, 5);
        }

        return;

        auto title = CCLabelBMFont::create("Decoration Workshop", "bigFont.fnt");
        title->setScale(0.55f);
        title->setPosition(contentSize.width / 2.0f, contentSize.height - 32.0f);
        panel->addChild(title, 5);

        // Compact template card, matching the intended Smart Templates-style layout.
        const float cardWidth = contentSize.width * 0.30f;
        const float cardHeight = contentSize.height * 0.72f;
        const float cardX = 24.0f + cardWidth / 2.0f;
        const float cardY = contentSize.height / 2.0f - 5.0f;

        auto card = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (!card)
            return;

        card->setContentSize({cardWidth, cardHeight});
        card->setPosition(cardX, cardY);
        card->setColor(ccc3(82, 63, 54));

        auto cardButton = CCMenuItemSpriteExtra::create(
            card,
            this,
            menu_selector(DecorationWorkshopEditorUI::onWorkshopCard)
        );

        auto menu = CCMenu::create();
        menu->setPosition(0, 0);
        cardButton->setPosition(cardX, cardY);
        menu->addChild(cardButton);
        panel->addChild(menu, 2);

        auto preview = this->createPreview({
            cardWidth - 24.0f,
            cardHeight * 0.42f
        });
        if (preview) {
            preview->setPosition(cardX, cardY + cardHeight * 0.25f);
            panel->addChild(preview, 4);
        }

        auto cardTitle = CCLabelBMFont::create("MODERN DECORATION", "bigFont.fnt");
        cardTitle->setScale(0.34f);
        cardTitle->setPosition(cardX, cardY + 7.0f);
        panel->addChild(cardTitle, 5);

        auto author = CCLabelBMFont::create("BY: CREATORNAME", "bigFont.fnt");
        author->setScale(0.28f);
        author->setPosition(cardX, cardY - 15.0f);
        panel->addChild(author, 5);

        addDownloadInfo(
            panel,
            cardX - cardWidth / 2.0f + 30.0f,
            cardY - cardHeight / 2.0f + 56.0f
        );

        addRating(
            panel,
            cardX + cardWidth / 2.0f - 76.0f,
            cardY - cardHeight / 2.0f + 56.0f
        );

        auto description = CCScale9Sprite::create("GJ_square01.png", CCRect(0, 0, 80, 80));
        if (description) {
            description->setContentSize({
                cardWidth - 24.0f,
                38.0f
            });
            description->setColor(ccc3(55, 38, 30));
            description->setOpacity(210);
            description->setPosition(
                cardX,
                cardY - cardHeight / 2.0f + 38.0f
            );

            auto descText = CCLabelBMFont::create(
                "Awesome description of my decoration template!",
                "goldFont.fnt"
            );
            descText->setScale(0.22f);
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

        addDownloadInfo(panel, 58.0f, contentSize.height * 0.34f);
        addRating(panel, contentSize.width - 86.0f, contentSize.height * 0.34f);

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