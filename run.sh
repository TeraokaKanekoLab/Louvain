modularity() {
    echo "g++ src/main.cpp -o ./main --std=c++17"
    g++ src/main.cpp -o ./main --std=c++17
    echo "./main $1 $2"
    ./main $1 $2
    rm ./main
}
old() {
    echo "g++ old/main.cpp -o ./main --std=c++17"
    g++ old/main.cpp -o ./main --std=c++17
    echo "./main $1 $2"
    ./main $1 $2
    rm ./main
}

case $1 in
"all")
    modularity $2 $3
    ;;
"old")
    old $2 $3
    ;;
esac
