/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKEXTENSION_HPP
#define POWSYBL_IIDM_NETWORKEXTENSION_HPP

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>

namespace powsybl {

namespace iidm {

class Network;

class NetworkExtension : public AbstractMultiVariantIdentifiableExtension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    NetworkExtension(Network& network, bool value);

    ~NetworkExtension() noexcept override = default;

    bool getValue() const;

    NetworkExtension& setValue(bool value);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::vector<bool> m_value;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKEXTENSION_HPP
