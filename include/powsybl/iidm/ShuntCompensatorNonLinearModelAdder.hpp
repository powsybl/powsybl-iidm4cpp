/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP

#include <functional>

#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensatorAdder;

namespace shunt_compensator_view {

class ShuntCompensatorNonLinearModelAdder {
public:
    class SectionAdder {
    public:
        SectionAdder(ShuntCompensatorAdder& adder, ShuntCompensatorNonLinearModelAdder& parent, unsigned long index, std::vector<ShuntCompensatorNonLinearModel::Section>& sections);

        ShuntCompensatorNonLinearModelAdder& endSection();

        SectionAdder& setB(double b);

        SectionAdder& setG(double g);

    private:
        double m_b = stdcxx::nan();

        double m_g = stdcxx::nan();

        ShuntCompensatorAdder& m_adder;

        ShuntCompensatorNonLinearModelAdder& m_parent;

        unsigned long m_index;

        std::vector<ShuntCompensatorNonLinearModel::Section>& m_sections;
    };

public:
    explicit ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent);

    ShuntCompensatorAdder& add();

    SectionAdder beginSection();

private:
    friend class iidm::ShuntCompensatorAdder;

private:
    std::vector<ShuntCompensatorNonLinearModel::Section> m_sections;

    unsigned long m_index = 1;

    std::reference_wrapper<ShuntCompensatorAdder> m_parent;
};

}  // namespace shunt_compensator_view

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODELADDER_HPP
