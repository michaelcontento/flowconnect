#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include "UrlOpener.h"

void UrlOpener::open(const char* pszUrl)
{
    NSString *msg = [NSString stringWithCString:pszUrl encoding:NSASCIIStringEncoding];
    NSURL *nsUrl = [NSURL URLWithString:msg];
    [[UIApplication sharedApplication] openURL:nsUrl];
}