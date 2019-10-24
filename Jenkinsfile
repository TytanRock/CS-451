pipeline {
	agent {
		dockerfile true
	}


	stages {
		stage ('Clean All') {
			steps {
				dir('Ness1') {
					sh 'make clean'
				}
				dir('Ness2') {
					sh 'make clean'
				}
			}
		}
		stage ('Build All') {
			steps {
				dir('Ness1') {
					sh 'make'
				}
				dir('Ness2') {
					sh 'make'
				}
			}
		}
		stage ('Test All') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps_test'
					sh './gcov/5ps_test'
				}
				dir('Ness2') {
					sh 'make test'
					dir('gcov') {
						sh './schedule_test'
					}
				}
			}
		}
		stage ('Coverage All') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps'
					dir('gcov') {
						sh 'gcovr -r . --xml -o rep.xml'
						sh 'gcovr -r . --html --html-details -o rep.html'
					}
				}
				dir('Ness2/gcov') {
					sh 'gcovr -r . --xml -o rep.xml'
					sh 'gcovr -r . --html --html-details -o rep.html'
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/*.xml'
			junit 'Ness2/gcov/run_xunit.xml'
			cobertura coberturaReportFile: 'Ness1/gcov/*.xml'
			cobertura coberturaReportFile: 'Ness2/gcov/rep.xml'
			publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'Ness2/gcov', reportFiles: 'rep.html', reportName: 'HTML Report', reportTitles: ''])
		}
	}
}
