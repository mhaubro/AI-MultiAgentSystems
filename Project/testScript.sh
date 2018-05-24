#Runs the server on the MAsimple.lvl, good for testing purposes.
make
java -Dsun.java2d.opengl=true -jar server.jar -l $1 -c "bin/searcher 1536 10000" -g
 
