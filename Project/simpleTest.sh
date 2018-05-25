#Runs the server on the MAsimple.lvl, good for testing purposes.
make
#java -Dsun.java2d.opengl=true -jar server.jar -l MARegionTestLevelHard.lvl -c "bin/searcher 1536 10000" -g
java -Dsun.java2d.opengl=true -jar server.jar -l competition_levels/MABahaMAS.lvl -c "bin/searcher 1536 10000" -g
