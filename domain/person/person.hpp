//
// Created by elif on 11/25/24.
//

#ifndef PERSON_HPP
#define PERSON_HPP
#include <string>


class Person {

    public:


    std::string id;
    int row;
    std::string name;
    std::string surname;
    std::string phone;
    std::string mail;

    Person(const std::string id, const int row, const std::string& name, const std::string& surname, const std::string& phone, const std::string& mail)
        : id(id), row(0), name(name), surname(surname), phone(phone), mail(mail) {}

    Person() : id(""),row(0), name(""), surname(""), phone(""), mail("") {}

    Person(const bsoncxx::document::view& view) {
        id = view["_id"].get_oid().value.to_string();
        name = view["name"].get_string().value.data();
        surname = view["surname"].get_string().value.data();
        phone = view["phone"].get_string().value.data();
        mail = view["mail"].get_string().value.data();
    }
    void print_info() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Row: " << row << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Surname: " << surname << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        std::cout << "Mail: " << mail << std::endl;
    }

};



#endif //PERSON_HPP
