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

namespace powsybl {

namespace iidm {

class Extendable {
public:
    Extendable() = default;

    Extendable(Extendable&& extendable) = default;

    virtual ~Extendable() noexcept = default;

    void addExtension(std::unique_ptr<Extension>&& extension);

private:
    std::map<std::string, std::unique_ptr<Extension> > m_extensionsByName;

    std::unordered_map<std::type_index, std::reference_wrapper<Extension> > m_extensionsByType;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENDABLE_HPP
