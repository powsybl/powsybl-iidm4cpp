/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP

#include <powsybl/iidm/ShuntCompensatorModelAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensatorAdder;

namespace shunt_compensator {

class ShuntCompensatorNonLinearModelAdder : public ShuntCompensatorModelAdder {
public:
    class SectionAdder {
    public:
        ShuntCompensatorNonLinearModelAdder& endSection();

        SectionAdder& setB(double b);

        SectionAdder& setG(double g);

    private:
        SectionAdder(ShuntCompensatorNonLinearModelAdder& parent);

        SectionAdder(ShuntCompensatorNonLinearModelAdder& parent, const SectionAdder& adder);

        friend class ShuntCompensatorNonLinearModelAdder;

    private:
        ShuntCompensatorNonLinearModelAdder& m_parent;

        double m_b = stdcxx::nan();

        double m_g = stdcxx::nan();
    };

public:  // ShuntCompensatorModelAdder
    ShuntCompensatorAdder& add() override;

public:
    explicit ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent);

    ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent, const ShuntCompensatorNonLinearModelAdder& adder);

    SectionAdder beginSection();

private:  // ShuntCompensatorModelAdder
    std::unique_ptr<ShuntCompensatorModel> build(ShuntCompensator& shuntCompensator) const override;

    std::unique_ptr<ShuntCompensatorModelAdder> clone(ShuntCompensatorAdder& parent) const override;

    unsigned long getMaximumSectionCount() const override;

private:
    std::vector<SectionAdder>& getSectionAdders();

    const ShuntCompensatorAdder& getShuntCompensatorAdder() const;

    friend class SectionAdder;

private:
    ShuntCompensatorAdder& m_parent;

    std::vector<SectionAdder> m_sectionAdders;
};

}  // namespace shunt_compensator

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP
