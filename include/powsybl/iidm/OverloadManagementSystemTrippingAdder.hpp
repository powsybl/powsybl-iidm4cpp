/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMTRIPPINGADDER_HPP
#define POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMTRIPPINGADDER_HPP

#include <memory>
#include <string>

#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>
#include <powsybl/iidm/Validable.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;
class OverloadManagementSystemAdder;

namespace overload_management_system {

class TrippingAdder : public virtual Validable, public PropertiesHolder, public std::enable_shared_from_this<TrippingAdder> {
public: //Validable
    virtual std::string getMessageHeader() const override;

public:
    ~TrippingAdder() noexcept override = default;

    OverloadManagementSystemAdder& add();

    TrippingAdder& setKey(const std::string& key);
    TrippingAdder& setName(const std::string& name);
    TrippingAdder& setCurrentLimit(double currentLimit);
    TrippingAdder& setOpenAction(bool open);

    virtual const Tripping::Type& getType() const = 0;

protected:
    TrippingAdder(OverloadManagementSystemAdder& parent);
    friend class iidm::OverloadManagementSystemAdder;

    virtual std::unique_ptr<Tripping> buildTripping(const std::string& overloadManagementSystemId) = 0;

    std::string getTrippingAttribute() const;
    stdcxx::Reference<Identifiable> checkElementId(const std::string& monitoredElementId, const std::string& attributeName);

    OverloadManagementSystemAdder& m_parent;
    std::string m_key;
    std::string m_name;
    double m_currentLimit = stdcxx::nan();
    bool m_openAction = true;
};

class SwitchTrippingAdder : public TrippingAdder {
public:
    template<typename ...Arg> 
    std::shared_ptr<SwitchTrippingAdder> static create(Arg&&...arg) {
        struct EnableMakeShared : public SwitchTrippingAdder {
            EnableMakeShared(Arg&&...arg) :SwitchTrippingAdder(std::forward<Arg>(arg)...) {}
        };
        return std::make_shared<EnableMakeShared>(std::forward<Arg>(arg)...);
    }

    ~SwitchTrippingAdder() noexcept override = default;
    const Tripping::Type& getType() const override;

    SwitchTrippingAdder& setSwitchToOperateId(const std::string& switchId);
protected:
    SwitchTrippingAdder(OverloadManagementSystemAdder& parent);
    friend class iidm::OverloadManagementSystemAdder;

    std::unique_ptr<Tripping> buildTripping(const std::string& overloadManagementSystemId) override;

    stdcxx::Reference<Identifiable> checkSwitchId(const std::string& monitoredElementId);

private:
    std::string m_switchId;
};

class BranchTrippingAdder : public TrippingAdder {
public:
    template<typename ...Arg> 
    std::shared_ptr<BranchTrippingAdder> static create(Arg&&...arg) {
        struct EnableMakeShared : public BranchTrippingAdder {
            EnableMakeShared(Arg&&...arg) :BranchTrippingAdder(std::forward<Arg>(arg)...) {}
        };
        return std::make_shared<EnableMakeShared>(std::forward<Arg>(arg)...);
    }

    ~BranchTrippingAdder() noexcept override = default;
    const Tripping::Type& getType() const override;

    BranchTrippingAdder& setBranchToOperateId(const std::string& branchId);

    BranchTrippingAdder& setSideToOperate(const TwoSides& side);
protected:
    BranchTrippingAdder(OverloadManagementSystemAdder& parent);
    friend class iidm::OverloadManagementSystemAdder;

    std::unique_ptr<Tripping> buildTripping(const std::string& overloadManagementSystemId) override;

    stdcxx::Reference<Identifiable> checkBranchId(const std::string& monitoredElementId);

private:
    std::string m_branchId;
    TwoSides m_side;
};

class ThreeWindingsTransformerTrippingAdder : public TrippingAdder {
public:
    template<typename ...Arg> 
    std::shared_ptr<ThreeWindingsTransformerTrippingAdder> static create(Arg&&...arg) {
        struct EnableMakeShared : public ThreeWindingsTransformerTrippingAdder {
            EnableMakeShared(Arg&&...arg) :ThreeWindingsTransformerTrippingAdder(std::forward<Arg>(arg)...) {}
        };
        return std::make_shared<EnableMakeShared>(std::forward<Arg>(arg)...);
    }

    ~ThreeWindingsTransformerTrippingAdder() noexcept override = default;
    const Tripping::Type& getType() const override;

    ThreeWindingsTransformerTrippingAdder& setThreeWindingsTransformerToOperateId(const std::string& transformerId);

    ThreeWindingsTransformerTrippingAdder& setSideToOperate(const ThreeSides& side);
protected:
    ThreeWindingsTransformerTrippingAdder(OverloadManagementSystemAdder& parent);
    friend class iidm::OverloadManagementSystemAdder;

    std::unique_ptr<Tripping> buildTripping(const std::string& overloadManagementSystemId) override;

    stdcxx::Reference<Identifiable> checkThreeWindingsTransformerId(const std::string& monitoredElementId);

private:
    std::string m_transformerId;
    ThreeSides m_side;
};



}  // namespace overload_management_system

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMTRIPPINGADDER_HPP

