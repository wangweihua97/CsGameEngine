#ifndef EventCenter_H
#define EventCenter_H
#include <string>
#include <vector>
#include <Map>
#include <boost/function.hpp>
using namespace std;

class MultipleBase
{

};
class MultipleAciton : public MultipleBase
{
private:
    map<int ,boost::function<void()>> _map;
    unsigned int _id;
public:
    MultipleAciton()
    {
        _id = 0;
    }
    int AddAction(boost::function<void()> ation)
    {
        _map[++_id] = ation;
        return _id;
    }

    void RemoveAction(int id)
    {
        _map.erase(id);
    }
    void Invoke()
    {
        for (map<int, boost::function<void()>>::iterator itera = _map.begin(); itera != _map.end(); itera++)
        {
            itera->second();
            
        }
    }
};
template <typename T>
class MultipleAcitonOneParame : public MultipleBase
{
private:
    map<int, boost::function<void(T)>> _map;
    unsigned int _id;
public:
    MultipleAcitonOneParame()
    {
        _id = 0;
    }
    int AddAction(boost::function<void(T)> ation)
    {
        _map[++_id] = ation;
        return _id;
    }

    void RemoveAction(int id)
    {
        _map.erase(id);
    }
    void Invoke(T value)
    {
        for (auto iter = _map.begin(); iter != _map.end(); iter++)
        {
            iter->second(value);

        }
    }
};

//template <typename T , typename V>
//class MultipleAcitonTwoParame : public MultipleBase
//{
//private:
//    map<int, boost::function<void(T,V)>> _map;
//    unsigned int _id;
//public:
//    MultipleAcitonTwoParame()
//    {
//        _id = 0;
//    }
//    int AddAction(boost::function<void(T,V)> ation)
//    {
//        _map[++_id] = ation;
//        return _id;
//    }
//
//    void RemoveAction(int id)
//    {
//        _map.erase(id);
//    }
//    void Invoke(T value ,V value2)
//    {
//        for (auto iter = _map.begin(); iter != _map.end(); iter++)
//        {
//            iter->second(value , value2);
//
//        }
//    }
//};

template <typename T , typename... Args>
class MultipleAcitonMoreParame : public MultipleBase
{
private:
    map<int, boost::function<void(T, Args...)>> _map;
    unsigned int _id;
public:
    MultipleAcitonMoreParame()
    {
        _id = 0;
    }
    int AddAction(boost::function<void(T, Args...)> ation)
    {
        _map[++_id] = ation;
        return _id;
    }

    void RemoveAction(int id)
    {
        _map.erase(id);
    }
    void Invoke(T value, Args... args)
    {
        for (auto iter = _map.begin(); iter != _map.end(); iter++)
        {
            iter->second(value ,args...);

        }
    }
};

class EventCenter
{
public:
    static EventCenter* Instance;
    int AddEvent(string eventName ,boost::function<void()> ation)
    {
        if (_map.find(eventName) == _map.end())
        {
            _map[eventName] = new MultipleAciton();
        }
        return reinterpret_cast<MultipleAciton*>(_map[eventName])->AddAction(ation);
    }
    template <typename T>
    int AddEvent(string eventName ,boost::function<void(T)> ation)
    {
        if (_map.find(eventName) == _map.end())
        {
            _map[eventName] = new MultipleAcitonOneParame<T>();
        }
        return reinterpret_cast<MultipleAcitonOneParame<T>*>(_map[eventName])->AddAction(ation);
    }
    /*template <typename T , typename V>
    int AddEvent(string eventName, boost::function<void(T,V)> ation)
    {
        if (_map.find(eventName) == _map.end())
        {
            _map[eventName] = new MultipleAcitonTwoParame<T, V>();
        }
        return reinterpret_cast<MultipleAcitonTwoParame<T, V>*>(_map[eventName])->AddAction(ation);
    }*/
    template <typename T, typename... Args>
    int AddEvent(string eventName, boost::function<void(T, Args...)> ation)
    {
        if (_map.find(eventName) == _map.end())
        {
            _map[eventName] = new MultipleAcitonMoreParame<T, Args...>();
        }
        return reinterpret_cast<MultipleAcitonMoreParame<T, Args...>*>(_map[eventName])->AddAction(ation);
    }
    void Remove(string eventName, int id)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAciton*>(_map[eventName])->RemoveAction(id);
        }
    }
    template <typename T>
    void Remove(string eventName, int id)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonOneParame<T>*>(_map[eventName])->RemoveAction(id);
        }
        
    }
    /*template <typename T , typename V>
    void Remove(string eventName, int id)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonTwoParame<T,V>*>(_map[eventName])->RemoveAction(id);
        }

    }*/
    template <typename T, typename... Args>
    void Remove(string eventName, int id)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonMoreParame<T, Args...>*>(_map[eventName])->RemoveAction(id);
        }

    }
    void Invoke(string eventName)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAciton*>(_map[eventName])->Invoke();
        }
    }
    template <typename T>
    void Invoke(string eventName ,T value)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonOneParame<T>*>(_map[eventName])->Invoke(value);
        }
    }
    /*template <typename T , typename V>
    void Invoke(string eventName, T value ,V value2)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonTwoParame<T, V>*>(_map[eventName])->Invoke(value , value2);
        }
    }*/
    template <typename T, typename... Args>
    void Invoke(string eventName, T value, Args... args)
    {
        if (_map.find(eventName) != _map.end())
        {
            reinterpret_cast<MultipleAcitonMoreParame<T, Args...>*>(_map[eventName])->Invoke(value, args...);
        }
    }

  
private:
    EventCenter();
    map<string, MultipleBase*> _map;
};
#endif

