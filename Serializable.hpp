#pragma once

#include <tuple>
#include <memory>

template<typename Derived>
struct BaseImpl
{
    virtual void serialize(std::ostream &s, const Derived &) = 0;
};

struct Output
{
    template<typename Obj>
    static void serializeMember(std::ostream &s, const Obj &member)
    {
        s << member << std::endl;
    }
};

template<typename OutputPolicy, typename Derived, typename Tuple, size_t ...I>
void printAttrs(std::ostream &s, std::index_sequence<I...>, const Tuple &tuple, const Derived &obj);

template<typename OutputPolicy, typename Derived, typename Tuple>
void printAttrs(std::ostream &, std::index_sequence<>, const Tuple &, const Derived &)
{}

template<typename OutputPolicy, typename Derived, typename Tuple, size_t I, size_t ...Rest>
void printAttrs(std::ostream &s, std::index_sequence<I, Rest...>, const Tuple &tuple, const Derived &obj)
{
    auto memberPointer = std::get<I>(tuple);

    OutputPolicy::serializeMember(s, obj.*memberPointer);

    printAttrs<OutputPolicy>(s, std::index_sequence<Rest...>(), tuple, obj);
}

template<typename Derived, typename OutputPolicy = Output>
class Serializable
{
    template<typename ...A>
    struct Impl : BaseImpl<Derived>
    {
        Impl(A...attrs)
                :
                _tuple(attrs...) {}

        void serialize(std::ostream &s, const Derived &obj) final
        {
            printAttrs<OutputPolicy>(s, std::index_sequence_for<A...>(), _tuple, obj);
        }

        std::tuple<A...> _tuple;
    };

public:
    template<typename ...MemberPointers>
    Serializable(const MemberPointers &...attrs)
            : _impl(
            new Impl<MemberPointers...>(
                    attrs...))
    {
    }

    friend std::ostream &operator<<(std::ostream &s, const Serializable &obj)
    {
        obj._impl->serialize(s, static_cast<const Derived &>(obj));
        return s;
    }

private:
    std::shared_ptr<BaseImpl<Derived>> _impl;
};
