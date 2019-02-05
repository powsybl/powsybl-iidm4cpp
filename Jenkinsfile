/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

def maintainers = "mathieu.bague@rte-france.com"

// Enable code-coverage only for the master branch
def withSonar = (env.gitlabActionType == null) || (env.gitlabSourceBranch == "master")
def buildType = withSonar ? "Debug" : "Release"
def codeCoverage = withSonar ? "TRUE" : "FALSE"

def buildWrapper = "LANG=C /home/jenkins/tools/build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir ./output"

node('powsybl-rh72') {

    try {
        stageDevin('Cloning git repository') {
            gitCheckout {}
        }

        stageDevin('GCC') {
            dir('build') {
                // Remove old build directory
                deleteDir()

                sh """
                cmake -DCMAKE_BUILD_TYPE=${buildType} -DCODE_COVERAGE=${codeCoverage} -DCMAKE_CXX_COMPILER=g++ ..
                ${buildWrapper} make -j4
                make tests
                """
            }
        }

        /*
        stageDevin('Clang') {
            dir('build-clang') {
                // Remove old build directory
                deleteDir()

                sh """
                cmake -DCMAKE_CXX_COMPILER=clang++ ..
                make -j4
                make tests
                """
            }
        }
        */

        // Run tests and compute code coverage results
        stageDevin('Code-coverage', withSonar) {
            dir('build') {
                sh """
                make code-coverage

                # Clean non relevant files
                rm -f coverage/reports/#usr#*
                """
            }
        }

        // Run sonar analysis
        stageDevin('Sonarqube', withSonar) {
            sonar {
                useMaven = "false"
            }
        }

    } catch (Exception e) {
        notify {
            to = maintainers
            errorMsg = e.toString()
        }
        throw e
    } finally {
        step([$class: 'WsCleanup'])
    }
}
