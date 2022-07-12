/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUITADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUITADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class IdentifiableShortCircuitAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit IdentifiableShortCircuitAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    IdentifiableShortCircuitAdder(const IdentifiableShortCircuitAdder&) = default;

    /**
     * Move constructor
     */
    IdentifiableShortCircuitAdder(IdentifiableShortCircuitAdder&&) = default;

    /**
     * Destructor
     */
    ~IdentifiableShortCircuitAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    IdentifiableShortCircuitAdder& operator=(const IdentifiableShortCircuitAdder&) = delete;

    /**
     * Move assignment operator
     */
    IdentifiableShortCircuitAdder& operator=(IdentifiableShortCircuitAdder&&) = delete;

    IdentifiableShortCircuitAdder& withIpMax(double ipMax);

    IdentifiableShortCircuitAdder& withIpMin(double ipMin);

protected:
    /**
     * Creates the IdentifiableShortCircuit extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    double m_ipMin = stdcxx::nan();

    double m_ipMax = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_IDENTIFIABLESHORTCIRCUITADDER_HPP
