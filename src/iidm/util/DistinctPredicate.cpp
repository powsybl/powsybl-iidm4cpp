/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/DistinctPredicate.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

DistinctPredicate::DistinctPredicate() :
    m_ids(std::make_shared<std::set<std::string>>()) {

}

bool DistinctPredicate::operator()(const Identifiable& identifiable) const {
    if (m_ids->find(identifiable.getId()) == m_ids->end()) {
        m_ids->insert(identifiable.getId());
        return true;
    }

    return false;
}

}  // namespace iidm

}  // namespace powsybl
