#ifndef FlowConnect_Singleton_h
#define FlowConnect_Singleton_h

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T oInstance;
        return oInstance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&)
    {
        return *this;
    }
};

#endif
