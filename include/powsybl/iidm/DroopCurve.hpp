/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DROOPCURVE_HPP
#define POWSYBL_IIDM_DROOPCURVE_HPP

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/range.hpp>

#include <set>

namespace powsybl {

namespace iidm {

class DroopCurveAdder;

/**
 * Droop Curve defines the droop function of a AcDcConverter in P_PCC_DROOP mode.
 * This curve is made of segments defined by its minimum voltage and the associated droop coefficient.
 * the maximum voltage of a segment if defined by the minimum voltage of the next segment (if any).
 * This ensure no overlapping and continuous segments definition.
 * If no segment, the default coefficient is 0.0
 */
class DroopCurve {

public:
    class Segment {
    public:
        Segment() = delete;

        struct compare {
            bool operator()(const Segment& seg1, const Segment& seg2) {
                return std::isless(seg1.getMinV(), seg2.getMinV());
            }
        };

        double getK() const;
        double getMinV() const;
        double getMaxV() const;

    private:
        Segment(DroopCurve& parentCurve, double minV, double k);
        friend class DroopCurveAdder;

        DroopCurve& m_parentCurve;
        double m_minV = stdcxx::nan();
        double m_k = stdcxx::nan();

    };

    virtual ~DroopCurve() noexcept = default;

    stdcxx::const_range<Segment> getSegments() const;

    double getK(double v) const;

    double getMaxV() const;

private:
    DroopCurve(double maxV);

    void addSegment(const Segment& segmentToAdd);
    friend class DroopCurveAdder;

    std::set<Segment, Segment::compare> m_segments;

    double m_maxV = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DROOPCURVE_HPP
