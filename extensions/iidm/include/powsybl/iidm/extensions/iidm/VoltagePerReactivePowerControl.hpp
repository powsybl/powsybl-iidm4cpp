/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class StaticVarCompensator;

namespace extensions {

namespace iidm {

class VoltagePerReactivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~VoltagePerReactivePowerControl() noexcept override = default;

    /**
     * Get slope
     *
     * @return the slope
     */
    double getSlope() const;

    /**
     * Set slope value
     *
     * @param slope the slope value
     *
     * @return this VoltagePerReactivePowerControl object
     */
    VoltagePerReactivePowerControl& setSlope(double slope);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    VoltagePerReactivePowerControl(StaticVarCompensator& svc, double slope);

    double checkSlope(double slope);

    template <typename B, typename D, typename, typename... Args>
    friend std::unique_ptr<B> stdcxx::make_unique(Args&&... args);

private:
    double m_slope;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROL_HPP
