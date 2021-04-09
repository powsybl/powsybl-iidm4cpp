/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODEADDER_HPP

#include <string>

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

class MergedXnodeAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit MergedXnodeAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    MergedXnodeAdder(const MergedXnodeAdder&) = default;

    /**
     * Move constructor
     */
    MergedXnodeAdder(MergedXnodeAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~MergedXnodeAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    MergedXnodeAdder& operator=(const MergedXnodeAdder&) = delete;

    /**
     * Move assignment operator
     */
    MergedXnodeAdder& operator=(MergedXnodeAdder&&) noexcept = delete;

    MergedXnodeAdder& withCode(const std::string& xNodeCode);

    MergedXnodeAdder& withLine1Name(const std::string& line1Name);

    MergedXnodeAdder& withLine2Name(const std::string& line2Name);

    MergedXnodeAdder& withRdp(double rdp);

    MergedXnodeAdder& withXdp(double xdp);

    MergedXnodeAdder& withXnodeP1(double xNodeP1);

    MergedXnodeAdder& withXnodeP2(double xNodeP2);

    MergedXnodeAdder& withXnodeQ1(double xNodeQ1);

    MergedXnodeAdder& withXnodeQ2(double xNodeQ2);

protected:
    /**
     * Creates the MergedXnode extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    double m_rdp = stdcxx::nan();

    double m_xdp = stdcxx::nan();

    double m_xnodeP1 = stdcxx::nan();

    double m_xnodeQ1 = stdcxx::nan();

    double m_xnodeP2 = stdcxx::nan();

    double m_xnodeQ2 = stdcxx::nan();

    std::string m_line1Name;

    std::string m_line2Name;

    std::string m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODEADDER_HPP
