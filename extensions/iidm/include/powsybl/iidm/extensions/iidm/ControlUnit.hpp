/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP

#include <powsybl/iidm/MultiVariantObject.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

class VariantManagerHolder;

namespace extensions {

namespace iidm {

class ControlUnit : public MultiVariantObject {
public:
    ControlUnit(const std::string& id, bool participate = true);

    ControlUnit() = delete;
    ControlUnit(const ControlUnit& coord) = default;
    ControlUnit(ControlUnit&&) noexcept = default;
    ~ControlUnit() noexcept = default;
    ControlUnit& operator=(const ControlUnit&) = default;
    ControlUnit& operator=(ControlUnit&&) noexcept = default;

    const std::string& getId() const;

    bool isParticipate() const;
    void setParticipate(bool participate);

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    void setVariantManagerHolder(const VariantManagerHolder& variantManagerHolder);
    friend class ControlZone;

private:
    unsigned long getVariantIndex() const;

private:
    std::string m_id;

    std::vector<bool> m_participate;

    stdcxx::Reference<VariantManagerHolder> m_variantManagerHolder;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNIT_HPP
