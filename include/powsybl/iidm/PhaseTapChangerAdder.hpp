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
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

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

        double m_alpha;

        double m_rho;

        double m_r;

        double m_x;

        double m_g;

        double m_b;
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

    long m_lowTapPosition;

    stdcxx::optional<long> m_tapPosition;

    std::vector<PhaseTapChangerStep> m_steps;

    PhaseTapChanger::RegulationMode m_regulationMode;

    double m_regulationValue;

    bool m_regulating;

    stdcxx::Reference<Terminal> m_regulationTerminal;

    double m_targetDeadband;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERADDER_HPP
