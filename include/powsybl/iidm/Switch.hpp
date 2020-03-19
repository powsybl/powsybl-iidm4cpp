/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCH_HPP
#define POWSYBL_IIDM_SWITCH_HPP

#include <vector>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/SwitchKind.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

class IIDM_DECLSPEC Switch : public Identifiable, public MultiVariantObject {
public:
    Switch(VoltageLevel& voltageLevel, const std::string& id, const std::string& name, SwitchKind kind, bool open,
           bool retained, bool fictitious);

    ~Switch() noexcept override = default;

    SwitchKind getKind() const;

    VoltageLevel& getVoltageLevel() const;

    bool isFictitious() const;

    bool isOpen() const;

    bool isRetained() const;

    Switch& setFictitious(bool fictitious);

    Switch& setOpen(bool open);

    Switch& setRetained(bool retained);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<VoltageLevel> m_voltageLevel;

    SwitchKind m_kind;

    bool m_fictitious;

    std::vector<bool> m_open;

    std::vector<bool> m_retained;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SWITCH_HPP
