/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENDABLE_HPP
#define POWSYBL_IIDM_EXTENDABLE_HPP

#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Extendable {
public:
    Extendable() = default;

    Extendable(Extendable&& extendable) noexcept;

    virtual ~Extendable() noexcept = default;

    void addExtension(std::unique_ptr<Extension>&& extension);

    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type>
    stdcxx::CReference<E> findExtension() const;

    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type>
    stdcxx::Reference<E> findExtension();

    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type>
    const E& getExtension() const;

    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type>
    E& getExtension();

    stdcxx::const_range<Extension> getExtensions() const;

    stdcxx::range<Extension> getExtensions();

    template <typename E, typename = typename std::enable_if<std::is_base_of<Extension, E>::value>::type>
    void removeExtension();

private:
    std::map<std::string, std::unique_ptr<Extension> > m_extensionsByName;

    std::unordered_map<std::type_index, std::reference_wrapper<Extension> > m_extensionsByType;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Extendable.hxx>

#endif  // POWSYBL_IIDM_EXTENDABLE_HPP
