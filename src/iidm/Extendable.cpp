/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Extendable.hpp>

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

Extendable::Extendable(Extendable&& extendable) noexcept :
    m_extensionsByName(std::move(extendable.m_extensionsByName)),
    m_extensionsByType(std::move(extendable.m_extensionsByType)) {

    for (auto& it : m_extensionsByName) {
        it.second->setExtendable(*this);
    }
}

void Extendable::addExtension(std::unique_ptr<Extension>&& extension) {
    auto it = m_extensionsByName.find(extension->getName());
    if (it != m_extensionsByName.end()) {
        // Clean the existing extension
        it->second->setExtendable(stdcxx::ref<Extendable>());
        m_extensionsByName.erase(extension->getName());
        m_extensionsByType.erase(extension->getType());
    }

    extension->setExtendable(*this);
    auto newIt = m_extensionsByName.emplace(extension->getName(), std::move(extension));

    std::reference_wrapper<Extension> refExtension = *newIt.first->second;
    m_extensionsByType.emplace(refExtension.get().getType(), refExtension);
}

stdcxx::const_range<Extension> Extendable::getExtensions() const {
    return boost::adaptors::values(m_extensionsByName) | boost::adaptors::indirected;
}

stdcxx::range<Extension> Extendable::getExtensions() {
    return boost::adaptors::values(m_extensionsByName) | boost::adaptors::indirected;
}

}  // namespace iidm

}  // namespace powsybl