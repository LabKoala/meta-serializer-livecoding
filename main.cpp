#include <dlfcn.h>

#include <string>
#include <iostream>

class DLloader
{
public:
    DLloader(const std::string &path) : _handle(dlopen(path.c_str(), RTLD_GLOBAL | RTLD_NOW))
    {
        if (_handle == nullptr)
            throw std::runtime_error("Woops");
    }
    ~DLloader() { dlclose(_handle); }

    template<typename T, typename ...Args>
    T execute(const std::string &name, const Args &...args)
    {
        auto ret = dlsym(_handle, name.c_str());
        auto func = (T(*)(const Args &...))ret;

        return func(args...);
    }

private:
    void *_handle;
};

#include "Serializable.hpp"

struct Child : public Serializable<Child, Output>
{
    Child() : Serializable(&Child::toto) {}

    std::string toto = "biteuh";
};

struct MyOutput
{
    template<typename Obj>
    static void serializeMember(std::ostream &s, const Obj &member)
    {
        s << "Member:" << member << std::endl;
    }
};

class Toto : public Serializable<Toto, MyOutput>
{
public:
    Toto() : Serializable(&Toto::x, &Toto::y, &Toto::c),
             x(42), y(84)
    {}

public:
    int x;
    int y;
    Child c;
};

#include "for_each.hpp"

int main()
{
/*    Toto t;
    std::cout << t << std::endl;

    Toto t2 = t;
    t2.x = 12;
    std::cout << t2 << std::endl;*/

    auto t = std::make_tuple("Toto", 42);
    for_each(t, [](const auto &param)
    {
        std::cout << param << std::endl;
    });
}