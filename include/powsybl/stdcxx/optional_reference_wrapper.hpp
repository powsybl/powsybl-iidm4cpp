/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPTIONAL_REFERENCE_WRAPPER_HPP
#define POWSYBL_IIDM_OPTIONAL_REFERENCE_WRAPPER_HPP

#include <powsybl/PowsyblException.hpp>

namespace stdcxx {

template <typename T>
class optional_reference_wrapper {
public:
    using type = T;

public:
    optional_reference_wrapper() noexcept :
        m_pointer(nullptr) {
    }

    explicit optional_reference_wrapper(T& reference) noexcept :
        m_pointer(std::addressof(reference)) {
    }

    optional_reference_wrapper(const optional_reference_wrapper&) = default;

    optional_reference_wrapper& operator=(const optional_reference_wrapper&) = default;

    bool operator!() const noexcept {
        return m_pointer == nullptr;
    }

    operator bool() const noexcept {
        return m_pointer != nullptr;
    }

    operator T&() const noexcept {
        return get();
    }

    T& get() const noexcept {
        if (m_pointer == nullptr) {
            throw std::runtime_error("m_pointer is null");
        }
        return *m_pointer;
    }

private:
    optional_reference_wrapper(T&& reference) = delete;

private:
    T* m_pointer;
};

template <typename T>
optional_reference_wrapper<T> optref() {
    return optional_reference_wrapper<T>();
}

template <typename T>
optional_reference_wrapper<T> optref(T& reference) {
    return optional_reference_wrapper<T>(reference);
}

template <typename T>
optional_reference_wrapper<T> optcref() {
    return optional_reference_wrapper<T>();
}

template <typename T>
optional_reference_wrapper<T> optcref(const T& reference) {
    return optional_reference_wrapper<T>(reference);
}

template <typename T> using Optional = optional_reference_wrapper<T>;

}

#endif  // POWSYBL_IIDM_OPTIONAL_REFERENCE_WRAPPER_HPP
