/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LOADDETAIL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LOADDETAIL_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Load;

namespace extensions {

namespace iidm {

class LoadDetail : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    /**
     * Constructor
     */
    LoadDetail(Load& load, double fixedActivePower, double fixedReactivePower, double variableActivePower, double variableReactivePower);

    /**
     * Destructor
     */
    ~LoadDetail() noexcept override = default;

    /**
     * Copy constructor
     */
    LoadDetail(const LoadDetail&) = default;

    /**
     * Move constructor
     */
    LoadDetail(LoadDetail&&) = default;

    /**
     * Copy assignment operator
     */
    LoadDetail& operator=(const LoadDetail&) = default;

    /**
     * Move assignment operator
     */
    LoadDetail& operator=(LoadDetail&&) = default;

    double getFixedActivePower() const;

    double getFixedReactivePower() const;

    double getVariableActivePower() const;

    double getVariableReactivePower() const;

    LoadDetail& setFixedActivePower(double fixedActivePower);

    LoadDetail& setFixedReactivePower(double fixedReactivePower);

    LoadDetail& setVariableActivePower(double variableActivePower);

    LoadDetail& setVariableReactivePower(double variableReactivePower);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkPower(double value, const std::string& message);

private:
    double m_fixedActivePower;

    double m_fixedReactivePower;

    double m_variableActivePower;

    double m_variableReactivePower;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LOADDETAIL_HPP
