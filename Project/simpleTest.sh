#Runs the server on the MAsimple.lvl, good for testing purposes.
make
java -Dsun.java2d.opengl=true -jar server.jar -l levels/MAsimple1.lvl -c "bin/searcher 512" -g
