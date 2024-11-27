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

#include "person-service.hpp"

int main()
{   PersonRepository repository;
    PersonService service(repository);
    std::string name, surname, phone, mail, option;
    int row;

    mongocxx::client client{mongocxx::uri{"mongodb://localhost:27017"}};

    auto db = client["phonebook"];
    auto collection = db["contacts"];

    std::cout << "Yapmak istediğiniz işlemi seçin: " << std::endl;
    std::cout << "1- Kişi ekle" << std::endl;
    std::cout << "2- Kişi bul" << std::endl;
    std::cout << "3- Kişi güncelle" << std::endl;
    std::cout << "4- Kişi sil" << std::endl;
    std::cout << "5- Rehberi görüntüle" << std::endl;
    std::getline(std::cin, option);

if (option == "1") {

    std::cout << "name:";
    std::getline(std::cin , name);
    std::cout << "surname:";
    std::getline(std::cin , surname);
    std::cout << "number:";
    std::getline(std::cin , phone);
    std::cout << "mail:";
    std::getline(std::cin , mail);

    Person person("", row, name, surname, phone, mail);

    try {

        repository.save(person);
    }

    catch (const mongocxx::exception& e) {

        std::cerr << "Veritabanı hatası: " << e.what() << std::endl;
    }
}

    else if (option == "2") {

        std::cout << "İsim girin:";
        std::getline(std::cin , name);

        try {

            Person person = repository.findPerson(name);

            std::cout << "Kişi bilgileri:" << std::endl;
            person.print_info();
            } catch (const std::exception& e) {
                std::cerr << "Hata: " << e.what() << std::endl;
            }
        }

    else if (option == "3") {
        std::string name;
        std::cout << "İsim girin:";
        std::getline(std::cin , name);
    }
    return 0;
}
