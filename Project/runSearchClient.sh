#$1: Level, $2: Memory, in MB, $3: Iterations between prints.
make
java -Dsun.java2d.opengl=true -jar server.jar -l $1.lvl -c "bin/searcher $2 $3" -g
