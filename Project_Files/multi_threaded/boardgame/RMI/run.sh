
# Script to run the server with RMI!

javac *.java
COUNT=$((COUNT+1))
PORTNUM=$((COUNT+5000))
rmiregistry $PORTNUM &
java Server
