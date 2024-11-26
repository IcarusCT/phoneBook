//
// Created by elif on 11/25/24.
//

#ifndef PERSON_SERVICE_HPP
#define PERSON_SERVICE_HPP
#include <vector>
#include "person/person.hpp"
#include "repository/person-repository.hpp"


class PersonService {

    private:
    PersonRepository& personRepository;

public:


    PersonService();

    explicit PersonService(PersonRepository& repo) : personRepository(repo) {}

    void PersonService::savePerson(const Person &person) {

        personRepository().save(person);

    }

    void PersonService::updatePerson(const Person &person, int id) {

        personRepository().update(person, person.id());
    }

    void PersonService::removePerson(const Person &person) {

        personRepository().remove(person);
    }
    std::vector<Person> findAll() {

        return personRepository().findAll();
    }

    void PersonService::findById(const Person &person , int id) {

        return personRepository().findById(person.id);
    }

    private:
    PersonRepository PersonRepository;

};



#endif //PERSON_SERVICE_HPP
