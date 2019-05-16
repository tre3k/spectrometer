#!/bin/bash

thisdir=`pwd`
tomcat=apache-tomcat-9.0.20

case $1 in
    start)
	$thisdir/$tomcat/bin/startup.sh
	;;
    stop)
	$thisdir/$tomcat/bin/shutdown.sh
	;;
esac
