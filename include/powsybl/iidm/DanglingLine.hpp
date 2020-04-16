/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINE_HPP
#define POWSYBL_IIDM_DANGLINGLINE_HPP

#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine : public Injection {
public:
    DanglingLine(VariantManagerHolder& network, const std::string& id, const std::string& name,
                 double p0, double q0, double r, double x, double g, double b, const std::string& ucteXnodeCode);

    ~DanglingLine() noexcept override = default;

    double getB() const;

    stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits();

    double getG() const;

    double getP0() const;

    double getQ0() const;

    double getR() const;

    const std::string& getUcteXnodeCode() const;

    double getX() const;

    CurrentLimitsAdder<std::nullptr_t, DanglingLine> newCurrentLimits();

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
    void setCurrentLimits(std::nullptr_t side, std::unique_ptr<CurrentLimits> limits);

private:
    friend class CurrentLimitsAdder<std::nullptr_t, DanglingLine>;

private:
    double m_b;

    double m_g;

    double m_r;

    double m_x;

    std::vector<double> m_p0;

    std::vector<double> m_q0;

    std::string m_ucteXnodeCode;

    std::unique_ptr<CurrentLimits> m_limits;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINE_HPP
