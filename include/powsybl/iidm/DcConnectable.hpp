/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCCONNECTABLE_HPP
#define POWSYBL_IIDM_DCCONNECTABLE_HPP

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/DcTerminal.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class Network;

class DcConnectable : public virtual Identifiable {

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    DcConnectable() = default;
public:
    ~DcConnectable() noexcept override = default;

    DcTerminal& addDcTerminal(std::unique_ptr<DcTerminal>&& dcTerminal);

    std::vector<std::reference_wrapper<DcTerminal> > getDcTerminals() const;
    std::vector<std::reference_wrapper<DcTerminal> > getDcTerminals(const stdcxx::optional<TwoSides>& side) const;

    virtual void remove();

protected:
    const DcTerminal& getDcTerminal(unsigned long index) const;
    DcTerminal& getDcTerminal(unsigned long index);
    unsigned long getDcTerminalsCount() const;

private:
    std::vector<std::unique_ptr<DcTerminal>> m_dcTerminals;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCCONNECTABLE_HPP
