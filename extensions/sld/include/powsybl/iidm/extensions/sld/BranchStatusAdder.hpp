/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUSADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUSADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/sld/BranchStatus.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

class BranchStatusAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit BranchStatusAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~BranchStatusAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    BranchStatusAdder(const BranchStatusAdder&) = default;

    /**
     * Move constructor
     */
    BranchStatusAdder(BranchStatusAdder&&) = default;

    /**
     * Copy assignment operator
     */
    BranchStatusAdder& operator=(const BranchStatusAdder&) = delete;

    /**
     * Move assignment operator
     */
    BranchStatusAdder& operator=(BranchStatusAdder&&) = delete;

    /**
     * Set branch status
     *
     * @param status the branch status
     *
     * @return this BranchStatusAdder object
     */
    BranchStatusAdder& withStatus(const BranchStatus::Status& status);

protected:
    /**
     * Creates the BranchStatus extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    stdcxx::optional<BranchStatus::Status> m_status;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_BRANCHSTATUSADDER_HPP
