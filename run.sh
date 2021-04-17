modularity() {
    echo "g++ src/main.cpp -o ./test --std=c++17"
    g++ src/test.cpp -o ./test --std=c++17
    echo "./main $1 $2"
    ./test $1 $2
    rm ./test
}

case $1 in
"all")
    modularity $2 $3
    ;;
esac
