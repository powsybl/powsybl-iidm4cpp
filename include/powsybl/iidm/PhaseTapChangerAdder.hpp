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

class PhaseTapChangerAdder {
public:
    class StepAdder {
    public:
        ~StepAdder() noexcept = default;

        PhaseTapChangerAdder& endStep();

        StepAdder& setAlpha(double alpha);

        StepAdder& setB(double b);

        StepAdder& setG(double g);

        StepAdder& setR(double r);

        StepAdder& setRho(double rho);

        StepAdder& setX(double x);

    private:
        explicit StepAdder(PhaseTapChangerAdder& parent);

        friend class PhaseTapChangerAdder;

    private:
        PhaseTapChangerAdder& m_parent;

        double m_alpha = stdcxx::nan();

        double m_rho = 1.0;

        double m_r = 0.0;

        double m_x = 0.0;

        double m_g = 0.0;

        double m_b = 0.0;
    };

public:
    explicit PhaseTapChangerAdder(PhaseTapChangerHolder& parent);

    ~PhaseTapChangerAdder() noexcept = default;

    PhaseTapChanger& add();

    StepAdder beginStep();

    PhaseTapChangerAdder& setLowTapPosition(long lowTapPosition);

    PhaseTapChangerAdder& setRegulating(bool regulating);

    PhaseTapChangerAdder& setRegulationMode(const PhaseTapChanger::RegulationMode& regulationMode);

    PhaseTapChangerAdder& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal);

    PhaseTapChangerAdder& setRegulationValue(double regulationValue);

    PhaseTapChangerAdder& setTapPosition(long tapPosition);

    PhaseTapChangerAdder& setTargetDeadband(double targetDeadband);

protected:
    Network& getNetwork();

private:
    PhaseTapChangerAdder& addStep(double alpha, double rho, double r, double x, double g, double b);

    Validable& getValidable();

private:
    PhaseTapChangerHolder& m_parent;

    long m_lowTapPosition = 0;

    stdcxx::optional<long> m_tapPosition;

    std::vector<PhaseTapChangerStep> m_steps;

    PhaseTapChanger::RegulationMode m_regulationMode = PhaseTapChanger::RegulationMode::FIXED_TAP;

    double m_regulationValue = stdcxx::nan();

    bool m_regulating = false;

    stdcxx::Reference<Terminal> m_regulationTerminal;

    double m_targetDeadband = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERADDER_HPP
