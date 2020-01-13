/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORSTARTUP_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORSTARTUP_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace cvg {

class GeneratorStartup : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    GeneratorStartup(Generator& generator, double predefinedActivePowerSetpoint, double marginalCost, double plannedOutageRate, double forcedOutageRate);

    double getForcedOutageRate() const;

    double getMarginalCost() const;

    double getPlannedOutageRate() const;

    double getPredefinedActivePowerSetpoint() const;

    GeneratorStartup& setForcedOutageRate(double forcedOutageRate);

    GeneratorStartup& setMarginalCost(double marginalCost);

    GeneratorStartup& setPredefinedActivePowerSetpoint(double predefinedActivePowerSetpoint);

    GeneratorStartup& setPlannedOutageRate(double plannedOutageRate);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    double m_predefinedActivePowerSetpoint;

    double m_marginalCost;

    double m_plannedOutageRate;

    double m_forcedOutageRate;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORSTARTUP_HPP

