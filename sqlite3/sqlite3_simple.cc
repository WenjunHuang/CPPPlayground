//
// Created by rick on 2019/12/15.
//
#include "sqlite_orm.h"
#include <iostream>
#include <memory>
#include <string>
using namespace sqlite_orm;
struct User {
    int id;
    std::string firstName;
    std::string lastName;
    int birthDate;
    std::unique_ptr<std::string> imageUrl;
    int typeId;
};

struct UserType {
    int id;
    std::string name;
};

int main(int argc, char* argv[]) {
    auto storage = make_storage(
        "sqlite3_simple.sqlite",
        make_table("users",
                   make_column("id", &User::id, autoincrement(), primary_key()),
                   make_column("first_name", &User::firstName),
                   make_column("last_name", &User::lastName),
                   make_column("birth_date", &User::birthDate),
                   make_column("image_url", &User::imageUrl),
                   make_column("type_id", &User::typeId)),
        make_table(
            "user_types",
            make_column("id", &UserType::id, autoincrement(), primary_key()),
            make_column("name", &UserType::name,
                        default_value("place_holder"))));

    User user{-1,
              "Jonh",
              "Doe",
              664416000,
              std::make_unique<std::string>("url_to_heaven"),
              3};

    auto insertedId = storage.insert(user);
    std::cout << "insertedId = " << insertedId << std::endl;
    user.id = insertedId;

    User secondUser{-1, "Alice", "Inwonder", 831168000, {}, 2};
    insertedId = storage.insert(secondUser);
    std::cout << "insertedId = " << insertedId << std::endl;
    secondUser.id = insertedId;

    secondUser.firstName = "Nicholas";
    secondUser.imageUrl =
        std::make_unique<std::string>("https://cdn1.iconfinder.com/data/icons/"
                                      "man-icon-set/100/man_icon-21-512.png");
    storage.update(secondUser);

    if (auto user = storage.get_pointer<User>(insertedId)) {
        std::cout << "user = " << user->firstName << " " << user->lastName
                  << std::endl;
    } else {
        std::cout << "no user with id" << insertedId << std::endl;
    }

    storage.update_all(set(c(&User::lastName) = "Hardey", c(&User::typeId) = 2),
                       where(c(&User::firstName) == "Tom"));
}
