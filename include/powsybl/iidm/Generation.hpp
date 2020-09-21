/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GENERATION_HPP
#define POWSYBL_IIDM_GENERATION_HPP

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

namespace dangling_line_views {

class Generation : public Validable, public ReactiveLimitsHolder {
public:  // Validable
    std::string getMessageHeader() const override;

public:
    Generation(double minP, double maxP, double targetP, double targetQ, bool voltageRegulationOn, double targetV);

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
    Generation& setDanglingLine(const VariantManagerHolder& network, const DanglingLine& danglingLine);

    friend class iidm::DanglingLine;

private:
    stdcxx::Reference<DanglingLine> m_danglingLine;

    std::unique_ptr<ReactiveLimitsHolder> m_reactiveLimits;

    double m_minP;

    double m_maxP;

    double m_initialTargetP;

    double m_initialTargetQ;

    bool m_initialVoltageRegulationOn;

    double m_initialTargetV;

    // attributes depending on the variant

    std::vector<double> m_targetP;

    std::vector<double> m_targetQ;

    std::vector<bool> m_voltageRegulationOn;

    std::vector<double> m_targetV;
};

}  // namespace dangling_line_views

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATION_HPP
