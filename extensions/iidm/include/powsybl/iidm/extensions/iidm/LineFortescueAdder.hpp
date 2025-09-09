/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class LineFortescueAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit LineFortescueAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~LineFortescueAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    LineFortescueAdder(const LineFortescueAdder&) = default;

    /**
     * Move constructor
     */
    LineFortescueAdder(LineFortescueAdder&&) = default;

    /**
     * Copy assignment operator
     */
    LineFortescueAdder& operator=(const LineFortescueAdder&) = delete;

    /**
     * Move assignment operator
     */
    LineFortescueAdder& operator=(LineFortescueAdder&&) = delete;

    LineFortescueAdder& withRz(double rz);
    LineFortescueAdder& withXz(double xz);

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

    double m_rz = stdcxx::nan();
    double m_xz = stdcxx::nan();

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUEADDER_HPP
