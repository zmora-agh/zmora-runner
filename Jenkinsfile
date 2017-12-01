pipeline {
	agent any
	triggers { pollSCM('H/5 * * * *') }
	stages {
		stage('Build') {
			steps {
				dir('build'){
					sh 'cmake ../ && make -j4'
				}
			}
		}
	}
}
