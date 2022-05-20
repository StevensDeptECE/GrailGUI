#pragma once

#include <iostream>

class Person {
 public:
  uint64_t id;
  std::string firstName;
  std::string lastName;
  std::string room;
  uint64_t phone;
  std::string email;
  Person(uint64_t id, const std::string& firstName, const std::string& lastName,
         const std::string& room, uint64_t phone, const std::string& email)
      : id(id),
        firstName(firstName),
        lastName(lastName),
        room(room),
        phone(phone),
        email(email) {}
  friend std::istream& operator>>(std::istream& s, Person& p) {
    return s >> p.id >> p.firstName >> p.lastName >> p.email;
  }
};
