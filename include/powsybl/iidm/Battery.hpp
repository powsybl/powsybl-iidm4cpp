/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BATTERY_HPP
#define POWSYBL_IIDM_BATTERY_HPP

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class Battery : public Injection, public ReactiveLimitsHolder {
public:  // Identifiable
    const IdentifiableType& getType() const override;

public:
    Battery(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
        double targetP, double targetQ, double minP, double maxP);

    ~Battery() noexcept override = default;

    double getMaxP() const;

    double getMinP() const;

    double getTargetP() const;

    double getTargetQ() const;

    Battery& setMaxP(double maxP);

    Battery& setMinP(double minP);

    Battery& setTargetP(double targetP);

    Battery& setTargetQ(double targetQ);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    std::vector<double> m_targetP;

    std::vector<double> m_targetQ;

    double m_minP;

    double m_maxP;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BATTERY_HPP
