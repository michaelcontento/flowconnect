#ifndef __ALERTVIEW_DELEGATE_H_
#define __ALERTVIEW_DELEGATE_H_

class AlertViewDelegate
{
public:
    virtual void alertViewClickedButtonAtIndex(int buttonIndex) = 0;
};

#endif // __ALERTVIEW_DELEGATE_H_
