/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORMODEL_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORMODEL_HPP

#include <powsybl/iidm/ShuntCompensatorModelType.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensator;
class ShuntCompensatorAdder;

class ShuntCompensatorModel {
public:
    virtual ~ShuntCompensatorModel() noexcept = default;

protected:
    explicit ShuntCompensatorModel(ShuntCompensator& shuntCompensator);

protected:
    ShuntCompensator& m_shuntCompensator;

private:
    virtual double getB(unsigned long sectionCount) const = 0;

    virtual double getG(unsigned long sectionCount) const = 0;

    virtual unsigned long getMaximumSectionCount() const = 0;

    const ShuntCompensator& getShuntCompensator() const;

    ShuntCompensator& getShuntCompensator();

    virtual const ShuntCompensatorModelType& getType() const = 0;

    friend class ShuntCompensator;

    friend class ShuntCompensatorAdder;

    friend class ShuntCompensatorNonLinearModel;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORMODEL_HPP
