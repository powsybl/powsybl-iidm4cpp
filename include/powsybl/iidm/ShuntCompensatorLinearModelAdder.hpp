/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP

#include <powsybl/iidm/ShuntCompensatorModelAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensatorAdder;

namespace shunt_compensator {

class ShuntCompensatorLinearModelAdder : public ShuntCompensatorModelAdder {
public:  // ShuntCompensatorModelAdder
    ShuntCompensatorAdder& add() override;

public:
    explicit ShuntCompensatorLinearModelAdder(ShuntCompensatorAdder& parent);

    ShuntCompensatorLinearModelAdder(ShuntCompensatorAdder& parent, const ShuntCompensatorLinearModelAdder& adder);

    ShuntCompensatorLinearModelAdder& setBPerSection(double bPerSection);

    ShuntCompensatorLinearModelAdder& setGPerSection(double gPerSection);

    ShuntCompensatorLinearModelAdder& setMaximumSectionCount(unsigned long maximumSectionCount);

private:  // ShuntCompensatorModelAdder
    std::unique_ptr<ShuntCompensatorModel> build(ShuntCompensator& shuntCompensator) const override;

    std::unique_ptr<ShuntCompensatorModelAdder> clone(ShuntCompensatorAdder& parent) const override;

    unsigned long getMaximumSectionCount() const override;

private:
    ShuntCompensatorAdder& m_parent;

    double m_bPerSection = stdcxx::nan();

    double m_gPerSection = stdcxx::nan();

    stdcxx::optional<unsigned long> m_maximumSectionCount;
};

}  // namespace shunt_compensator

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODELADDER_HPP
