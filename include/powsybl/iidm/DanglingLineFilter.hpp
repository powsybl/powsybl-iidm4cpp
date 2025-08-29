/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEFILTER_HPP
#define POWSYBL_IIDM_DANGLINGLINEFILTER_HPP

#include <functional>

namespace powsybl {

namespace iidm {

class DanglingLine;

class DanglingLineFilter {
public: 

    static const DanglingLineFilter& ALL();
    static const DanglingLineFilter& PAIRED();
    static const DanglingLineFilter& UNPAIRED();

    DanglingLineFilter(const DanglingLineFilter&) = delete;
    DanglingLineFilter(DanglingLineFilter&&) = delete;
    ~DanglingLineFilter() = default;

    const std::function<bool(const DanglingLine&)>& getPredicate() const;

private:
    DanglingLineFilter(std::function<bool(const DanglingLine&)> pred) noexcept;

    std::function<bool(const DanglingLine&)> m_predicate;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEFILTER_HPP
