# Двоичное дерево поиска и AVL дерево

Добавляет N (54000) чисел в случайном (первое) и возрастающем (второе) порядке в BST и AVL деревья. Ищет N/10 случайных
чисел в каждом дереве. Удаляет N/10 случайных элементов в каждом дереве.

Добавляет 10000000 чисел в ДОП по алгоритму 1 и 2. Ищет 1000000 случайных чисел в каждом дереве.

# Зависимости сборки

* c++11
* cmake
* gtest

# Сборка cmake

``` shell
mkdir ../build
cd ../build
cmake ..
cmake --build .
```

## Запуск тестов

```shell 
./binary-search-tree --gtest_filter=BST.CompareTwoTreesAvl        # AVL
./binary-search-tree --gtest_filter=BST.CompareTrees              # BST
./binary-search-tree --gtest_filter=Dop.Algo1                     # DOP1
./binary-search-tree --gtest_filter=Dop.Algo2                     # DOP2
```

# Docker

```shell
docker build -t testing .
docker run -it testing sh
/src/testing/build/testing
```
