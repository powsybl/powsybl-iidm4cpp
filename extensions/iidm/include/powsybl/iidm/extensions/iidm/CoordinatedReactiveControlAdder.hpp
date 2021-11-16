/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class CoordinatedReactiveControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit CoordinatedReactiveControlAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~CoordinatedReactiveControlAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    CoordinatedReactiveControlAdder(const CoordinatedReactiveControlAdder&) = default;

    /**
     * Move constructor
     */
    CoordinatedReactiveControlAdder(CoordinatedReactiveControlAdder&&) = default;

    /**
     * Copy assignment operator
     */
    CoordinatedReactiveControlAdder& operator=(const CoordinatedReactiveControlAdder&) = delete;

    /**
     * Move assignment operator
     */
    CoordinatedReactiveControlAdder& operator=(CoordinatedReactiveControlAdder&&) = delete;

    /**
     * Set the percent of the coordinated reactive control that comes
     * from the generator to which the extension is linked
     *
     * @param qPercent percent of coordinated reactive control that comes from the generator to which the extension is linked
     *
     * @return this CoordinatedReactiveControlAdder object
     */
    CoordinatedReactiveControlAdder& withQPercent(double qPercent);

protected:
    /**
     * Creates the CoordinatedReactiveControl extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    double m_qPercent = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROLADDER_HPP
