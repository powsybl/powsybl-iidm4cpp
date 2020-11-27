/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP

#include <functional>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensatorAdder;

namespace shunt_compensator_view {

class ShuntCompensatorLinearModelAdder {
public:
    explicit ShuntCompensatorLinearModelAdder(ShuntCompensatorAdder& parent);

    ShuntCompensatorAdder& add();

    ShuntCompensatorLinearModelAdder& setBPerSection(double bPerSection);

    ShuntCompensatorLinearModelAdder& setGPerSection(double gPerSection);

    ShuntCompensatorLinearModelAdder& setMaximumSectionCount(unsigned long maximumSectionCount);

private:
    friend class iidm::ShuntCompensatorAdder;

private:
    double m_bPerSection = stdcxx::nan();

    double m_gPerSection = stdcxx::nan();

    stdcxx::optional<unsigned long> m_maximumSectionCount;

    std::reference_wrapper<ShuntCompensatorAdder> m_parent;
};

}  // namespace shunt_compensator_view

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP
