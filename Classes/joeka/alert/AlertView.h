#ifndef __ALERT_VIEW_H_
#define __ALERT_VIEW_H_

#include "cocos2d.h"
#include "AlertViewDelegate.h"

class AlertView
{
public:
    /** @brief Create alert view */
    static void createAlert(const char* title, const char* message, const char* cancelButtonTitle);
    
    /** @brief Add button to alert view, return button index */
    static int addAlertButton(const char* buttonTitle);

    /** @brief Show alert view */
    static void showAlert(AlertViewDelegate* delegate = NULL);

    /** @brief Hide and remove alert view */
    static void cancelAlert(void);

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static void showAlertObjc(void *delegate);
#endif
    
private:
    AlertView(void) {}
    virtual ~AlertView() {}
    
    DISALLOW_COPY_AND_ASSIGN(AlertView);

};

#endif // __ALERT_VIEW_H_