/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEPADDER_HPP
#define POWSYBL_IIDM_TAPCHANGERSTEPADDER_HPP

#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

template<typename TCStepAdder, typename TCAdder>
class TapChangerStepAdder : public PropertiesHolder {
public:
    virtual ~TapChangerStepAdder() noexcept = default;

public:
    virtual TCStepAdder& setRho(double rho) = 0;

    virtual TCStepAdder& setR(double r) = 0;

    virtual TCStepAdder& setX(double x) = 0;

    virtual TCStepAdder& setG(double g) = 0;

    virtual TCStepAdder& setB(double b) = 0;

    virtual TCAdder& endStep() = 0;

protected:
    TapChangerStepAdder(TCAdder& parent);

    TCAdder& m_parent;

    double m_rho = stdcxx::nan();

    double m_r = 0.0;

    double m_x = 0.0;

    double m_g = 0.0;

    double m_b = 0.0;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TapChangerStepAdder.hxx>

#endif  // POWSYBL_IIDM_TAPCHANGERSTEPADDER_HPP
