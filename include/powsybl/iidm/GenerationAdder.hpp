/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GENERATIONADDER_HPP
#define POWSYBL_IIDM_GENERATIONADDER_HPP

#include <functional>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class DanglingLineAdder;

namespace dangling_line_views {

class GenerationAdder {
public:
    explicit GenerationAdder(DanglingLineAdder& adder);

    DanglingLineAdder& add();

    GenerationAdder& setMaxP(double maxP);

    GenerationAdder& setMinP(double minP);

    GenerationAdder& setTargetP(double targetP);

    GenerationAdder& setTargetQ(double targetQ);

    GenerationAdder& setTargetV(double targetV);

    GenerationAdder& setVoltageRegulationOn(bool voltageRegulationOn);

private:
    friend class iidm::DanglingLineAdder;

private:
    std::reference_wrapper<DanglingLineAdder> m_danglingLineAdder;

    double m_minP = stdcxx::nan();

    double m_maxP = stdcxx::nan();

    double m_targetP = stdcxx::nan();

    double m_targetQ = stdcxx::nan();

    bool m_voltageRegulationOn = false;

    double m_targetV = stdcxx::nan();
};

}  // namespace dangling_line_views

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATIONADDER_HPP
