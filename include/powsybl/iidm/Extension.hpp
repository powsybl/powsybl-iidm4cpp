/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSION_HPP
#define POWSYBL_IIDM_EXTENSION_HPP

#include <string>
#include <type_traits>
#include <typeindex>

#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Extendable;

class Extension {
public:
    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type, typename... Args>
    static std::unique_ptr<Extension> create(Args&&... args);

public:
    Extension() = default;

    explicit Extension(Extendable& extendable);

    Extension(const Extension&) = delete;

    Extension(Extension&&) noexcept = delete;

    virtual ~Extension() noexcept = default;

    Extension& operator=(const Extension&) = delete;

    Extension& operator=(Extension&&) noexcept = delete;

    template <typename E = Extendable, typename = typename std::enable_if<std::is_base_of<Extendable, E>::value>::type>
    stdcxx::CReference<E> getExtendable() const;

    template <typename E = Extendable, typename = typename std::enable_if<std::is_base_of<Extendable, E>::value>::type>
    stdcxx::Reference<E> getExtendable();

    virtual const std::string& getName() const = 0;

    virtual const std::type_index& getType() const = 0;

    void setExtendable(Extendable& extendable);

    void setExtendable(const stdcxx::Reference<Extendable>& extendable);

private:
    virtual void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const = 0;

private:
    stdcxx::Reference<Extendable> m_extendable;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Extension.hxx>

#endif  // POWSYBL_IIDM_EXTENSION_HPP
