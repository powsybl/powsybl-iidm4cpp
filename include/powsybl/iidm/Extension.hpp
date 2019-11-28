/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSION_HPP
#define POWSYBL_IIDM_EXTENSION_HPP

#include <string>
#include <typeindex>

#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Extendable;

class Extension {
public:
    Extension() = default;

    explicit Extension(Extendable& extendable);

    virtual ~Extension() noexcept = default;

    template <typename E = Extendable, typename = typename std::enable_if<std::is_base_of<Extendable, E>::value>::type>
    stdcxx::CReference<E> getExtendable() const;

    template <typename E = Extendable, typename = typename std::enable_if<std::is_base_of<Extendable, E>::value>::type>
    stdcxx::Reference<E> getExtendable();

    virtual const std::string& getName() const = 0;

    virtual const std::type_index& getType() const = 0;

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
