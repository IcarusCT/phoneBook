//
// Created by elif on 11/25/24.
//
#include "person/person.hpp"
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <vector>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>


#ifndef PERSONREPOSITORY_HPP
#define PERSONREPOSITORY_HPP


class PersonRepository {
    PersonRepository()
    : client(mongocxx::uri{}),
      collection(client["phonebook"]["contacts"]) {}


public:


    void save(const Person& person)
    {
        auto doc = bsoncxx::builder::basic::document{};
        doc.append(
         bsoncxx::builder::basic::kvp("name", person.name),
            bsoncxx::builder::basic::kvp("surname", person.surname),
            bsoncxx::builder::basic::kvp("phone", person.phone),
            bsoncxx::builder::basic::kvp("mail", person.mail)
        );
        std::cout << "Kişi başarıyla kaydedildi. "<< bsoncxx::to_json(doc.view()) << std::endl;
        collection.insert_one(doc.view());
    }

    Person findById(const std::string& id) {
        auto result = collection.find_one(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("_id", id)));

        if (result) {
            auto view = result->view();

            std::string id = view["_id"].get_oid().value.to_string();
            std::string name = view["name"].get_value().get_string().value.data();
            std::string surname = view["surname"].get_value().get_string().value.data();
            std::string phone = view["phone"].get_value().get_string().value.data();
            std::string mail = view["mail"].get_value().get_string().value.data();
            return Person(id, name, surname, phone, mail);
        }
        std::cout << "Belirtilen kişi bulunamadı." << std::endl;
    }

    std::vector<Person> findAll() {
        std::vector<Person> persons;

        for (auto&& doc : collection.find({})) {
            std::string id = doc["_id"].get_oid().value.to_string();
            std::string name = doc["name"].get_string().value.data();
            std::string surname = doc["surname"].get_string().value.data();
            std::string phone = doc["phone"].get_string().value.data();
            std::string mail = doc["mail"].get_string().value.data();

            persons.emplace_back(id, name, surname, phone, mail);
        }

        return persons;
    }


    void update(const std::string& id, const Person& updatedPerson) {

        auto filter = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", (bsoncxx::oid(id)))
    );
        auto updateDoc = bsoncxx::builder::basic::make_document(

        bsoncxx::builder::basic::kvp("$set",bsoncxx::builder::basic::make_document(

               bsoncxx::builder::basic::kvp("name", updatedPerson.name),
               bsoncxx::builder::basic::kvp("surname", updatedPerson.surname),
               bsoncxx::builder::basic::kvp("phone", updatedPerson.phone),
               bsoncxx::builder::basic::kvp("mail", updatedPerson.mail)
           )
       )
   );

        // Belgeyi güncelleme
        auto result = collection.update_one(filter.view(), updateDoc.view());

        // Güncelleme sonucu kontrolü
        if (result && result->modified_count() == 1) {
            std::cout << "Kullanıcı başarıyla güncellendi." << std::endl;
        } else {
            std::cout << "Belirtilen kişi bulunamadı." << std::endl;
        }

    }

    void remove(const std::string& id) {
        auto filter = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(id))
    );

        collection.delete_one(filter.view());

    }


private:

        mongocxx::client client;
        mongocxx::collection collection;

};

#endif //PERSONREPOSITORY_HPP
