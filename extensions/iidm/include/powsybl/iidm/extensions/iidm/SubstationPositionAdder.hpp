/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITIONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/SubstationPosition.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class SubstationPositionAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit SubstationPositionAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~SubstationPositionAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    SubstationPositionAdder(const SubstationPositionAdder&) = default;

    /**
     * Move constructor
     */
    SubstationPositionAdder(SubstationPositionAdder&&) = default;

    /**
     * Copy assignment operator
     */
    SubstationPositionAdder& operator=(const SubstationPositionAdder&) = delete;

    /**
     * Move assignment operator
     */
    SubstationPositionAdder& operator=(SubstationPositionAdder&&) = delete;

    /**
     * Set coordinates
     *
     * @param coords the coordinates
     *
     * @return this SubstationPositionAdder object
     */
    SubstationPositionAdder& withCoordinates(const Coordinate& coords);

protected:
    /**
     * Creates the SubstationPosition extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    Coordinate m_coordinates;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITIONADDER_HPP
