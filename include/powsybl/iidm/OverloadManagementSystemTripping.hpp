/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMTRIPPING_HPP
#define POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMTRIPPING_HPP

#include <string>

#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/iidm/TwoSides.hpp>
#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

namespace overload_management_system {

class Tripping : public Validable {
public:
    enum class Type : unsigned char {
        BRANCH_TRIPPING,
        THREE_WINDINGS_TRANSFORMER_TRIPPING,
        SWITCH_TRIPPING
    };

    virtual const Type& getType() const = 0;

    Tripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction);

    Tripping(const Tripping&) = delete;
    Tripping(Tripping&&) = default;
    Tripping& operator=(const Tripping&) = delete;
    Tripping& operator=(Tripping&&) = default;
    ~Tripping() noexcept override = default;

    const std::string& getKey() const;

    const std::string& getNameOrKey() const;

    Tripping& setName(const std::string& name);

    double getCurrentLimit() const;

    Tripping& setCurrentLimit(double currentLimit);

    bool isOpenAction() const;

    Tripping& setOpenAction(bool open);

public:  // Validable
    virtual std::string getMessageHeader() const override;

protected:
    std::string getTrippingAttribute() const;

private:
    std::string m_overloadManagementSystemId;
    std::string m_key;
    std::string m_name;
    double m_currentLimit;
    bool m_openAction;
};

class SwitchTripping : public Tripping {
public:
    SwitchTripping(const SwitchTripping&) = delete;
    SwitchTripping(SwitchTripping&&) = default;
    SwitchTripping& operator=(const SwitchTripping&) = delete;
    SwitchTripping& operator=(SwitchTripping&&) = default;
    ~SwitchTripping() noexcept override = default;

public:
    const Type& getType() const override;

    const std::string& getSwitchToOperateId() const;

    SwitchTripping& setSwitchToOperateId(const std::string& switchToOperateId);

private:
    SwitchTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& switchToOperateId);
    friend class SwitchTrippingAdder;

    std::string m_switchToOperateId;
};

class BranchTripping : public Tripping {
public:

    BranchTripping(const Tripping&) = delete;
    BranchTripping(BranchTripping&&) = default;
    BranchTripping& operator=(const BranchTripping&) = delete;
    BranchTripping& operator=(BranchTripping&&) = default;
    ~BranchTripping() noexcept override = default;

public:
    const Type& getType() const override;

    const std::string& getBranchToOperateId() const;

    BranchTripping& setBranchToOperateId(const std::string& branchToOperateId);

    const TwoSides& getSideToOperate() const;

    BranchTripping& setSideToOperate(const TwoSides& side);

private:
    BranchTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& branchToOperateId, const TwoSides& side);
    friend class BranchTrippingAdder;

    std::string m_branchToOperateId;
    TwoSides m_side;
};

class ThreeWindingsTransformerTripping : public Tripping {
public:

    ThreeWindingsTransformerTripping(const ThreeWindingsTransformerTripping&) = delete;
    ThreeWindingsTransformerTripping(ThreeWindingsTransformerTripping&&) = default;
    ThreeWindingsTransformerTripping& operator=(const ThreeWindingsTransformerTripping&) = delete;
    ThreeWindingsTransformerTripping& operator=(ThreeWindingsTransformerTripping&&) = default;
    ~ThreeWindingsTransformerTripping() noexcept override = default;

public:
    const Type& getType() const override;

    const std::string& getThreeWindingsTransformerToOperateId() const;

    ThreeWindingsTransformerTripping& setThreeWindingsTransformerToOperateId(const std::string& threeWindingsTransformerId);

    const ThreeSides& getSideToOperate() const;

    ThreeWindingsTransformerTripping& setSideToOperate(const ThreeSides& side);

private:
    ThreeWindingsTransformerTripping(const std::string& overloadManagementSystemId, const std::string& key, const std::string name,
            double currentLimit, bool openAction, const std::string& threeWindingsTransformerId, const ThreeSides& side);
    friend class ThreeWindingsTransformerTrippingAdder;

    std::string m_transformerToOperateId;
    ThreeSides m_side;
};

}  // namespace overload_management_system

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEM_HPP
