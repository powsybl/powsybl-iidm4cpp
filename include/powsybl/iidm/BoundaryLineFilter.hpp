/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARYLINEFILTER_HPP
#define POWSYBL_IIDM_BOUNDARYLINEFILTER_HPP

#include <functional>

namespace powsybl {

namespace iidm {

class BoundaryLine;

class BoundaryLineFilter {
public: 

    static const BoundaryLineFilter& ALL();
    static const BoundaryLineFilter& PAIRED();
    static const BoundaryLineFilter& UNPAIRED();

    BoundaryLineFilter(const BoundaryLineFilter&) = delete;
    BoundaryLineFilter(BoundaryLineFilter&&) = delete;
    ~BoundaryLineFilter() = default;

    const std::function<bool(const BoundaryLine&)>& getPredicate() const;

private:
    BoundaryLineFilter(std::function<bool(const BoundaryLine&)> pred) noexcept;

    std::function<bool(const BoundaryLine&)> m_predicate;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARYLINEFILTER_HPP
