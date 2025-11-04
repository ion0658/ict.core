#pragma once

#ifndef ICT_CORE_CONCEPTS_HPP
#define ICT_CORE_CONCEPTS_HPP

#include <concepts>
#include <type_traits>

namespace ict::core {

template <typename Base, typename Derived>
concept derived_from_and_not_same = std::derived_from<Derived, Base> && not std::same_as<Derived, Base>;

template <typename T, typename U>
concept is_weak_same_as = std::same_as<typename std::remove_cvref<T>::type, typename std::remove_cvref<U>::type>;

template <class T>
concept is_void = std::is_void_v<T>;

}  // namespace ict::core

#endif  // ICT_CORE_CONCEPTS_HPP

