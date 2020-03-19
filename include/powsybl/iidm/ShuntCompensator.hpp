/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class IIDM_DECLSPEC ShuntCompensator : public Injection {
public:
    ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name,
                     double bPerSection, unsigned long maximumSectionCount, unsigned long currentSectionCount);

    ~ShuntCompensator() noexcept override = default;

    double getbPerSection() const;

    double getCurrentB() const;

    unsigned long getCurrentSectionCount() const;

    double getMaximumB() const;

    unsigned long getMaximumSectionCount() const;

    ShuntCompensator& setbPerSection(double bPerSection);

    ShuntCompensator& setCurrentSectionCount(unsigned long currentSectionCount);

    ShuntCompensator& setMaximumSectionCount(unsigned long maximumSectionCount);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    double m_bPerSection;

    unsigned long m_maximumSectionCount;

    std::vector<unsigned long> m_currentSectionCount;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP
