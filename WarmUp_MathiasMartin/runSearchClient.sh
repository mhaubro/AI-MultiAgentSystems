java -Dsun.java2d.opengl=true -jar server.jar -l levels/$1.lvl -c "java -Xmx$4m searchclient.SearchClient -$2 " -t $5 >> /dev/null 2> log$3.txt
