/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_TRAVERSERESULT_HPP
#define POWSYBL_MATH_TRAVERSERESULT_HPP

namespace powsybl {

namespace math {

enum class TraverseResult {
    /** Indicates that traversal should continue */
    CONTINUE,

    /** Indicates that traversal should terminate on current path */
    TERMINATE_PATH,

    /** Indicates that traversal should break, i.e., terminate on all paths */
    TERMINATE_TRAVERSER
};

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_TRAVERSERESULT_HPP
