pipeline {
	agent any


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
					sh 'cd Ness1 && make'
				}
			}
		}
	}
}
