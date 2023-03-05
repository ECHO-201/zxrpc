#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T * instance()
    {
        static T * instance = nullptr;
        if (instance == nullptr)
            instance = new T();
        return instance;
    }

protected:
    Singleton() {}
    Singleton(const Singleton<T> &);
    Singleton<T> & operator = (const Singleton<T> &);
    ~Singleton() {}
};


#endif // SINGLETON_H
