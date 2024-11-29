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

            service.findPersonsByName();
        }

    else if (option == "3") {
        service.findPersonsByName();

        try {
            int row;
            std::cout << "Güncellemek istediğiniz kişinin sıra numarasını girin: ";
            std::cin >> row;
            std::cin.ignore();

            auto person = service.findPersonByRow(row);
            person.print_info();

            std::string newName, newSurname, newPhone, newMail;
            std::cout << "Yeni isim (boş bırakmak için Enter): ";
            std::getline(std::cin, newName);
            std::cout << "Yeni soyisim (boş bırakmak için Enter): ";
            std::getline(std::cin, newSurname);
            std::cout << "Yeni telefon (boş bırakmak için Enter): ";
            std::getline(std::cin, newPhone);
            std::cout << "Yeni mail (boş bırakmak için Enter): ";
            std::getline(std::cin, newMail);

            if (!newName.empty()) person.name = newName;
            if (!newSurname.empty()) person.surname = newSurname;
            if (!newPhone.empty()) person.phone = newPhone;
            if (!newMail.empty()) person.mail = newMail;

            service.updatePerson(person, row);

            std::cout << "Kişi başarıyla güncellendi." << std::endl;
            person.print_info();

        } catch (const std::exception& e) {
            std::cerr << "Hata: " << e.what() << std::endl;
        }


    }

    else if (option == "4") {
        try {
            service.findPersonsByName();
            std::cout << "Silmek istediğiniz kişinin sıra numarasını girin: ";
            int row;
            std::cin >> row;

            service.removePerson(row);
            std::cout << "Kişi başarıyla silindi." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Hata: " << e.what() << std::endl;
        }
    }

    else if (option == "5") {
        service.listAll();
    }

    return 0;
}
