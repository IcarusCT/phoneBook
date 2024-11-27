#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include "repository/person-repository.hpp"
#include <bsoncxx/types.hpp>
#include "person/person.hpp"
#include <mongocxx/exception/exception.hpp>

int main()
{
    std::string name, surname, phone, mail;

    mongocxx::client client{mongocxx::uri{"mongodb://localhost:27017"}};

    auto db = client["phonebook"];
    auto collection = db["contacts"];

    std::cout << "name:";
    std::getline(std::cin , name);
    std::cout << "surname:";
    std::getline(std::cin , surname);
    std::cout << "number:";
    std::getline(std::cin , phone);
    std::cout << "mail:";
    std::getline(std::cin , mail);

    Person person("", name, surname, phone, mail);
    PersonRepository repository;

    try {
        repository.save(person);

    } catch (const mongocxx::exception& e) {
        std::cerr << "Veritabanı hatası: " << e.what() << std::endl;
    }


    return 0;
}
