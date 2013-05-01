//
//  Product.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__Product__
#define __Adventures_on_the_farm__Product__

#include <string.h>
#include "PaymentProvider.h"
#include "cocos2d.h"

class Product
{
public:
    PaymentProvider *paymentProvider;

    std::string internalId;
    std::string nativeId;

    Product()
    : price(0)
    {}
    virtual ~Product() {}

    virtual void setId(const char *id);
    virtual void setNativeId(const char *id);

    virtual void purchase();

    virtual bool isPurchased();

    virtual void setPrice(const float newPrice);
    virtual float getPrice();
    
    virtual void setLocalizedName(const std::string newName);
    virtual std::string getLocalizedName();

    virtual void setLocalizedDescription(const std::string newDescription);
    virtual std::string getLocalizedDescription();

    virtual void setLocalizedPrice(const std::string newPrice);
    virtual std::string getLocalizedPrice();

private:
    float price;
    std::string localizedName;
    std::string localizedDescription;
    std::string localizedPrice;

    void cacheSet(const char* key, const std::string& value);
    void cacheSet(const char* key, const float value);
    std::string cacheGet(const char* key);
    float cacheGetFloat(const char* key);
    const char* getCacheKey(const char* prefix);
};

#endif /* defined(__Adventures_on_the_farm__Product__) */
