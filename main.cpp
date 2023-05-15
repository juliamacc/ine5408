// Copyright [2023] Julia Macedo e João Pedro Cordeiro

#include <cctype>
#include <string>
#include <cstring>
#include <iostream>
#include <exception>
#include <fstream>

#include "parse.h"
#include "array_stack.h"
#include "array_queue.h"
#include "linked_list.h"

using namespace structures;


/*
 * @brief This is a cenario structure, it will storage the cenario variables
 *
 * 
 */
struct Cenario {
    std::string nome;
    int altura;
    int largura;
    int X;
    int Y;
    bool* matriz;

    Cenario() {}

    Cenario(
        std::string nome,
        int altura,
        int largura,
        int X,
        int Y,
        bool* matriz
    ) :
       nome(nome),
       altura(altura),
       largura(largura),
       X(X),
       Y(Y),
       matriz(matriz) {}
    
    int size() {
        return altura * largura;
    }
};


std::string read_file(char* xmlfilename);
bool* matriz_str_to_array(std::string& matriz_string);
void parse (const std::string &input, LinkedList<Cenario> &lista_cenarios);
int counter(Cenario* cenario);

int main() {

    char xmlfilename[100];

    std::cin >> xmlfilename;  // entrada
    
    // Read input file
    std::string input = read_file(xmlfilename);
    LinkedList<Cenario> *list_cenarios = new LinkedList<Cenario>;

    // Parce the cenarios and check if the structure of the XML is correct
    try {
        parse(input, *list_cenarios);
    }
    catch (...) {
        std::cout << "erro" << std::endl;   // Print erro in the terminal
        return 0;
    }


    int count = 0;
    // Check how many spaces the robot will clean
    for (int i = 0; i < list_cenarios->size(); i++) {
        Cenario cenario = list_cenarios->at(i);
        count = counter(&cenario);
        std::cout << cenario.nome << " " << count << std::endl;
    }
    

    // std::cout << xmlfilename << std::endl;  // esta linha deve ser removida

    return 0;
}

std::string read_file(char* xmlfilename) {
    

    std::ifstream file (xmlfilename);
    if (file.is_open())
    {
        file.seekg (0, std::ios::end);
        size_t size = file.tellg();

        std::string memblock(size, ' ');
        file.seekg(0);
        file.read(&memblock[0], size);
        file.close();

        return memblock;
    }
    else std::cout << "Unable to open file";

    return "";
}

void parse (const std::string &input, LinkedList<Cenario> &lista_cenarios) {
    LeitorString r(input);
    ArrayStack<std::string> stack(100);
    Cenario cenario;

    while (r.get_i()) {
        std::string contents = r.limite('<');

        if (!r.get_i()) break;
        LeitorString::Tag tag = r.analisa_tag();

        if (!tag.is_closing) {  // Check if the tag IS NOT closing
            stack.push(tag.key);    // Push in the stack

            if (tag.key == "cenario") {
                cenario = Cenario();
            }
        } else {    // If the tag IS closing
            std::string current = stack.pop();

            if (current != tag.key) {
                throw parse_error(cenario.nome);
            }

            if (tag.key == "cenario") {
                lista_cenarios.push_back(cenario);
            }
            else if (tag.key == "nome") {
                cenario.nome = contents;
            }
            else if (tag.key == "x") {
                cenario.X = std::stoi(contents);
            }
            else if (tag.key == "y") {
                cenario.Y = std::stoi(contents);
            }
            else if (tag.key == "largura") {
                cenario.largura = std::stoi(contents);
            }
            else if (tag.key == "altura") {
                cenario.altura = std::stoi(contents);
            }
            else if (tag.key == "matriz") {
                cenario.matriz = matriz_str_to_array(contents);
            }
        }
    }

    // Check size of the stack after the parse
    if (stack.size()) {
        throw parse_error("xml structure erro");
    }
}

bool* matriz_str_to_array(std::string& matriz_string) {
    LeitorString r(matriz_string);

    std::string trimmed = ""; // Result string without beeing trimmed

    while (char ch = r.get_i()) {    // Remove the blank spaces
        if (!std::isspace(ch)){
            trimmed += ch;
        }
        ++r;    // If it is space, jump cursor to next char
    }

    int lenght = trimmed.length();
    bool* matriz = new bool[lenght];

    for (int i = 0; i < lenght; i++) {
        matriz[i] = trimmed[i] - 48;    // ascii to 0 and 1
    }

    return matriz;
}

int counter(Cenario* cenario) {
    LinkedList<int> index_list;
    
    int size_matriz = cenario->size();
    bool* matriz = cenario->matriz;
    bool* matriz_limpa;
    matriz_limpa = new bool[size_matriz];

    int count = 0;

    // Fill the empty matriz
    for (int i = 0; i < size_matriz; i++) {
        matriz_limpa[i] = false;
    }

    // Filling the inicial robot space
    int index_inicial = (cenario->Y) + ((cenario->X) * cenario->largura);
    if (matriz[index_inicial] == false) {
        return 0;
    }
    index_list.push_back(index_inicial);
    matriz_limpa[index_inicial] = true;
    count++;

    // Checking the rest of the matriz
    while (!index_list.empty()) {

        int index = index_list.pop_front();

        int up = index - cenario->largura;
        int down = index + cenario->largura;
        int right = index + 1;
        int left = index - 1;

        if ((up >= 0) && (matriz_limpa[up] == false) && (matriz[up] == true)) {
            index_list.push_back(up);
            matriz_limpa[up] = true;
            count++;
        }
        if ((down <= size_matriz) && (matriz_limpa[down] == false) && (matriz[down] == true)) {
            index_list.push_back(down);
            matriz_limpa[down] = true;
            count++;
        }
        if ((right <= size_matriz) && (matriz_limpa[right] == false) && (matriz[right] == true)) {
            index_list.push_back(right);
            matriz_limpa[right] = true;
            count++;
        }
        if ((left >= 0) && (matriz_limpa[left] == false) && (matriz[left] == true)) {
            index_list.push_back(left);
            matriz_limpa[left] = true;
            count++;
        }
    }

    return count;
}