/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITIONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class Coordinate;

class LinePositionAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit LinePositionAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~LinePositionAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    LinePositionAdder(const LinePositionAdder&) = default;

    /**
     * Move constructor
     */
    LinePositionAdder(LinePositionAdder&&) = default;

    /**
     * Copy assignment operator
     */
    LinePositionAdder& operator=(const LinePositionAdder&) = delete;

    /**
     * Move assignment operator
     */
    LinePositionAdder& operator=(LinePositionAdder&&) = delete;

    /**
     * Set coordinates
     *
     * @param coords the list of coordinates as a vector
     *
     * @return this LinePositionAdder object
     */
    LinePositionAdder& withCoordinates(std::vector<Coordinate> coords);

protected:
    /**
     * Creates the LinePosition extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    std::vector<Coordinate> m_coordinates;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITIONADDER_HPP
