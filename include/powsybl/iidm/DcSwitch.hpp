/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCSWITCH_HPP
#define POWSYBL_IIDM_DCSWITCH_HPP

#include <powsybl/iidm/DcSwitchKind.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class DcNode;
class Network;

class DcSwitch : public Identifiable {
public:
    ~DcSwitch() noexcept override = default;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    DcSwitchKind getKind() const;

    const DcNode& getDcNode1() const;
    DcNode& getDcNode1();

    const DcNode& getDcNode2() const;
    DcNode& getDcNode2();

    bool isOpen() const;
    DcSwitch& setOpen(bool open);
    
    void remove();

protected:
    DcSwitch(const std::string& id, const std::string& name, bool fictitious,
            DcNode& node1, DcNode& node2, const DcSwitchKind& kind, bool open);
    friend class DcSwitchAdder;

private:
    stdcxx::Reference<DcNode> m_dcNode1;
    stdcxx::Reference<DcNode> m_dcNode2;

    DcSwitchKind m_kind;

    std::vector<bool> m_open;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCSWITCH_HPP
