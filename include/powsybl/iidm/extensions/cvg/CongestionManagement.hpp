/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_CONGESTIONMANAGEMENT_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_CONGESTIONMANAGEMENT_HPP

#include <vector>

#include <powsybl/iidm/AbstractMultiVariantConnectableExtension.hpp>
#include <powsybl/iidm/Battery.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

class CongestionManagement : public AbstractMultiVariantConnectableExtension {
public:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    CongestionManagement(Battery& battery, bool enabled);

    bool isEnabled() const;

    CongestionManagement& setEnabled(bool enabled);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::vector<bool> m_enabled;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_CONGESTIONMANAGEMENT_HPP
