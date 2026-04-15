/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERADDER_HPP
#define POWSYBL_IIDM_TAPCHANGERADDER_HPP

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
class TapChangerAdder {
public:
    virtual ~TapChangerAdder() noexcept = default;

public:

    virtual TCAdder& setLowTapPosition(long lowTapPosition);

    virtual TCAdder& setTapPosition(long tapPosition);

    virtual TCAdder& setSolvedTapPosition(long solvedTapPosition);

    virtual TCAdder& setRegulating(bool regulating);

    virtual TCAdder& setLoadTapChangingCapabilities(bool loadTapChangingCapabilities);

    virtual TCAdder& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal);

    virtual TCAdder& setTargetDeadband(double targetDeadband);

    virtual TCAdder& setRegulationValue(double regulationValue);

    virtual TC& add() = 0;

    virtual TCStepAdder beginStep() = 0;

protected:
    Network& getNetwork();

protected:
    TapChangerAdder(TCHolder& parent, bool loadTapChangingCapabilities = false);

    TCHolder& m_parent;

    long m_lowTapPosition = 0;

    stdcxx::optional<long> m_tapPosition;

    stdcxx::optional<long> m_solvedTapPosition;

    bool m_regulating = false;

    bool m_loadTapChangingCapabilities = false;

    stdcxx::Reference<Terminal> m_regulationTerminal;

    double m_targetDeadband = stdcxx::nan();

    double m_regulationValue = stdcxx::nan();

};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TapChangerAdder.hxx>

#endif  // POWSYBL_IIDM_TAPCHANGERADDER_HPP
