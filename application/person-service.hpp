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

    void updatePerson(const Person& person, const std::string& id) {

        personRepository.update(id, person);
    }

    void removePerson(const std::string& id) {

        personRepository.remove(id);
    }

    std::vector<Person> findAll() {

        return personRepository.findAll();
    }

    Person findPerson(const std::string& id) {

        return personRepository.findPerson(id);
    }
};

#endif //PERSON_SERVICE_HPP
