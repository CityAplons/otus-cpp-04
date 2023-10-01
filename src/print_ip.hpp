#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <type_traits>
#include <iterator>

namespace otus {

/// @brief SFINAE print_ip implementation
namespace impl {

/// @brief  Check specialization for vector and list
/// @tparam Container - true if std::list or std::vector
template <typename Container>
struct is_container : std::false_type { };

template <typename... Ts> 
struct is_container<std::vector<Ts...>> : std::true_type { };

template <typename... Ts> 
struct is_container<std::list<Ts...>> : std::true_type { };

/// @brief container type SFINAE detection. Print it with forrange.
/// @tparam T - generic type
/// @param value - generic value, if iterable container, this will be called
/// @param  _ - shadow parameter to go deeper
/// @return not-applicable, ignore ""
template <typename T, std::enable_if_t<is_container<T>::value, bool> = true>
auto print_ip(T& value, int) {
  for (auto&& item : value) {
    std::cout << item << '.';
  }
  std::cout << "\b \b\n";
}

/// @brief  Check specialization for tuple
/// @tparam Container - true if std::tuple
template <typename Container>
struct is_tuple : std::false_type { };

template <typename... Ts> 
struct is_tuple<std::tuple<Ts...>> : std::true_type { };

// TODO: check for the same type
template<class TupType, size_t... I>
void print_tuple(const TupType& _tup, std::index_sequence<I...>)
{
    (..., (std::cout << (I == 0? "" : ".") << std::get<I>(_tup)));
    std::cout << "\n";
}

template<class... T>
void print_tuple (const std::tuple<T...>& _tup)
{
    print_tuple(_tup, std::make_index_sequence<sizeof...(T)>());
}

/// @brief std::tuple type SFINAE detection. Using fold expressions to print it.
/// @tparam T - generic type
/// @param value - generic value, if std::tuple, this will be called
/// @param  _ - shadow parameter to go deeper
/// @return not-applicable, ignore ""
template <typename T, std::enable_if_t<is_tuple<T>::value, bool> = true>
auto print_ip(T& value, int) {
  print_tuple(value);
}

/// @brief std::string type SFINAE detection. Just print it.
/// @tparam T - generic type
/// @param value - generic value, if std::string, this will be called
/// @param  _ - shadow parameter to go deeper
/// @return not-applicable, ignore ""
template <typename T>
auto print_ip(T& value, int) -> decltype(value.c_str()) {
  std::cout << value << '\n';
  return "";
}

/// @brief numeric types SFINAE detection. Represent value as bytes and print
/// them with following dot
/// @tparam T - generic type
/// @param value - generic value, if std::string, this will be called
/// @param  _ - shadow parameter to stop here
/// @return no return
template <typename T>
auto print_ip(T& value, long) {
  size_t bytesAmount = sizeof(value);
  for (size_t i = bytesAmount; i > 0; --i) {
    std::cout << unsigned(static_cast<uint8_t>((value >> 8 * (i - 1)) & 0xFF));
    if (i != 1) {
      std::cout << '.';
    }
  }
  std::cout << '\n';
}

}  // namespace impl

/// @brief Common call
/// @tparam T - generic type
/// @param value - generic value
template <typename T>
void print_ip(T&& value) {
  impl::print_ip(value, int{});
}

}  // namespace otus
