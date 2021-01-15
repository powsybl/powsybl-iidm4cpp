/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEGENERATIONADDER_HPP
#define POWSYBL_IIDM_DANGLINGLINEGENERATIONADDER_HPP

#include <functional>
#include <memory>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;
class DanglingLineAdder;

namespace dangling_line {

class Generation;

class GenerationAdder {
public:
    explicit GenerationAdder(DanglingLineAdder& parent);

    DanglingLineAdder& add();

    GenerationAdder& setMaxP(double maxP);

    GenerationAdder& setMinP(double minP);

    GenerationAdder& setTargetP(double targetP);

    GenerationAdder& setTargetQ(double targetQ);

    GenerationAdder& setTargetV(double targetV);

    GenerationAdder& setVoltageRegulationOn(bool voltageRegulationOn);

private:
    std::unique_ptr<Generation> build() const;

    friend class iidm::DanglingLineAdder;

private:
    DanglingLineAdder& m_parent;

    double m_minP = stdcxx::nan();

    double m_maxP = stdcxx::nan();

    double m_targetP = stdcxx::nan();

    double m_targetQ = stdcxx::nan();

    double m_targetV = stdcxx::nan();

    bool m_voltageRegulationOn = false;
};

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEGENERATIONADDER_HPP
