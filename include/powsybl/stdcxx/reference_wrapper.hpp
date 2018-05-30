/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_REFERENCE_WRAPPER_HPP
#define POWSYBL_STDCXX_REFERENCE_WRAPPER_HPP

#include <powsybl/PowsyblException.hpp>

namespace stdcxx {

template <typename T>
class reference_wrapper {
public:
    using type = T;

public:
    reference_wrapper() noexcept :
        m_pointer(nullptr) {
    }

    explicit reference_wrapper(T& reference) noexcept :
        m_pointer(std::addressof(reference)) {
    }

    reference_wrapper(const reference_wrapper&) = default;

    reference_wrapper(reference_wrapper&&) = default;

    reference_wrapper& operator=(const reference_wrapper&) = default;

    reference_wrapper& operator=(reference_wrapper&&) = default;

    bool operator==(const reference_wrapper& reference) const {
        return m_pointer == reference.m_pointer;
    }

    bool operator!() const noexcept {
        return m_pointer == nullptr;
    }

    operator bool() const noexcept {
        return m_pointer != nullptr;
    }

    operator T&() const {
        return get();
    }

    T& get() const {
        if (m_pointer == nullptr) {
            throw std::runtime_error("m_pointer is null");
        }
        return *m_pointer;
    }

    void reset() noexcept {
        m_pointer = nullptr;
    }

private:
    reference_wrapper(T&& reference) = delete;

private:
    T* m_pointer;
};

template <typename T> using CReference = reference_wrapper<const T>;

template <typename T> using Reference = reference_wrapper<T>;

template <typename T>
Reference<T> ref() {
    return Reference<T>();
}

template <typename T>
Reference<T> ref(T& reference) {
    return Reference<T>(reference);
}

template <typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
Reference<T> ref(U& reference) {
    return Reference<T>(dynamic_cast<T&>(reference));
}

template <typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
Reference<T> ref(const Reference<U>& reference) {
    return static_cast<bool>(reference) ? Reference<T>(dynamic_cast<T&>(reference.get())) : Reference<T>();
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_REFERENCE_WRAPPER_HPP
