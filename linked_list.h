#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>


namespace structures {

//! ...
template<typename T>
class LinkedList {
 public:
    //! ...
    LinkedList();  // construtor padrão
    //! ...
    ~LinkedList();  // destrutor
    //! ...
    void clear();  // limpar lista
    //! ...
    void push_back(const T& data);  // inserir no fim
    //! ...
    void push_front(const T& data);  // inserir no início
    //! ...
    void insert(const T& data, std::size_t index);  // inserir na posição
    //! ...
    void insert_sorted(const T& data);  // inserir em ordem
    //! ...
    T& at(std::size_t index);  // acessar um elemento na posição index
    //! ...
    T pop(std::size_t index);  // retirar da posição
    //! ...
    T pop_back();  // retirar do fim
    //! ...
    T pop_front();  // retirar do início
    //! ...
    void remove(const T& data);  // remover específico
    //! ...
    bool empty() const;  // lista vazia
    //! ...
    bool contains(const T& data) const;  // contém
    //! ...
    std::size_t find(const T& data) const;  // posição do dado
    //! ...
    std::size_t size() const;  // tamanho da lista

 private:
    class Node {  // Elemento
     public:
        explicit Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {  // getter: dado
            return data_;
        }

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }

        const Node* next() const {  // getter const: próximo
            return next_;
        }

        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_{nullptr};
    };

    Node* end() {  // último nodo da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }

    Node* head{nullptr};
    std::size_t size_{0u};
};

}  // namespace structures

#endif

template<typename T>
structures::LinkedList<T>::LinkedList() {
    head = nullptr;
    size_ = 0;
}

template<typename T>
structures::LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
void structures::LinkedList<T>::clear() {
    while (size_ > 0) {
        pop_front();
    }
}

template<typename T>
void structures::LinkedList<T>::push_back(const T& data) {
    insert(data, size_);
}

template<typename T>
void structures::LinkedList<T>::push_front(const T& data) {
    Node* new_node = new Node(data, head);
    head = new_node;
    size_++;
}

template<typename T>
void structures::LinkedList<T>::insert(const T& data, std::size_t index) {
    if (index > size_) {
        throw std::out_of_range("index invalido");
    } else {
        if (index == 0) {
            push_front(data);
        } else {
            Node *new_node = new Node(data);
            Node *previous = head;
            std::size_t posicao = 0;
            // Caminha ate um antes da posicao
            while (posicao < index - 1) {
                previous = previous->next();
                posicao++;
            }
            if (previous->next() != nullptr) {
                new_node->next(previous->next());
            }
            previous->next(new_node);
            size_++;
        }
    }
}

template<typename T>
void structures::LinkedList<T>::insert_sorted(const T& data) {
    if (empty()) {
        push_front(data);
    } else {
        Node *current = head;
        std::size_t index = 0;
        while (current->data() < data && index < size_) {
            if (current->next() != nullptr) {
                current = current->next();
            }
            index++;
        }
        insert(data, index);
    }
}

template<typename T>
T& structures::LinkedList<T>::at(std::size_t index) {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    } else if (index > size_ || index < 0) {
        throw std::out_of_range("posicao invalida");
    } else {
        Node *current = head;
        for (std::size_t i = 0; i < index; i++) {
            current = current->next();
        }
        return current->data();
    }
}

template<typename T>
T structures::LinkedList<T>::pop(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("lista vazia ou index invalido");
    } else {
        if (index == 0) {
            return pop_front();
        } else {
            // Caminha ate o fim da lista
            Node *previous = head;
            for (std::size_t posicao = 0; posicao < index-1; posicao++) {
                previous = previous->next();
            }
            Node *eliminate = previous->next();
            previous->next(eliminate->next());
            T info_back = eliminate->data();
            delete eliminate;
            size_--;
            return(info_back);
        }
    }
}

template<typename T>
T structures::LinkedList<T>::pop_back() {
    return pop(size_ - 1);
}

template<typename T>
T structures::LinkedList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    } else {
        size_--;
        Node *eliminate = head;
        T info_back = eliminate->data();
        head = eliminate->next();
        delete eliminate;
        return(info_back);
    }
}

template<typename T>
void structures::LinkedList<T>::remove(const T& data) {
    pop(data);
}

template<typename T>
bool structures::LinkedList<T>::empty()const {
    return (size_ == 0);
}

template<typename T>
bool structures::LinkedList<T>::contains(const T& data) const {
    return (find(data) != size_);
}


template<typename T>
std::size_t structures::LinkedList<T>::find(const T& data) const {
    Node *current = head;
    std::size_t pos = 0;
    while (current->data() != data && pos < size_) {
        if (current->next() != nullptr) {
            current = current->next();
        }
        pos++;
    }
    if (pos >= size_) {
        return size_;
    } else {
        return(pos);
    }
}

template<typename T>
std::size_t structures::LinkedList<T>::size() const {
    return(size_);
}