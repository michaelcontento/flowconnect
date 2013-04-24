#include "AlertView.h"
#import <UIKit/UIKit.h>
#import "AlertViewIOS.h"

void AlertView::createAlert(const char* title, const char* message, const char* cancelButtonTitle)
{
    NSString *title_ = [NSString stringWithUTF8String:title ? title : ""];
    NSString *message_ = [NSString stringWithUTF8String:message ? message : ""];
    NSString *cancelButtonTitle_ = cancelButtonTitle ? [NSString stringWithUTF8String:cancelButtonTitle] : nil;
    
    [[AlertViewIOS sharedInstance] createAlertView:title_
                                       andMessage:message_
                             andCancelButtonTitle:cancelButtonTitle_];
}

int AlertView::addAlertButton(const char* buttonTitle)
{
    NSString *buttonTitle_ = [NSString stringWithUTF8String:buttonTitle ? buttonTitle : "Button"];
    return [[AlertViewIOS sharedInstance] addAlertButton:buttonTitle_];
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