modularity() {
    echo "g++ src/louvain.cpp -o ./louvain --std=c++17"
    g++ src/louvain.cpp -o ./louvain --std=c++17
    echo "./louvain $1 $2"
    ./louvain $1 $2
    rm ./louvain
}
old() {
    echo "g++ old/main.cpp -o ./main --std=c++17"
    g++ old/main.cpp -o ./main --std=c++17
    echo "./main $1 $2"
    ./main $1 $2
    rm ./main
}
loop() {
    g++ src/louvain.cpp -o ./louvain --std=c++17
    for i in {20..30}
    do
    echo "# nodes considered same: $i"
    for j in {1..10}
    do
        python3 src/merge.py graph/sbm1.gr graph/sbm2.gr $i
        ./louvain $1 $2
    done
    done
}

case $1 in
"all")
    modularity $2 $3
    ;;
"old")
    old $2 $3
    ;;
"loop")
    loop $2 $3
    ;;
esac
