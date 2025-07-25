/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_REFERENCE_HPP
#define POWSYBL_STDCXX_REFERENCE_HPP

#include <memory>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/hash.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace stdcxx {

template <typename T>
class Reference {
public:
    using type = T;

public:
    Reference() noexcept = default;

    explicit Reference(T& reference) noexcept :
        m_pointer(std::addressof(reference)) {
    }

    Reference(const Reference&) = default;

    Reference(T&& reference) noexcept = delete;

    Reference(Reference&&) noexcept = default;

    Reference& operator=(const Reference&) = default;

    Reference& operator=(Reference&&) noexcept = default;

    Reference& operator=(T& reference) {
        m_pointer = std::addressof(reference);
        return *this;
    }

    bool operator==(const Reference& reference) const {
        return m_pointer == reference.m_pointer;
    }

    bool operator!=(const Reference& reference) const {
        return m_pointer != reference.m_pointer;
    }

    bool operator==(const T& reference) const {
        return m_pointer == &reference;
    }

    bool operator!=(const T& reference) const {
        return m_pointer != &reference;
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
    T* m_pointer = nullptr;
};

template <typename T> using CReference = Reference<const T>;

template <typename T>
CReference<T> cref() {
    return CReference<T>();
}

template <typename T>
CReference<T> cref(const T& reference) {
    return CReference<T>(reference);
}

template <typename T>
CReference<T> cref(T& reference) {
    return CReference<T>(reference);
}

template <typename T>
CReference<T> cref(const std::unique_ptr<T>& pointer) {
    return static_cast<bool>(pointer) ? CReference<T>(*pointer) : CReference<T>();
}

template <typename T, typename U, typename = typename std::enable_if<!std::is_abstract<U>::value && std::is_same<T, U>::value>::type>
CReference<T> cref(const stdcxx::optional<U>& optional) {
    return static_cast<bool>(optional) ? CReference<T>(*optional) : CReference<T>();
}

template <typename T>
CReference<T> cref(const Reference<T>& reference) {
    return static_cast<bool>(reference) ? CReference<T>(reference.get()) : CReference<T>();
}

template <typename T>
CReference<T> cref(const std::reference_wrapper<const T>& reference) {
    return CReference<T>(reference.get());
}

template <typename T>
Reference<T> ref() {
    return Reference<T>();
}

template <typename T>
Reference<T> ref(T& reference) {
    return Reference<T>(reference);
}

template <typename T>
Reference<T> ref(const std::unique_ptr<T>& pointer) {
    return static_cast<bool>(pointer) ? Reference<T>(*pointer) : Reference<T>();
}

template <typename T, typename U, typename = typename std::enable_if<!std::is_abstract<U>::value && std::is_same<T, U>::value>::type>
Reference<T> ref(stdcxx::optional<U>& optional) {
    return static_cast<bool>(optional) ? Reference<T>(*optional) : Reference<T>();
}

template <typename T, typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value && !std::is_same<T, U>::value>::type>
Reference<T> ref(U& reference) {
    return Reference<T>(dynamic_cast<T&>(reference));
}

template <typename T>
Reference<T> ref(const Reference<T>& reference) {
    return Reference<T>(reference);
}

template <typename T>
Reference<T> ref(const CReference<T>& reference) {
    return static_cast<bool>(reference) ? Reference<T>(const_cast<T&>(reference.get())) : Reference<T>();
}

template <typename T>
Reference<T> ref(const T& reference) {
    return Reference<T>(const_cast<T&>(reference));
}

/**
 * Upcast / Downcast a Reference instance from U to T
 */
template <typename T, typename U, typename = typename std::enable_if<!std::is_same<T, U>::value>::type>
Reference<T> ref(const Reference<U>& reference) {
    return static_cast<bool>(reference) ? Reference<T>(dynamic_cast<T&>(reference.get())) : Reference<T>();
}

template <typename T, typename U, typename = typename std::enable_if<!std::is_same<T, U>::value>::type>
CReference<T> cref(const Reference<U>& reference) {
    return static_cast<bool>(reference) ? CReference<T>(dynamic_cast<const T&>(reference.get())) : CReference<T>();
}

template <typename T, typename U, typename = typename std::enable_if<!std::is_same<T, U>::value>::type>
CReference<T> cref(const CReference<U>& reference) {
    return static_cast<bool>(reference) ? CReference<T>(dynamic_cast<const T&>(reference.get())) : CReference<T>();
}

template <typename T, typename U>
const U& map(const T& reference) {
    return dynamic_cast<const U&>(reference.get());
}

/**
 * Specialization of std::equal_to for std::reference_wrapper
 */
template <typename T>
class equal_to {
public:
    bool operator()(const std::reference_wrapper<T>& reference1, const std::reference_wrapper<T>& reference2) const {
        return std::addressof(reference1.get()) == std::addressof(reference2.get());
    }
};

/**
 * Specialization of std::hash for std::reference_wrapper
 */
template <typename T>
class hash<std::reference_wrapper<T>, false> {
public:
    std::size_t operator()(const std::reference_wrapper<T>& value) const {
        return std::hash<T*>()(std::addressof(value.get()));
    }
};

/**
 * Specialization of std::less for std::reference_wrapper
 */
template <typename T>
class less {
public:
    bool operator()(const std::reference_wrapper<T>& reference1, const std::reference_wrapper<T>& reference2) const {
        return std::addressof(reference1.get()) < std::addressof(reference2.get());
    }
};

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_REFERENCE_HPP
