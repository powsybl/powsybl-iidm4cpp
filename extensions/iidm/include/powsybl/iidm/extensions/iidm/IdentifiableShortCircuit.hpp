/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUIT_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace iidm {

class IdentifiableShortCircuit : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    /**
     * Get maximum allowable peak short-circuit current [A]
     */
    double getIpMax() const;

    /**
     * Get minimum allowable peak short-circuit current [A]
     */
    double getIpMin() const;

    /**
     * Set maximum allowable peak short-circuit current [A]
     */
    IdentifiableShortCircuit& setIpMax(double ipMax);

    /**
     * Set minimum allowable peak short-circuit current [A]
     */
    IdentifiableShortCircuit& setIpMin(double ipMin);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    IdentifiableShortCircuit(Identifiable& identifiable, double ipMin, double ipMax);

    friend class IdentifiableShortCircuitAdder;

private:
    /**
     * Minimum allowable peak short-circuit current
     */
    double m_ipMin;

    /**
     * Maximum allowable peak short-circuit current
     */
    double m_ipMax;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUIT_HPP
