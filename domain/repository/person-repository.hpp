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
public:

    PersonRepository()
       : client(mongocxx::uri{}),
         collection(client["phonebook"]["contacts"]) {}

    void save(const Person& person)
    {
        int nextRow = 1;
        auto cursor = collection.find({});
        for (auto&& doc : cursor) {
            if (doc.find("row") != doc.end()) {
                int currentRow = doc["row"].get_int32();
                if (currentRow >= nextRow) {
                    nextRow = currentRow+ 1;
                }
            }
        }
        auto doc = bsoncxx::builder::basic::document{};
        doc.append(
         bsoncxx::builder::basic::kvp("row", nextRow),
            bsoncxx::builder::basic::kvp("name", person.name),
            bsoncxx::builder::basic::kvp("surname", person.surname),
            bsoncxx::builder::basic::kvp("phone", person.phone),
            bsoncxx::builder::basic::kvp("mail", person.mail)
        );

        std::cout << "Kişi başarıyla kaydedildi. "<< bsoncxx::to_json(doc.view()) << std::endl;
        collection.insert_one(doc.view());
    }

    Person findPerson(const std::string& name) {
        auto cursor = collection.find(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("name", name)));

        bool found=false;

        for (auto&& doc : cursor) {
            found=true;
            Person person(doc);
            person.print_info();
        }
        /*auto view = result->view();
        std::string id = view["_id"].get_oid().value.to_string();
        int row = view["row"].get_int32();
        /*row kısmını ekle
        std::string name = view["name"].get_value().get_string().value.data();
        std::string surname = view["surname"].get_value().get_string().value.data();
        std::string phone = view["phone"].get_value().get_string().value.data();
        std::string mail = view["mail"].get_value().get_string().value.data();
        return Person(id, row, name, surname, phone, mail);*/
        if (!found) {
            throw std::runtime_error("Belirtilen kişi bulunamadı.");
        }
    }


    std::vector<Person> findAll() {
        std::vector<Person> persons;

        for (auto&& doc : collection.find({})) {
            persons.emplace_back(doc);
           /* std::string id = doc["_id"].get_oid().value.to_string();
            int row = doc["row"].get_int32();
            std::string name = doc["name"].get_string().value.data();
            std::string surname = doc["surname"].get_string().value.data();
            std::string phone = doc["phone"].get_string().value.data();
            std::string mail = doc["mail"].get_string().value.data();*/

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

        auto result = collection.update_one(filter.view(), updateDoc.view());

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
