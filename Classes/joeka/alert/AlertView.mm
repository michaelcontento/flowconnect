#include "AlertView.h"
#import <UIKit/UIKit.h>
#import "AlertViewIOS.h"

void AlertView::createAlert(const char* title, const char* message, const char* cancelButtonTitle)
{
    [[AlertViewIOS sharedInstance] createAlertView:[NSString stringWithUTF8String:title]
                                        andMessage:[NSString stringWithUTF8String:message]
                              andCancelButtonTitle:[NSString stringWithUTF8String:cancelButtonTitle]];
}

int AlertView::addAlertButton(const char* buttonTitle)
{
    return [[AlertViewIOS sharedInstance] addAlertButton:[NSString stringWithUTF8String:buttonTitle]];
}

void AlertView::showAlert(AlertViewDelegate* delegate)
{
    [[AlertViewIOS sharedInstance] showAlertViewWithDelegate:delegate];
}

void AlertView::cancelAlert(void)
{
    [[AlertViewIOS sharedInstance] cancelAlertView];
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void AlertView::showAlertObjc(void *delegate)
{
    [[AlertViewIOS sharedInstance] showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate];
}
#endif