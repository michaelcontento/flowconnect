#import "AlertViewIOS.h"

using namespace cocos2d;

@implementation AlertViewIOS

static AlertViewIOS *s_sharedInstance;

+ (AlertViewIOS *)sharedInstance
{
    if (!s_sharedInstance) {
        s_sharedInstance = [[AlertViewIOS alloc] init];
    }
    return s_sharedInstance;
}

- (void)dealloc
{
    [self removeAlertView];
    [super dealloc];
    s_sharedInstance = nil;
}

- (void)createAlertView:(NSString *)title
             andMessage:(NSString *)message
   andCancelButtonTitle:(NSString *)cancelButtonTitle
{
    if (alertView_) {
        return;
    }
    
    alertView_ = [[UIAlertView alloc] initWithTitle:title
                                            message:message
                                           delegate:self
                                  cancelButtonTitle:cancelButtonTitle
                                  otherButtonTitles:nil];
}

- (NSInteger)addAlertButton:(NSString *)buttonTitle
{
    if (!alertView_) {
        return 0;
    }

    return [alertView_ addButtonWithTitle:buttonTitle];
}

- (void)showAlertViewWithDelegate:(AlertViewDelegate *)delegate
{
    if (!alertView_) {
        return;
    }

    alertViewDelegates_ = delegate;
    [alertView_ show];
}

- (void)removeAlertView
{
    if (!alertView_) {
        return;
    }

    [alertView_ release];
    alertView_ = nil;
    alertViewDelegates_ = nil;
}

- (void)cancelAlertView
{
    if (!alertView_) {
        return;
    }
    
    [alertView_ dismissWithClickedButtonIndex:0 animated:YES];
    [self removeAlertView];
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
- (void)showAlertViewWithObjcDelegate:(id<UIAlertViewDelegate>)delegate
{
    if (!alertView_) {
        return;
    }
    
    [alertView_ setDelegate:delegate];
    [alertView_ show];
}
#endif

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    AlertViewDelegate* delegate = alertViewDelegates_;
    [self removeAlertView];
    
    if (delegate) {
        delegate->alertViewClickedButtonAtIndex(buttonIndex);
    }
}

@end