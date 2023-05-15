#include <cctype>
#include <string>
#include <cstring>
#include <iostream>
#include <exception>

class parse_error : std::exception {
    std::string message;

 public:
    parse_error() : message("") {}
    parse_error(std::string msg) : message(msg) {}

    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class LeitorString {
public:
    LeitorString(const std::string& string) : str(string) {}

    char get_i() {
        return str[i];
    }

    char caracter(char ch = 0) {
        char p = get_i();
        
        if (!ch || ch == p) {
            i++;
            return p;
        }
        return 0;
    }

    std::string palavra() {
        int start = i;
        char ch = get_i();

        while(ch && !isspace(ch) && ch != '<' && ch != '>') {
            i++;
            ch = get_i();
        }
        return str.substr(start, i - start);
    }

    std::string limite(char ch) {
        int start = i;
        while(get_i() && get_i() != ch) {
            i++;
        }
        return str.substr(start, i - start);
    }

    void tirar_espaco() {
        while (get_i() && isspace(get_i())) {
            i++;
        }
    }

    char expect(char ch) {
        char result = caracter(ch);
        if (!result) {
            throw parse_error(std::string("Expected character '") + ch + "'. Got '" + get_i() + "'.");
        }
        return result;
    }

    struct Tag {
        std::string key = "";
        bool is_closing = false;
    };
    Tag analisa_tag() {
        expect('<');
        bool is_closing = caracter('/');
        tirar_espaco();

        std::string key = palavra();

        tirar_espaco();
        expect('>');
        Tag tag;
        tag.is_closing = is_closing;
        tag.key = key;

        return tag;
    }

    LeitorString& operator++() {
        ++i;
        return *this;
    }

private:
    const std::string& str;
    int i = 0;
};