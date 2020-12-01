/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEGENERATION_HPP
#define POWSYBL_IIDM_DANGLINGLINEGENERATION_HPP

#include <memory>
#include <set>
#include <vector>

#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;
class VariantManagerHolder;

namespace dangling_line {

class DanglingLineGeneration : public Validable, public ReactiveLimitsHolder {
public:  // Validable
    std::string getMessageHeader() const override;

public:
    DanglingLineGeneration(DanglingLine& danglingLine, double minP, double maxP, double targetP, double targetQ, bool voltageRegulationOn, double targetV);

    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex);

    void extendVariantArraySize(unsigned long number, unsigned long sourceIndex);

    double getMaxP() const;

    double getMinP() const;

    double getTargetP() const;

    double getTargetQ() const;

    double getTargetV() const;

    bool isVoltageRegulationOn() const;

    void reduceVariantArraySize(unsigned long number);

    DanglingLineGeneration& setMaxP(double maxP);

    DanglingLineGeneration& setMinP(double minP);

    DanglingLineGeneration& setTargetP(double targetP);

    DanglingLineGeneration& setTargetQ(double targetQ);

    DanglingLineGeneration& setTargetV(double targetV);

    DanglingLineGeneration& setVoltageRegulationOn(bool voltageRegulationOn);

private:
    friend class iidm::DanglingLine;

private:
    std::reference_wrapper<DanglingLine> m_danglingLine;

    double m_minP;

    double m_maxP;

    std::vector<double> m_targetP;

    std::vector<double> m_targetQ;

    std::vector<double> m_targetV;

    std::vector<bool> m_voltageRegulationOn;
};

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEGENERATION_HPP
