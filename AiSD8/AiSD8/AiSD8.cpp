#include <iostream>
#include <vector>
#include <string>
#include <locale>
//7 вариант

// --- Часть 1: Двоичный поиск (лексикографический) ---
int binarySearchString(std::string arr[], int n, const std::string& target) { //поиск строки
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid; //нашли
        if (arr[mid] < target) left = mid + 1; //ищем справа (лексикографически)
        else right = mid - 1; //ищем слева
    }
    return -1; //не нашли
}

// --- Часть 2: Хеш-таблица с методом цепочек ---
template <typename K, typename V>
class HashTable {
private:
    struct Node { //узел списка
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    std::vector<Node*> table; //массив бакетов
    int size;
    int count;

    int hash(const K& key) const { //хеш-функция для int
        return key % size;
    }

public:
    HashTable(int s = 10) : size(s), count(0) { //конструктор
        table.resize(size, nullptr);
    }

    ~HashTable() { //деструктор (очистка памяти)
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const K& key, const V& value) { //вставка
        int index = hash(key);
        Node* current = table[index];
        while (current != nullptr) { //проверка, есть ли такой ключ
            if (current->key == key) {
                current->value = value; //обновляем значение
                return;
            }
            current = current->next;
        }
        Node* newNode = new Node(key, value); //создаем новый узел
        newNode->next = table[index];
        table[index] = newNode;
        count++;
    }

    bool find(const K& key, V& result) const { //поиск
        int index = hash(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                result = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool remove(const K& key) { //удаление
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) table[index] = current->next;
                else prev->next = current->next;
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void print() const { //вывод таблицы
        for (int i = 0; i < size; i++) {
            std::cout << i << ": ";
            Node* current = table[i];
            while (current != nullptr) {
                std::cout << "(" << current->key << ", " << current->value << ") ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
};

using namespace std; //разрешаем std в main

int main() {
    setlocale(LC_ALL, "ru");

    // --- Тест 1: Двоичный поиск строк ---
    cout << "== Часть 1: Лексикографический поиск ==" << endl;
    string words[] = { "apple", "banana", "cherry", "date", "elderberry" };
    int n = sizeof(words) / sizeof(words[0]);

    string target = "cherry";
    int index = binarySearchString(words, n, target);

    if (index != -1) cout << "Слово \"" << target << "\" найдено под индексом " << index << endl;
    else cout << "Слово \"" << target << "\" не найдено" << endl;

    target = "fig";
    index = binarySearchString(words, n, target);
    if (index != -1) cout << "Слово \"" << target << "\" найдено под индексом " << index << endl;
    else cout << "Слово \"" << target << "\" не найдено" << endl;

    // --- Тест 2: Хеш-таблица ---
    cout << "\n== Часть 2: Хеш-таблица (метод цепочек) ==" << endl;
    HashTable<int, string> ht(5); //размер 5

    ht.insert(10, "десять");
    ht.insert(20, "двадцать");
    ht.insert(30, "тридцать");
    ht.insert(15, "пятнадцать");
    ht.insert(25, "двадцать пять");

    cout << "Исходная таблица:" << endl;
    ht.print();

    string value;
    cout << "\nПоиск ключа 20: ";
    if (ht.find(20, value)) cout << "найдено значение \"" << value << "\"" << endl;
    else cout << "не найдено" << endl;

    cout << "\nУдаляем ключ 20..." << endl;
    ht.remove(20);

    cout << "Таблица после удаления:" << endl;
    ht.print();

    return 0;
}