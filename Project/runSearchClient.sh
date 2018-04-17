make
java -Dsun.java2d.opengl=true -jar server.jar -l levels/$1.lvl -c "bin/searcher 512 10000" -g
