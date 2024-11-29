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

        if (!found) {
            throw std::runtime_error("Belirtilen kişi bulunamadı.");
        }
    }


    std::vector<Person> listAll() {
        std::vector<Person> persons;

        for (auto&& doc : collection.find({})) {
            persons.emplace_back(doc);
        }

        return persons;
    }


    void update(const int row, const Person& updatedPerson) {

        auto result = collection.update_one(
               bsoncxx::builder::basic::make_document(
                   bsoncxx::builder::basic::kvp("row", row)
               ),
               bsoncxx::builder::basic::make_document(
               bsoncxx::builder::basic::kvp("$set",bsoncxx::builder::basic::make_document(
               bsoncxx::builder::basic::kvp("name", updatedPerson.name),
               bsoncxx::builder::basic::kvp("surname", updatedPerson.surname),
               bsoncxx::builder::basic::kvp("phone", updatedPerson.phone),
               bsoncxx::builder::basic::kvp("mail", updatedPerson.mail)
           )
           )
       )
       );

        if (!result) {
            throw std::runtime_error ("Belirtilen kişi bulunamadı.");
        }

    }

    void remove(const int row) {
        auto filter = bsoncxx::builder::basic::make_document(
         bsoncxx::builder::basic::kvp("row", row)
     );

        auto result = collection.delete_one(filter.view());

        if (!result) {
            throw std::runtime_error("Bu row numarasıyla bir kişi bulunamadı veya silinemedi.");
        }
    }

    std::vector<Person> findPersonsByName(const std::string& name) {
        std::vector<Person> persons;

        auto cursor = collection.find(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("name", name)));

        for (auto&& doc : cursor) {
            persons.emplace_back(doc);
        }

        return persons;
    }

    Person findByRow(int row) {
        auto result = collection.find_one(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("row", row)
        ));

        if (result) {
            return Person(result->view());
        }
        throw std::runtime_error("Bu sıra numarasıyla bir kişi bulunamadı.");
    }


private:

        mongocxx::client client;
        mongocxx::collection collection;

};

#endif //PERSONREPOSITORY_HPP
