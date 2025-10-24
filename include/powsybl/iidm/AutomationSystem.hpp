/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_AUTOMATIONSYSTEM_HPP
#define POWSYBL_IIDM_AUTOMATIONSYSTEM_HPP

#include <vector>

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

class AutomationSystem : public Identifiable {
protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    AutomationSystem(Network& network, const std::string& id, const std::string& name, bool fictitious, bool enabled);

    ~AutomationSystem() noexcept override = default;

    bool isEnabled() const;

    void setEnabled(bool enabled);

private:
    std::vector<bool> m_enabled;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_AUTOMATIONSYSTEM_HPP