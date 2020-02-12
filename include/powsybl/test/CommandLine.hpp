/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_COMMANDLINE_HPP
#define POWSYBL_TEST_COMMANDLINE_HPP

#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace powsybl {

namespace test {

class CommandLine {
public:
    const boost::program_options::variable_value &getOptionValue(const std::string &name) const {
        return m_options[name];
    }

protected:
    void parse(const boost::program_options::options_description &desc) {
        m_options.clear();
        boost::program_options::store(
                boost::program_options::parse_command_line(
                        boost::unit_test::framework::master_test_suite().argc,
                        boost::unit_test::framework::master_test_suite().argv,
                        desc),
                m_options);
        boost::program_options::notify(m_options);
    }

private:
    boost::program_options::variables_map m_options;

};

}  // namespace test

}  // namespace powsybl

#endif  // POWSYBL_TEST_COMMANDLINE_HPP
