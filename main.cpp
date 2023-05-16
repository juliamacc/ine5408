// Copyright [2023] Julia Macedo e João Pedro Cordeiro

#include <cctype>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "array_stack.h"
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

// Funções para ler as strings
char get_i(const std::string& str, int i) {
    return str[i];
}

char caracter(const std::string& str, int& i, char ch = 0) {
    char p = str[i];
        
    if (!ch || ch == p) {
        i++;
        return p;
    }
    return 0;
}

std::string palavra(const std::string& str, int& i) {
    int start = i;
    char ch = str[i];

    while(ch && !isspace(ch) && ch != '<' && ch != '>') {
        i++;
        ch = str[i];
    }
    return str.substr(start, i - start);
}

std::string limite(const std::string& str, int& i, char ch) {
    int start = i;
    while(str[i] && str[i] != ch) {
        i++;
    }
    return str.substr(start, i - start);
}

void tirar_espaco(const std::string& str, int& i) {
    while (str[i] && isspace(str[i])) {
        i++;
    }
}

char Espera(const std::string& str, int& i, char ch) {
    char result = caracter(str, i, ch);
    if (!result) {
        throw std::runtime_error(std::string("Espera character '") + ch + "'. Got '" + str[i] + "'.");
    }
    return result;
}

struct Tag {
    std::string key = "";
    bool is_closing = false;
};
Tag analisa_tag(const std::string& str, int& i) {
    Espera(str, i, '<');
    bool is_closing = caracter(str, i, '/');

    std::string key = palavra(str, i);

    Espera(str, i, '>');
    Tag tag;
    tag.is_closing = is_closing;
    tag.key = key;

    return tag;
}

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
    int i = 0;
    ArrayStack<std::string> stack(100);
    Cenario cenario;

    while (get_i(input, i)) {
        std::string contents = limite(input, i, '<');

        if (!get_i(input, i)) break;
        Tag tag = analisa_tag(input, i);

        if (!tag.is_closing) {
            stack.push(tag.key);

            if (tag.key == "cenario") {
                cenario = Cenario();
            }
        } else {
            std::string current = stack.pop();

            if (current != tag.key) {
                throw std::runtime_error(cenario.nome);
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

    if (stack.size()) {
        throw std::runtime_error("xml structure erro");
    }
}

bool* matriz_str_to_array(std::string& matriz_string) {
    int i = 0;
    std::string trimmed = ""; 

    while (char ch = get_i(matriz_string, i)) {
        if (!std::isspace(ch)){
            trimmed += ch;
        }
        ++i;
    }

    int length = trimmed.length();
    bool* matriz = new bool[length];

    for (int i = 0; i < length; i++) {
        matriz[i] = trimmed[i] - 48;
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
        int largura = cenario->largura;

        int x = index / largura;
        int y = index % largura;

        int up = (x - 1)* largura + y;
        int down = (x + 1)* largura + y;
        int right = (y + 1) + x * largura;
        int left = (y - 1) + x * largura;

        if (((x - 1) >= 0) && (matriz_limpa[up] == false) && (matriz[up] == true)) {
            index_list.push_back(up);
            matriz_limpa[up] = true;
            count++;
        }
        if (((x + 1) < cenario->altura) && (matriz_limpa[down] == false) && (matriz[down] == true)) {
            index_list.push_back(down);
            matriz_limpa[down] = true;
            count++;
        }
        if (((y + 1) < largura) && (matriz_limpa[right] == false) && (matriz[right] == true)) {
            index_list.push_back(right);
            matriz_limpa[right] = true;
            count++;
        }
        if (((y - 1) >= 0) && (matriz_limpa[left] == false) && (matriz[left] == true)) {
            index_list.push_back(left);
            matriz_limpa[left] = true;
            count++;
        }
    }

    return count;
}