#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include "repository/person-repository.hpp"
#include "bsoncxx/types.hpp"
#include "person/person.hpp"


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

    Person person(0, name, surname, phone, mail);

    bsoncxx::builder::stream::document document{};
    document << "name" << name;
             << "surname" << surname;
             << "phone" << phone
             << "mail" << mail;

    if (auto result = collection.insert_one(document.view()))
    {
        std::cout << "Veri başarıyla kaydedildi! ID: "
                  << result->inserted_id().get_oid().value.to_string() << std::endl;
    }

    else {
        std::cerr << "Veri kaydedilirken bir hata oluştu." << std::endl;
    }

    return 0;
}
