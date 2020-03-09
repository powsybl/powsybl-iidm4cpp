/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODE_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODE_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeExport.hpp>

namespace powsybl {

namespace iidm {

class Line;

namespace extensions {

    namespace entsoe {

        class EXT_ENTSOE_EXPORT MergedXnode : public Extension {
        public:  // Extension
            const std::string &getName() const override;

            const std::type_index &getType() const override;

        public:
            MergedXnode(Line &line, double rdp, double xdp, double xnodeP1, double xnodeQ1, double xnodeP2,
                        double xnodeQ2,
                        const std::string &line1Name, const std::string &line2Name, const std::string &code);

            ~MergedXnode() noexcept override = default;

            const std::string &getCode() const;

            const std::string &getLine1Name() const;

            const std::string &getLine2Name() const;

            double getRdp() const;

    double getXdp() const;

    double getXnodeP1() const;

    double getXnodeP2() const;

    double getXnodeQ1() const;

    double getXnodeQ2() const;

    MergedXnode& setCode(const std::string& xNodeCode);

    MergedXnode& setLine1Name(const std::string& line1Name);

    MergedXnode& setLine2Name(const std::string& line2Name);

    MergedXnode& setRdp(double rdp);

    MergedXnode& setXdp(double xdp);

    MergedXnode& setXnodeP1(double xNodeP1);

    MergedXnode& setXnodeQ1(double xNodeQ1);

    MergedXnode& setXnodeP2(double xNodeP2);

    MergedXnode& setXnodeQ2(double xNodeQ2);

private:
    static double checkDividerPosition(double dp);

    static double checkPowerFlow(double value);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    double m_rdp; // r divider position 1 -> 2

    double m_xdp; // x divider position 1 -> 2

    double m_xnodeP1;

    double m_xnodeQ1;

    double m_xnodeP2;

    double m_xnodeQ2;

    std::string m_line1Name;

    std::string m_line2Name;

    std::string m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_MERGEDXNODE_HPP

