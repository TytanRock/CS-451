pipeline {
	agent {
		dockerfile true
	}


	stages {
		stage ('clean') {
			steps {
				dir('Ness1') {
					sh 'make clean'
				}
			}
		}
		stage ('build 1') {
			steps {
				dir('Ness1') {
					sh 'make'
				}
			}
		}
		stage ('Test 1') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps_test'
					sh './gcov/5ps_test'
				}
			}
		}
		stage ('Coverage 1') {
			steps {
				dir('Ness1') {
					sh 'make gcov/5ps'
					dir('gcov') {
						sh 'gcovr -r . --xml -o rep.xml'
						sh 'gcovr -r . --html --html-details -o rep.html'
					}
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/*.xml'
			cobertura coberturaReportFile: 'Ness1/gcov/*.xml'
			publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'Ness1/gcov', reportFiles: 'rep.html', reportName: 'HTML Report', reportTitles: ''])
		}
	}
}
