/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_DISTINCTPREDICATE_HPP
#define POWSYBL_IIDM_UTIL_DISTINCTPREDICATE_HPP

#include <memory>
#include <set>
#include <string>

#include <iostream>

namespace powsybl {

namespace iidm {

class Identifiable;

class DistinctPredicate {
public:
    DistinctPredicate();

    ~DistinctPredicate() = default;

    DistinctPredicate(const DistinctPredicate&) = default;

    DistinctPredicate(DistinctPredicate&&) = default;

    bool operator()(const Identifiable& identifiable) const;

private:
    std::shared_ptr<std::set<std::string> > m_ids;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DISTINCTPREDICATE_HPP
