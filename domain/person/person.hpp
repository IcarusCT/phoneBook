//
// Created by elif on 11/25/24.
//

#ifndef PERSON_HPP
#define PERSON_HPP
#include <string>


class Person {

    public:

    std::string id;
    std::string name;
    std::string surname;
    std::string phone;
    std::string mail;

    Person(const std::string id, const std::string& name, const std::string& surname, const std::string& phone, const std::string& mail)
        : id(id), name(name), surname(surname), phone(phone), mail(mail) {}

    Person() : id(0), name(""), surname(""), phone(""), mail("") {}

    void print_info() const;

};



#endif //PERSON_HPP
