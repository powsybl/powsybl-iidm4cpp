/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Extendable.hpp>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

void Extendable::addExtension(std::unique_ptr<Extension>&& extension) {
    extension->setExtendable(stdcxx::ref(*this));
    auto it = m_extensionsByName.emplace(std::make_pair(extension->getName(), std::move(extension)));

    std::reference_wrapper<Extension> refExtension = *it.first->second;
    m_extensionsByType.emplace(std::make_pair(refExtension.get().getType(), refExtension));
}

}  // namespace iidm

}  // namespace powsybl