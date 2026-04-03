/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP

#include <powsybl/iidm/MultiVariantObject.hpp>

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

#include <powsybl/stdcxx/optional.hpp>

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlZone : public MultiVariantObject {
public:

    ControlZone(const std::string& name, const PilotPoint& pilotPoint, const std::vector<ControlUnit>& controlUnits);

    ControlZone() = delete;
    ControlZone(const ControlZone& coord) = default;
    ControlZone(ControlZone&&) noexcept = default;
    ~ControlZone() noexcept = default;
    ControlZone& operator=(const ControlZone&) = default;
    ControlZone& operator=(ControlZone&&) noexcept = default;

    const std::string& getName() const;
    const PilotPoint& getPilotPoint() const;
    PilotPoint& getPilotPoint();
    const std::vector<ControlUnit>& getControlUnits() const;
    std::vector<ControlUnit>& getControlUnits();
    stdcxx::optional<ControlUnit> getControlUnit(const std::string id) const;
    stdcxx::Reference<ControlUnit> getControlUnit(const std::string id);

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    void setVariantManagerHolder(const VariantManagerHolder& variantManagerHolder);
    friend class SecondaryVoltageControl;

private:

    std::string m_name;

    PilotPoint m_pilotPoint;

    std::vector<ControlUnit> m_controlUnits;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP
