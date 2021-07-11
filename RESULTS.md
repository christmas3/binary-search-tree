# Таблица времени выполнения операций на двух BST, заполненных случайно и последовательно. 

#Размер данных 55000 целых чисел. Поиск и удаление 5500 чисел. Значения в секундах.
| Тип            |  insert  |  search  |  remove  |
| :---           |   ---:   |   ---:   |   ---:   |
| SequenceBst    |  49.6461 |  5.01968 |  4.70972 |
| RandomBst      |   0.0543 |  0.00607 |  0.00563 |

#SequenceBst
Вставка N элементов в возрастающем порядке в BTS имеет сложность O(N^2). 
Поиск и удаление N/10 элементов из данного BST  имеет сложность O(N^2 / 10).

#RandomBst
Вставка N элементов в случайном порядке в BTS имеет сложность O(N*logN).
Поиск и удаление N/10 элементов из данного BST имеет сложность O(N*logN / 10).