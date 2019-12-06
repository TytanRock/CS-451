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
				dir('Ness3') {
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
					sh './gcov/schedule_test'
				}
				dir('Ness3') {
					sh 'make test'
					sh './gcov/elevator_test'
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
				dir('Ness3/gcov') {
					sh 'gcovr -r . --xml -o rep.xml'
					sh 'gcovr -r . --html --html-details -o rep.html'
				}
			}
		}
	}

	post {
		always {
			junit 'Ness1/run_xunit.xml'
			junit 'Ness2/run_xunit.xml'
			junit 'Ness3/run_xunit.xml'
			
			cobertura coberturaReportFile: 'Ness3/gcov/rep.xml'
		}
	}
}
