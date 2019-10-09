pipeline {
	agent any


	stages {
		stage ('clean') {
			steps {
				sh 'cd Ness1 && make clean'
			}
		}
		stage ('build 1') {
			steps {
				sh 'cd Ness1 && make'
			}
		}
	}
}
