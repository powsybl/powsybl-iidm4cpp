/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ObjectStore.hpp>

#include <powsybl/stdcxx/memory.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

void ObjectStore::checkId(const std::string& id) {
    checkNotEmpty(id, "Invalid id '" + id + "'");
}

template <>
Identifiable& ObjectStore::get(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it == m_objectsById.end()) {
        throw PowsyblException(logging::format("Unable to find to the identifiable '%1%'", id));
    }

    return *(it->second.get());
}

template <>
unsigned long ObjectStore::getObjectCount<Identifiable>() const {
    return m_objectsById.size();
}

void ObjectStore::remove(Identifiable& identifiable) {
    const auto& it = m_objectsById.find(identifiable.getId());

    // assert that the two instances are identical (not namesake)
    if ((it != m_objectsById.end()) && stdcxx::areSame(*it->second, identifiable)) {
        Identifiables& identifiables = m_objectsByType.find(typeid(identifiable))->second;

        const auto& itIdentifiable = std::find_if(identifiables.begin(), identifiables.end(), [&](std::reference_wrapper<Identifiable>& item)
        {
            return stdcxx::areSame(identifiable, item.get());
        });

        if (itIdentifiable != identifiables.end()) {
            identifiables.erase(itIdentifiable);
            m_objectsById.erase(identifiable.getId());
        }
    }
}

}

}
