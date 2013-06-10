#include "TimeAttackMenuScene.h"

#include <algorithm>
#include <avalon/i18n/LanguageKey.h>
#include <avalon/i18n/Localization.h>
#include <avalon/ads/Manager.h>
#include "SceneManager.h"
#include "LevelMenuScene.h"
#include "LevelLoader.h"
#include "Globals.h"
#include "../buttons/ButtonFactory.h"
#include "userstate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using avalon::i18n::_;

#pragma mark Initialization

TimeAttackMenuScene::TimeAttackMenuScene()
: menu(NULL)
{
}

TimeAttackMenuScene::~TimeAttackMenuScene()
{
}

CCScene* TimeAttackMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(TimeAttackMenuScene::create());
    return scene;
}

void TimeAttackMenuScene::onEnter()
{
    CCLayer::onEnter();
    avalon::ads::Manager::showBannerIgnoreTime();
    createMenu();
}

bool TimeAttackMenuScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    menu = CCMenu::create();
    addChild(menu);

    addChild(ButtonFactory::createSceneBackButton());
    addChild(ButtonFactory::createStar());
    addChild(ButtonFactory::createHeadline(_("menu.timeattack", "headline").get().c_str()));

    setKeypadEnabled(true);
    return true;
}

void TimeAttackMenuScene::keyBackClicked()
{
    SceneManager::getInstance().popSceneWithSound();
}

void TimeAttackMenuScene::createMenu()
{
    menu->removeAllChildren();

    createButton(0);
    createButton(1);
    createButton(2);
    createButton(3);
    createButton(4);

    // comming soon button
    menu->addChild(ButtonFactory::createCategory(globalLevelLoader.getCategories().back(), NULL, NULL));

    menu->alignItemsVerticallyWithPadding(MENU_PADDING);
}

void TimeAttackMenuScene::createButton(const int id)
{
    char name[20] = {0};
    char desc[20] = {0};
    snprintf(name, sizeof(name), "level.%d.name", id);
    snprintf(desc, sizeof(desc), "level.%d.desc", id);

    int score = userstate::getScoreForTimeAttack(id);
    std::string scoreStr("-");
    if (score > 0) {
        scoreStr = score;
    }

    auto button = ButtonFactory::createPaymentButton(
        _("menu.timeattack", name).get().c_str(),
        _("menu.timeattack", desc)
        .assign("time.level", (int)BoardStats::getAttackLevelTime(id))
        .assign("time.puffer", (int)BoardStats::getAttackPufferTime(id))
        .get().c_str(),
        scoreStr.c_str(),
        this, menu_selector(TimeAttackMenuScene::btnGame)
    );
    button->setUserObject(CCInteger::create(id));
    menu->addChild(button);
}

void TimeAttackMenuScene::btnGame(void* sender)
{
    auto button = static_cast<CCMenuItemFont*>(sender);
    if (!button) {
        return;
    }

    auto index = static_cast<CCInteger*>(button->getUserObject());
    if (!index) {
        return;
    }

    GameScene::mode = GameScene::MODE_TIMEATTACK;
    GameScene::timeAttackId = index->getValue();

    setGlobalLevelAndAttackLevelQueue(index->getValue());

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    SceneManager::getInstance().gotoScene(GameScene::scene());
}

void TimeAttackMenuScene::setGlobalLevelAndAttackLevelQueue(const int id)
{
    globalAttackLevels.clear();

    std::vector<LoaderLevel*> levels;
    if (id == 0) {
        levels = getLevelsForCategory(4); // 4x4
    } else if (id == 1) {
        levels = getLevelsForCategory(5); // 5x5
    } else if (id == 2) {
        levels = getLevelsForCategory(6); // 6x6
    } else if (id == 3) {
        levels = getMergedLevels(4, 5); // 4x4 and 5x5
    } else if (id == 4) {
        levels = getMergedLevels(5, 6); // 5x5 and 6x6
    }

    globalAttackLevels = levels;
    globalAttackIterator = globalAttackLevels.begin();
    globalLevel = *globalAttackIterator;
}

std::vector<LoaderLevel*> TimeAttackMenuScene::getLevelsForCategory(const int id)
{
    std::vector<LoaderLevel*> levels;
    
    for (auto& category : globalLevelLoader.getCategories()) {
        if (category->localid == id) {
            levels.reserve(category->countLevels());
            for (auto& page : category->pages) {
                for (auto& level : page->levels) {
                    levels.push_back(level);
                }
            }

            std::srand(time(0));
            std::random_shuffle(levels.begin(), levels.end());

            return levels;
        }
    }

    return levels;
}

std::vector<LoaderLevel*> TimeAttackMenuScene::getMergedLevels(const int a, const int b)
{
    auto levelsA = getLevelsForCategory(a);
    auto levelsB = getLevelsForCategory(b);

    std::vector<LoaderLevel*> levels;
    levels.reserve(levelsA.size() + levelsB.size());

    auto iterA = levelsA.begin();
    auto iterB = levelsB.begin();

    while (iterA != levelsA.end() || iterB != levelsB.end()) {
        if (iterA != levelsA.end()) {
            levels.push_back(*iterA);
            ++iterA;
        }
        if (iterB != levelsB.end()) {
            levels.push_back(*iterB);
            ++iterB;
        }
    }

    return levels;
}