/****************************************************************************
Copyright (c) 2014-2019 Gabriel Heilig
fresh-engine
funkyzooink@gmail.com
****************************************************************************/
// TODO this is old legacy code and needs a cleanup

#include "ColorPlugin.h"
#include "../GameData/Constants.h"
#include "../GameData/GameConfig.h"
#include "Utility.h"

#include "cocos2d.h"

cocos2d::Color4F ColorPlugin::colorHelper(cocos2d::Color4F start, cocos2d::Color4F end, float div)
{
  cocos2d::Color4F mixedColor;
  mixedColor.r = start.r + (end.r - start.r) * div;
  mixedColor.g = start.g + (end.g - start.g) * div;
  mixedColor.b = start.b + (end.b - start.b) * div;
  mixedColor.a = start.a + (end.a - start.a) * div;
  return mixedColor;
}

void ColorPlugin::initLayerBackground(cocos2d::TMXTiledMap* tiledMap, int worldID, cocos2d::Node* cameraFollowNode)
{
  // read objects
  cocos2d::TMXObjectGroup* grp = tiledMap->getObjectGroup("background");
  if (grp)
  {
    auto objects = grp->getObjects();
    for (auto& object : objects)
    {
      auto properties = object.asValueMap();
      cocos2d::Point initialPosition = cocos2d::Point(0, 0);
      initialPosition.x = properties.at("x").asInt();
      initialPosition.y = properties.at("y").asInt();

      cocos2d::Size initialSize = cocos2d::Size(0, 0);
      initialSize.width = properties.at("width").asInt();  
      initialSize.height = properties.at("height").asInt();
      std::string name = properties.at("name").asString();

      float div = Utility::randomValueBetween(0, 3) * 0.1f;
      cocos2d::Color4F bgColor = colorHelper(Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 0)),
                                             Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 1)),
                                             div);

      bgColor.a = 0.3;

      auto backObj = spriteWithColor(bgColor, bgColor, initialSize.width, initialSize.height, false);
      backObj->setLocalZOrder(Constants::LocalZOrderEnum::BACKGROUND_Z_ORDER);
      backObj->setPosition(initialPosition + backObj->getContentSize() / 2);
      cameraFollowNode->addChild(backObj);
    }
  }
}
void ColorPlugin::initLayerColors(cocos2d::TMXTiledMap* tiledMap, int worldID, cocos2d::Node* cameraFollowNode,
                                  bool showItems)
{
  // read objects
  cocos2d::TMXObjectGroup* grp = tiledMap->getObjectGroup("colors");

  if (grp)
  {
    auto objects = grp->getObjects();
    for (auto& object : objects)
    {
      auto properties = object.asValueMap();
      cocos2d::Point initialPosition = cocos2d::Point(0, 0);
      initialPosition.x = (int)(properties.at("x").asInt());
      initialPosition.y = (int)(properties.at("y").asInt());

      cocos2d::Size initialSize = cocos2d::Size(0, 0);
      initialSize.width = (int)(properties.at("width").asInt());  
      initialSize.height = (int)(properties.at("height").asInt());
      std::string name = properties.at("name").asString();

      if (name == "color")
      {
        float div = Utility::randomValueBetween(0, 10) * 0.1f;
        cocos2d::Color4F bgColor = colorHelper(Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 0)),
                                               Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 1)),
                                               div);  // TODO error handling
        div = Utility::randomValueBetween(0, 10) * 0.1f;
        cocos2d::Color4F fgColor = colorHelper(Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 1)),
                                               Utility::colorConverterFloat(GAMECONFIG.getWorldColors(worldID, 0)),
                                               div);  // TODO error handling

        auto backObj = spriteWithColor(bgColor, fgColor, initialSize.width, initialSize.height, true);
        backObj->setLocalZOrder(Constants::LocalZOrderEnum::NO_Z_ORDER);
        backObj->setPosition(initialPosition + backObj->getContentSize() / 2);
        cameraFollowNode->addChild(backObj);
      }
      else if (showItems)  // TODO else if due to mainscene
      {
        switch (worldID)
        {
          case 0:
          {
            if (name == "diamond" || name == "treeshape5")  // mNWorldNr<4)
            {
              auto file = name + ".png";
              // load object
              cocos2d::Sprite* background1 = cocos2d::Sprite::createWithSpriteFrameName(file);
              background1->setPosition(initialPosition);
              background1->setLocalZOrder(Constants::LocalZOrderEnum::BACKGROUND_Z_ORDER);  // TODO was - 4
              background1->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
              //                    if(name=="tree" || name=="tree2")
              background1->setColor(cocos2d::Color3B::BLACK);
              float scalex = initialSize.width / background1->getContentSize().width;
              float scaley = initialSize.height / background1->getContentSize().height;
              if (scalex > scaley)
              {
                background1->setScale(scaley);
              }
              else
              {
                background1->setScale(scalex);
              }
              cameraFollowNode->addChild(background1);
            }
            else
            {
              // replace all bg img with pixel tile
              createPixelWall(initialPosition, initialSize, cameraFollowNode);
            }
            break;
          }
          case 1:
            if (name != "rock" && name != "rockred" && name != "treeshape" && name != "treeshape2" &&
                name != "diamond" && name != "flower1" && name != "flower2")
            {
              // replace all bg img with pixel tile
              createPixelWall(initialPosition, initialSize, cameraFollowNode);
            }
            else
            {
              if (name == "rock" || name == "rockred")
              {
                name = name + Utility::numberToString(Utility::randomValueBetween(0, 3));
              }
              auto file = name + ".png";
              // load object
              cocos2d::Sprite* background1 = cocos2d::Sprite::createWithSpriteFrameName(file);
              background1->setPosition(initialPosition);
              background1->setLocalZOrder(Constants::LocalZOrderEnum::BACKGROUND_Z_ORDER);  // TODO was - 4
              background1->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
              if (name == "tree0" || name == "tree1" || name == "tree2" || name == "tree3" || name == "tree4" ||
                  name == "rock0" || name == "rock1" || name == "rock2" || name == "rock3" || name == "rockred0" ||
                  name == "rockred1" || name == "rockred2" || name == "rockred3")
              {
                background1->setFlippedX(Utility::randomValueBetween(0, 1));
                background1->setPosition(initialPosition.x, initialPosition.y - CONSTANTS.getOffset() * 0.1);
              }
              else
              {
                //                    if(name=="tree" || name=="tree2")
                //                        background1->setColor(cocos2d::Color3B(192,104,31));
                float scalex = initialSize.width / background1->getContentSize().width;
                float scaley = initialSize.height / background1->getContentSize().height;
                if (scalex > scaley)
                {
                  background1->setScale(scaley);
                }
                else
                {
                  background1->setScale(scalex);
                }
              }
              cameraFollowNode->addChild(background1);
            }
            break;
          default:
            if (name == "rock")
            {
              name = name + Utility::numberToString(Utility::randomValueBetween(0, 3));
            }
            // leave all as it is
            auto file = name + ".png";
            // load object
            cocos2d::Sprite* background1 = cocos2d::Sprite::createWithSpriteFrameName(file);
            background1->setPosition(initialPosition);
            background1->setLocalZOrder(Constants::LocalZOrderEnum::BACKGROUND_Z_ORDER);  // TODO was - 4
            background1->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
            if (name == "treeshape" || name == "treeshape2")
            {
              background1->setColor(cocos2d::Color3B(192, 104, 31));
              float scalex = initialSize.width / background1->getContentSize().width;
              float scaley = initialSize.height / background1->getContentSize().height;
              if (scalex > scaley)
              {
                background1->setScale(scaley);
              }
              else
              {
                background1->setScale(scalex);
              }
            }
            else if (name == "flower1" || name == "flower2")
            {
              background1->setColor(cocos2d::Color3B(192, 104, 31));
              background1->setFlippedX(Utility::randomValueBetween(0, 1));
              float scalex = initialSize.width / background1->getContentSize().width;
              float scaley = initialSize.height / background1->getContentSize().height;
              if (scalex > scaley)
              {
                background1->setScale(scaley);
              }
              else
              {
                background1->setScale(scalex);
              }
            }
            else if (name == "plant0" || name == "plant1" || name == "plant2" || name == "plant3" || name == "plant4" ||
                     name == "plant5" || name == "plant6" || name == "mushroom0" || name == "mushroom1" ||
                     name == "mushroom2" || name == "mushroom3")
            {
              //                    background1->setColor(cocos2d::Color3B(192,
              //                    104, 31));
              background1->setFlippedX(Utility::randomValueBetween(0, 1));
            }
            else if (name == "tree0" || name == "tree1" || name == "tree2" || name == "tree3" || name == "tree4" ||
                     name == "rock0" || name == "rock1" || name == "rock2" || name == "rock3" || name == "rockred0" ||
                     name == "rockred1" || name == "rockred2" || name == "rockred3")
            {
              background1->setFlippedX(Utility::randomValueBetween(0, 1));
              background1->setPosition(initialPosition.x, initialPosition.y - CONSTANTS.getOffset() * 0.1);
            }

            cameraFollowNode->addChild(background1);
            break;
        }
      }
    }
  }
}
void ColorPlugin::createPixelWall(cocos2d::Point initialPosition, cocos2d::Size initialSize,
                                  cocos2d::Node* cameraFollowNode)
{
  int iMax = initialSize.width / (CONSTANTS.getOffset() * 2);
  int jMax = initialSize.height / (CONSTANTS.getOffset() * 2);
  for (int i = 0; i < iMax; ++i)
  {
    for (int j = 0; j < jMax; ++j)
    {
      auto name = "pixelwall" + Utility::numberToString(Utility::randomValueBetween(1, 4)) + ".png";
      cocos2d::Sprite* background1 = cocos2d::Sprite::createWithSpriteFrameName(name);
      background1->setPosition(initialPosition.x + i * background1->getContentSize().width,
                               initialPosition.y + j * background1->getContentSize().height);
      background1->setLocalZOrder(Constants::LocalZOrderEnum::BACKGROUND_Z_ORDER);  // TODO was - 5
      cameraFollowNode->addChild(background1);
    }
  }
}

cocos2d::Sprite* ColorPlugin::spriteWithColor(cocos2d::Color4F bgColor, cocos2d::Color4F fgColor, float tWidth,
                                              float tHeight, bool gradient)
{
  cocos2d::RenderTexture* rt = cocos2d::RenderTexture::create(tWidth, tHeight);
  rt->beginWithClear(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

  if (gradient)
  {
    cocos2d::Sprite* noise2 = cocos2d::Sprite::createWithSpriteFrameName(CONSTANTS.iconNoise);
    cocos2d::BlendFunc blend2 = { GL_DST_COLOR, GL_ZERO };
    noise2->setBlendFunc(blend2);
    noise2->setPosition(tWidth / 2, tHeight / 2);
    float scaleY2 = tHeight / noise2->getContentSize().height;
    float scaleX2 = tWidth / noise2->getContentSize().width;
    noise2->setScaleX(scaleX2);
    noise2->setScaleY(scaleY2);
    noise2->visit();
  }
  rt->end();
  return cocos2d::Sprite::createWithTexture(rt->getSprite()->getTexture());
}
