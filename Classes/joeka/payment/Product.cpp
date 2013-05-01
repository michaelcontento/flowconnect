//
//  Product.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "Product.h"
#include <cassert>
#include "cocos2d.h"

using namespace cocos2d;

void Product::setId(const char *id)
{
    internalId = id;
}

void Product::setNativeId(const char *id)
{
    nativeId = id;
}

bool Product::isPurchased()
{
    return paymentProvider->isPurchased(nativeId.c_str());
}

void Product::purchase()
{
    assert(paymentProvider && "paymentProvider not set");
    assert(!nativeId.empty() && "nativeId of Product not set!");
    paymentProvider->purchase(internalId.c_str());
}

void Product::setPrice(const float newPrice)
{
    price = newPrice;
    cacheSet("price", newPrice);
}

float Product::getPrice()
{
    if (price == 0) {
        price = cacheGetFloat("price");
    }
    
    return price;
}

void Product::setLocalizedName(const std::string newName)
{
    localizedName = newName;
    cacheSet("name", newName);

}

std::string Product::getLocalizedName()
{
    if (localizedName.empty()) {
        localizedName = cacheGet("name");
    }
    
    return localizedName;
}

void Product::setLocalizedDescription(const std::string newDescription)
{
    localizedDescription = newDescription;
    cacheSet("desc", newDescription);
}

std::string Product::getLocalizedDescription()
{
    if (localizedDescription.empty()) {
        localizedDescription = cacheGet("desc");
    }
    
    return localizedDescription;
}

void Product::setLocalizedPrice(const std::string newPrice)
{
    localizedPrice = newPrice;
    cacheSet("priceStr", newPrice);
}

std::string Product::getLocalizedPrice()
{
    if (localizedPrice.empty()) {
        localizedPrice = cacheGet("priceStr");
    }
    
    return localizedPrice;
}

void Product::cacheSet(const char* key, const std::string& value)
{
    auto cache = CCUserDefault::sharedUserDefault();
    cache->setStringForKey(getCacheKey(key), value);
    cache->flush();
}

void Product::cacheSet(const char* key, const float value)
{
    auto cache = CCUserDefault::sharedUserDefault();
    cache->setFloatForKey(getCacheKey(key), value);
    cache->flush();
}

std::string Product::cacheGet(const char* key)
{
    auto cache = CCUserDefault::sharedUserDefault();
    return cache->getStringForKey(getCacheKey(key));
}

float Product::cacheGetFloat(const char* key)
{
    auto cache = CCUserDefault::sharedUserDefault();
    return cache->getFloatForKey(getCacheKey(key));
}

const char* Product::getCacheKey(const char* prefix)
{
    assert(!nativeId.empty() && "nativid should be set");
    
    static char buffer[250] = {0};
    snprintf(
        buffer, sizeof(buffer),
        "_PAYMENT_PRODUCT_CACHE__%s_%s",
        nativeId.c_str(), prefix
    );
    return buffer;
}