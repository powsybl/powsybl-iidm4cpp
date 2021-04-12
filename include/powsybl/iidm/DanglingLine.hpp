/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINE_HPP
#define POWSYBL_IIDM_DANGLINGLINE_HPP

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/DanglingLineGeneration.hpp>
#include <powsybl/iidm/FlowsLimitsHolder.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/OperationalLimitsHolder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;

class DanglingLine : public Injection, public FlowsLimitsHolder {
public:
    using Generation = dangling_line::Generation;

public:
    DanglingLine(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
                 double p0, double q0, double r, double x, double g, double b, const std::string& ucteXnodeCode,
                 std::unique_ptr<Generation>&& generation);

    ~DanglingLine() noexcept override = default;

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const override;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits() override;

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const override;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits() override;

    double getB() const;

    const Boundary& getBoundary() const;

    Boundary& getBoundary();

    stdcxx::CReference<CurrentLimits> getCurrentLimits() const override;

    stdcxx::Reference<CurrentLimits> getCurrentLimits() override;

    double getG() const;

    stdcxx::CReference<Generation> getGeneration() const;

    stdcxx::Reference<Generation> getGeneration();

    stdcxx::const_range<OperationalLimits> getOperationalLimits() const override;

    stdcxx::range<OperationalLimits> getOperationalLimits() override;

    double getP0() const;

    double getQ0() const;

    double getR() const;

    const std::string& getUcteXnodeCode() const;

    double getX() const;

    ActivePowerLimitsAdder newActivePowerLimits() override;

    ApparentPowerLimitsAdder newApparentPowerLimits() override;

    CurrentLimitsAdder newCurrentLimits() override;

    DanglingLine& setB(double b);

    DanglingLine& setG(double g);

    DanglingLine& setP0(double p0);

    DanglingLine& setQ0(double q0);

    DanglingLine& setR(double r);

    DanglingLine& setX(double x);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    double m_b;

    double m_g;

    double m_r;

    double m_x;

    std::vector<double> m_p0;

    std::vector<double> m_q0;

    std::string m_ucteXnodeCode;

    std::unique_ptr<Generation> m_generation;

    OperationalLimitsHolder m_operationalLimitsHolder;

    std::unique_ptr<Boundary> m_boundary;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINE_HPP
