/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEGENERATION_HPP
#define POWSYBL_IIDM_DANGLINGLINEGENERATION_HPP

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

class Generation : public Validable, public ReactiveLimitsHolder {
public:  // Validable
    std::string getMessageHeader() const override;

public:
    Generation(VariantManagerHolder& network, double minP, double maxP, double targetP, double targetQ, double targetV, bool voltageRegulationOn);

    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex);

    void extendVariantArraySize(unsigned long number, unsigned long sourceIndex);

    double getMaxP() const;

    double getMinP() const;

    double getTargetP() const;

    double getTargetQ() const;

    double getTargetV() const;

    bool isVoltageRegulationOn() const;

    void reduceVariantArraySize(unsigned long number);

    Generation& setMaxP(double maxP);

    Generation& setMinP(double minP);

    Generation& setTargetP(double targetP);

    Generation& setTargetQ(double targetQ);

    Generation& setTargetV(double targetV);

    Generation& setVoltageRegulationOn(bool voltageRegulationOn);

private:
    Generation& attach(DanglingLine& danglineLine);

    friend DanglingLine;

private:
    stdcxx::Reference<DanglingLine> m_danglingLine;

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
