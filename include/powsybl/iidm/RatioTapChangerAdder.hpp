/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP

#include <vector>

#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerStepAdder.hpp>
#include <powsybl/iidm/TapChangerAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChangerAdder : public TapChangerAdder<RatioTapChanger, RatioTapChangerAdder, RatioTapChangerStepAdder<RatioTapChangerAdder>, RatioTapChangerHolder> {

public:
    explicit RatioTapChangerAdder(RatioTapChangerHolder& parent);

    /**
     * Adder by copy of a given RatioTapChanger
     */
    explicit RatioTapChangerAdder(RatioTapChangerHolder& parent, const RatioTapChanger& ratioTapChanger);

    ~RatioTapChangerAdder() noexcept = default;

    //TapChangerAdder
    RatioTapChanger& add() override;

    RatioTapChangerStepAdder<RatioTapChangerAdder> beginStep() override;

    //RatioTapChangerAdder
    RatioTapChangerAdder& setLoadTapChangingCapabilities(bool loadTapChangingCapabilities);

    RatioTapChangerAdder& setRegulationMode(const RatioTapChanger::RegulationMode& regulationMode);

    RatioTapChangerAdder& setTargetV(double targetV);

private:
    RatioTapChangerAdder& addStep(double rho, double r, double x, double g, double b);

    Validable& getValidable();

    friend class RatioTapChangerStepAdder<RatioTapChangerAdder>;

private:
    std::vector<RatioTapChangerStep> m_steps;

    bool m_loadTapChangingCapabilities = false;

    RatioTapChanger::RegulationMode m_regulationMode = RatioTapChanger::RegulationMode::VOLTAGE;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP
