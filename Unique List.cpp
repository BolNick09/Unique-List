#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

namespace errors
{
    class DuplicateElementException : public std::exception
    {
    public:
        // завести конструктор не принимающий аргументы
        DuplicateElementException() {}

        // завести конструктор принимающий строку
        DuplicateElementException(const char* msg) : std::exception(msg) {} //std::exception принимает аргументом C-строку

        // перегрузить метод для написания ошибки - const char* what() const noexcept
        const char* what() const noexcept override
        {
            return "ERROR: Duplicate element";
        }
    };

    class ElementNotFoundException : public std::exception
    {
    public:
        ElementNotFoundException() {}
        ElementNotFoundException(const char* msg) : std::exception(msg) {}
        
        const char* what() const noexcept override
        {
            return "ERROR: Element not found";
        }
    };
} 

template <typename T>
class Node
{
public:
    T data;
    Node* next;

    Node(T data) : data(data), next(nullptr) {}
};

template <typename T>
class UniqueLinkedList
{
private:
    Node<T>* head;
    std::map<int, Node<T>*> node_map;

public:
    UniqueLinkedList() : head(nullptr) {}

    // Проверка на дубликат с использованием node_map
    // Добавление нового элемента в node_map
    // Если элемент уже существует, выбросить DuplicateElementException
    void push_back(T value)
    {
        if (node_map.count(value)) //count - Number of elements with key that compares equivalent to key or x       
            throw errors::DuplicateElementException("Duplicate element");
        

        Node<T>* new_node = new Node<T>(value);
        new_node->next = head;
        head = new_node;

        node_map[value] = new_node;
    }

    // Проверка, существует ли элемент в списке
    // Удаление элемента из node_map
    // Если элемент не найден, выбросить ElementNotFoundException
    void remove(T value)
    {
        if (!node_map.count(value))      
            throw errors::ElementNotFoundException("Element not found");
        

        Node<T>* curr = head;
        Node<T>* prev = nullptr;

        while (curr && curr->data != value)
        {
            prev = curr;
            curr = curr->next;
        }

        if (!curr)// Элемент не найден           
            throw errors::ElementNotFoundException("ERROR: Element not found");
        

        if (!prev)// Удаляем первый элемент            
            head = head->next;        
        else        
            prev->next = curr->next;        

        node_map.erase(value);
    }

    void print()
    {
        Node<T>* curr = head;
        while (curr)
        {
            std::cout << curr->data << " ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    // Оператор сравнения для двух списков, должен возвращать true если все элементы совпадают (и наоборот)
    bool operator==(const UniqueLinkedList<T>& other) const
    {
        Node<T>* curr1 = head;
        Node<T>* curr2 = other.head;

        while (curr1 && curr2)
        {
            if (curr1->data != curr2->data)
            {
                return false;
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }

        return (!curr1 && !curr2);
    }

    ~UniqueLinkedList()
    {
        Node<T>* curr = head;
        while (curr)
        {
            Node<T>* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main()
{
    UniqueLinkedList<int> list;

    std::cout << "Check duplicate elements:" << std::endl;
    try
    {
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(2); // Должно выбросить исключение DuplicateElementException
    }
    catch (const errors::DuplicateElementException& e)
    {
        std::cerr << e.what() << std::endl;
    }

    list.print(); // Вывод: 1 2 3 (выводит 3 2 1, т.к. порядок элементов наоборот)

    std::cout << std::endl << "Check not fount element:" << std::endl;
    try
    {
        list.remove(2);
        list.print(); // Вывод: 1 3
        list.remove(2); // Должно выбросить исключение ElementNotFoundException
    }
    catch (const errors::ElementNotFoundException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::cout << std::endl << "Check matching elements in 2 lists" << std::endl;

    // добавить сюда проверку двух списков на одинаковость элементов (порядок элементов может отличаться)

    UniqueLinkedList<int> list2;
    list2.push_back(1);
    list2.push_back(3);
    list2.push_back(2);

    std::cout << (list == list2 ? "Lists equal" : "Lists NOT equal") << std::endl;

    return 0;
}