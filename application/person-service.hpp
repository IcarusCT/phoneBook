//
// Created by elif on 11/25/24.
//

#ifndef PERSON_SERVICE_HPP
#define PERSON_SERVICE_HPP
#include <vector>
#include "person/person.hpp"
#include "repository/person-repository.hpp"


class PersonService {

    PersonRepository& personRepository;


public:

    PersonService();

    explicit PersonService(PersonRepository& repo) : personRepository(repo) {}

    void savePerson(const Person& person) {

        personRepository.save(person);
    }

    void updatePerson(const Person& person, int row) {

        personRepository.update(row, person);
    }

    void removePerson(const int row) {

        personRepository.remove(row);
    }

    std::vector<Person> listAll() {

        try {
            auto persons = personRepository.listAll();

            if (persons.empty()) {
                std::cout << "Rehberde kayıtlı kişi yok." << std::endl;
            }

            for (const auto& person : persons) {
                person.print_info();
                std::cout << "------------------" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Hata: " << e.what() << std::endl;
        }
    }


    void findPersonsByName() {
        std::cout << "İsim girin: ";
        std::string name;
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cerr << "Hata: İsim boş olamaz." << std::endl;
            return;
        }

        try {

            auto persons = personRepository.findPersonsByName(name);

            if (persons.empty()) {
                std::cout << "Bu isimle eşleşen kişi bulunamadı." << std::endl;
            } else {

                for (const auto& person : persons) {
                    person.print_info();
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Hata: " << e.what() << std::endl;
        }
    }

    Person findPersonByRow(int row) {
        return personRepository.findByRow(row);
    }


};

#endif //PERSON_SERVICE_HPP
