#ifndef SERIALIZATIONTEST_H
#define SERIALIZATIONTEST_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <array>

#include <boost/hana.hpp>

#include "BaseTest.h"

using namespace boost;

template <typename Xs>
std::string join(Xs&& xs, std::string sep) {
    return hana::fold(hana::intersperse(std::forward<Xs>(xs), sep), "", hana::_ + hana::_);
}
inline std::string quote(std::string s) { return "\"" + s + "\""; }
template <typename T>
auto to_json(T const& x) -> decltype(std::to_string(x)) {
    return std::to_string(x);
}
inline std::string to_json(char c) { return quote({c}); }
inline std::string to_json(std::string s) { return quote(s); }

// 模版预声明
template <typename Xs>
std::enable_if_t<std::is_same<Xs, std::vector<typename Xs::value_type, typename Xs::allocator_type>>::value,
std::string> to_json(Xs const& xs);

template <typename Xs>
std::enable_if_t<std::is_same<Xs, std::map<typename Xs::key_type, typename Xs::mapped_type, typename Xs::key_compare, typename Xs::allocator_type>>::value,
std::string> to_json(Xs const& xs);

template <typename T>
std::enable_if_t<hana::Struct<T>::value,
std::string> to_json(T const& x) {
    auto json = hana::transform(hana::keys(x), [&](auto name) {
        auto const& member = hana::at_key(x, name);
        return quote(hana::to<char const*>(name)) + " : " + to_json(member);
    });
    return "{" + join(std::move(json), ", ") + "}";
}

template <typename Xs>
std::enable_if_t<hana::Sequence<Xs>::value,
std::string> to_json(Xs const& xs) {
    auto json = hana::transform(xs, [](auto const& x) {
        return to_json(x);
    });
    return "[" + join(std::move(json), ", ") + "]";
}

template <typename Xs>
std::enable_if_t<std::is_same<Xs, std::vector<typename Xs::value_type, typename Xs::allocator_type>>::value,
std::string> to_json(Xs const& xs) {
    std::string result;
    for (auto& x : xs) {
        result += to_json(x) + ",";
    }

    return "[" + result + "]";
}

template <typename Xs>
std::enable_if_t<std::is_same<Xs, std::map<typename Xs::key_type, typename Xs::mapped_type, typename Xs::key_compare, typename Xs::allocator_type>>::value,
std::string> to_json(Xs const& xs) {
    std::string result;
    for (auto& x : xs) {
        result += to_json(x.first) + " : " + to_json(x.second) + ",";
    }

    return "[" + result + "]";
}

class SerializationTest : public BaseTest
{
public:
    SerializationTest();

    virtual void Init();
};

#endif // SERIALIZATIONTEST_H
