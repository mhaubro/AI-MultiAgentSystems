make
java -Dsun.java2d.opengl=true -jar server.jar -l levels/$1.lvl -c "searchClientcpp/searchClient" -t 300 -g 200
