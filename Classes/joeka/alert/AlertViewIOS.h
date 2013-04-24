#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "AlertViewDelegate.h"
#include "cocos2d.h"

@interface AlertViewIOS : NSObject < UIAlertViewDelegate >
{
    UIActivityIndicatorView *activityIndicatorView_;
    
    UIAlertView *alertView_;
    AlertViewDelegate *alertViewDelegates_;
}

+ (AlertViewIOS *)sharedInstance;

- (void)createAlertView:(NSString *)title
             andMessage:(NSString *)message
   andCancelButtonTitle:(NSString *)cancelButtonTitle;
- (NSInteger)addAlertButton:(NSString *)buttonTitle;
- (void)showAlertViewWithDelegate:(AlertViewDelegate *)delegate;
- (void)removeAlertView;
- (void)cancelAlertView;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
- (void)showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate;
#endif

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end
