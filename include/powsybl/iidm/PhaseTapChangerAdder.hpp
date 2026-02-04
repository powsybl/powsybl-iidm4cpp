/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERADDER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERADDER_HPP

#include <vector>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>
#include <powsybl/iidm/TapChangerAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Network;
class PhaseTapChangerStep;
class Terminal;
class TwoWindingsTransformer;
class Validable;

class PhaseTapChangerAdder : public TapChangerAdder<PhaseTapChanger, PhaseTapChangerAdder, PhaseTapChangerStepAdder<PhaseTapChangerAdder>, PhaseTapChangerHolder> {

public:
    explicit PhaseTapChangerAdder(PhaseTapChangerHolder& parent);

    ~PhaseTapChangerAdder() noexcept = default;

    //TapChanger
    PhaseTapChanger& add() override;

    PhaseTapChangerStepAdder<PhaseTapChangerAdder> beginStep() override;

    //PhaseTapChangerAdder
    PhaseTapChangerAdder& setRegulationMode(const PhaseTapChanger::RegulationMode& regulationMode);

private:
    PhaseTapChangerAdder& addStep(double alpha, double rho, double r, double x, double g, double b);

    Validable& getValidable();

    friend class PhaseTapChangerStepAdder<PhaseTapChangerAdder>;

private:
    std::vector<PhaseTapChangerStep> m_steps;

    PhaseTapChanger::RegulationMode m_regulationMode = PhaseTapChanger::RegulationMode::FIXED_TAP;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERADDER_HPP
