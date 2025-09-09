/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICALADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICALADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/LoadConnectionType.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class LoadAsymmetricalAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit LoadAsymmetricalAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~LoadAsymmetricalAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    LoadAsymmetricalAdder(const LoadAsymmetricalAdder&) = default;

    /**
     * Move constructor
     */
    LoadAsymmetricalAdder(LoadAsymmetricalAdder&&) = default;

    /**
     * Copy assignment operator
     */
    LoadAsymmetricalAdder& operator=(const LoadAsymmetricalAdder&) = delete;

    /**
     * Move assignment operator
     */
    LoadAsymmetricalAdder& operator=(LoadAsymmetricalAdder&&) = delete;

    LoadAsymmetricalAdder& withConnectionType(LoadConnectionType connectionType);
    LoadAsymmetricalAdder& withDeltaPa(double deltaPa);
    LoadAsymmetricalAdder& withDeltaQa(double deltaQa);
    LoadAsymmetricalAdder& withDeltaPb(double deltaPb);
    LoadAsymmetricalAdder& withDeltaQb(double deltaQb);
    LoadAsymmetricalAdder& withDeltaPc(double deltaPc);
    LoadAsymmetricalAdder& withDeltaQc(double deltaQc);

protected:
    /**
     * Creates the LineFortescue extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:

    LoadConnectionType m_connectionType = LoadConnectionType::Y;
    double m_deltaPa = 0.0;
    double m_deltaQa = 0.0;
    double m_deltaPb = 0.0;
    double m_deltaQb = 0.0;
    double m_deltaPc = 0.0;
    double m_deltaQc = 0.0;


};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LOADASYMMETRICALADDER_HPP
