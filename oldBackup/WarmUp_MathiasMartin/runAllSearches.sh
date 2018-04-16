#$1 = 1st algorithm (e.g. bfs), $2 = 2nd algorithm (e.g. dfs), $3 = timeout (e.g. 180), $4 = memory (e.g. 1024)
#If timeouts larger than 300 are wished, edits will have to be done in runSearchClient.sh as well, as probe usually just interrupts ./runSearchClient
#Runs all searches for the warmup-assignment. For parameters, see up, so call by e.g. doing ./runAllSearches bfs dfs 180 4096.
#Will give a file output.txt, which can be compiled using latex.
javac searchclient/*.java
LEVEL=(SAD1 SAD2 friendsofDFS friendsofBFS SAFirefly SACrunch)
g++ prober.cpp -o prober
for VAL in {0..10..2}
do
lvl=$((VAL/2))
#echo $alg
#echo ${LEVEL[${lvl}]}
./runSearchClient.sh ${LEVEL[${lvl}]} $1 $VAL $4 $3&
#./runSearchClient.sh SACrunch $1 $VAL $4 &
PID0=$!
./prober $3 $VAL
kill $PID0
done

for VAL in {1..11..2}
do
lvl=$((VAL/2))
./runSearchClient.sh "${LEVEL[$lvl]}" $2 $VAL $4 $3&
PID0=$!
./prober $3 $VAL
kill $PID0
done
g++ makeTable.cpp -o makeTable
./makeTable
#Code below was tought to actually print a pdf with the values. However, due to running the latex command actually starts a program expecting class-files, this is omitted for now
#touch table.tex
#echo "\documentclass{article}\n\usepackage[utf8]{inputenc}\n\begin{document}\n" >> table.tex
#cat output.txt >> table.tex
#echo "\end{document}" >> table.tex
#pdflatex table.tex -interaction=nonstopmode
rm -f log0.txt log1.txt log2.txt log3.txt log4.txt log5.txt log6.txt log7.txt log8.txt log9.txt log10.txt log11.txt log12.txt makeTable prober

