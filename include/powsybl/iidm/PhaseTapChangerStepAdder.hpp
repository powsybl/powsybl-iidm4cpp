/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP

#include <powsybl/iidm/TapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

template <typename Parent>
class PhaseTapChangerStepAdder : public TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent> {
public:
    ~PhaseTapChangerStepAdder() noexcept = default;

    PhaseTapChangerStepAdder<Parent>& setRho(double rho) override;

    PhaseTapChangerStepAdder<Parent>& setR(double r) override;

    PhaseTapChangerStepAdder<Parent>& setX(double x) override;

    PhaseTapChangerStepAdder<Parent>& setG(double g) override;

    PhaseTapChangerStepAdder<Parent>& setB(double b) override;

    Parent& endStep() override;

    PhaseTapChangerStepAdder<Parent>& setAlpha(double alpha);

private:
    explicit PhaseTapChangerStepAdder(Parent &parent);
    friend class PhaseTapChangerAdder;
    friend class PhaseTapChangerStepsReplacer;

    double m_alpha = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/PhaseTapChangerStepAdder.hxx>

#endif  // POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP
