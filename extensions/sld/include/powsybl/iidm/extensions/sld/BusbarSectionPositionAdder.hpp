/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_BUSBARSECTIONPOSITIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_BUSBARSECTIONPOSITIONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

class BusbarSectionPositionAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit BusbarSectionPositionAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    BusbarSectionPositionAdder(const BusbarSectionPositionAdder&) = default;

    /**
     * Move constructor
     */
    BusbarSectionPositionAdder(BusbarSectionPositionAdder&&) = default;

    /**
     * Destructor
     */
    ~BusbarSectionPositionAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    BusbarSectionPositionAdder& operator=(const BusbarSectionPositionAdder&) = delete;

    /**
     * Move assignment operator
     */
    BusbarSectionPositionAdder& operator=(BusbarSectionPositionAdder&&) = delete;

    /**
     * Set busbar index
     *
     * @param busbarIndex the busbar index
     *
     * @return this BusbarSectionPositionAdder object
     */
    BusbarSectionPositionAdder& withBusbarIndex(unsigned long busbarIndex);

    /**
     * Set section index
     *
     * @param sectionIndex the section index
     *
     * @return this BusbarSectionPositionAdder object
     */
    BusbarSectionPositionAdder& withSectionIndex(unsigned long sectionIndex);

protected:
    /**
     * Creates the BusbarSectionPosition extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    stdcxx::optional<unsigned long> m_busbarIndex;

    stdcxx::optional<unsigned long> m_sectionIndex;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_BUSBARSECTIONPOSITIONADDER_HPP
