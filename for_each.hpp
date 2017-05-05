#pragma once

namespace detail
{
    template<typename Func, typename ...A, size_t ...I>
    void for_each(const std::tuple<A...> &tuple, const Func &f, std::index_sequence<I...>);

    template<typename Func, typename ...A>
    void for_each(const std::tuple<A...> &, const Func &, std::index_sequence<>)
    {}

    template<typename Func, typename ...A, size_t I, size_t ...Rest>
    void for_each(const std::tuple<A...> &tuple, const Func &f, std::index_sequence<I, Rest...>)
    {
        f(std::get<I>(tuple));
        for_each(tuple, f, std::index_sequence<Rest...>());
    }
}

template<typename Func, typename ...A>
void for_each(const std::tuple<A...> &tuple, const Func &f)
{
    detail::for_each(tuple, f, std::index_sequence_for<A...>());
}