/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP

#include <vector>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Network;
class RatioTapChanger;
class RatioTapChangerHolder;
class RatioTapChangerStep;
class Terminal;
class Validable;

class RatioTapChangerAdder {
public:
    class StepAdder {
    public:
        ~StepAdder() noexcept = default;

        RatioTapChangerAdder& endStep();

        StepAdder& setB(double b);

        StepAdder& setG(double g);

        StepAdder& setR(double r);

        StepAdder& setRho(double rho);

        StepAdder& setX(double x);

    private:
        explicit StepAdder(RatioTapChangerAdder& parent);

        friend class RatioTapChangerAdder;

    private:
        RatioTapChangerAdder& m_parent;

        double m_rho;

        double m_r;

        double m_x;

        double m_g;

        double m_b;
    };

public:
    explicit RatioTapChangerAdder(RatioTapChangerHolder& parent);

    ~RatioTapChangerAdder() noexcept = default;

    RatioTapChanger& add();

    StepAdder beginStep();

    RatioTapChangerAdder& setLoadTapChangingCapabilities(bool loadTapChangingCapabilities);

    RatioTapChangerAdder& setLowTapPosition(long lowTapPosition);

    RatioTapChangerAdder& setRegulating(bool regulating);

    RatioTapChangerAdder& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal);

    RatioTapChangerAdder& setTapPosition(long tapPosition);

    RatioTapChangerAdder& setTargetDeadband(double targetDeadband);

    RatioTapChangerAdder& setTargetV(double targetV);

protected:
    Network& getNetwork();

private:
    RatioTapChangerAdder& addStep(double rho, double r, double x, double g, double b);

    Validable& getValidable();

private:
    RatioTapChangerHolder& m_parent;

    long m_lowTapPosition;

    stdcxx::optional<long> m_tapPosition;

    std::vector<RatioTapChangerStep> m_steps;

    bool m_loadTapChangingCapabilities;

    bool m_regulating;

    double m_targetV;

    stdcxx::Reference<Terminal> m_regulationTerminal;

    double m_targetDeadband;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERADDER_HPP
