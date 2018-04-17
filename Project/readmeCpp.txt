README

Running a test-setup

Compiling the searchclient:
make
(Alternatively do a make clean first)

Running, using linux:
java -Dsun.java2d.opengl=true -jar server.jar -l levels/SAsoko1_48.lvl -c "bin/searcher 512 10000" -g

512 being memory usable and 10000 being iterations between prints

Note that the iteration-concept is up for change

As a default: "make" only creates the searcher, "make all" creates searcher, prober and latex-folder, make clean cleans searcher, make cleanall cleans everything.

For doing a testrun, try executing the shell-script ./runSearchClient.sh
Multiple agents, do :
for (i = 0; i < #num; i++){
[Action], [Action], ...
If no action -> NoOp.
