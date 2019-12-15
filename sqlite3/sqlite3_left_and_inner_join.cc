//
// Created by rick on 2019/12/15.
//
#include "sqlite_orm.h"
#include <memory>
#include <string>
#include <iostream>

using namespace sqlite_orm;

struct Artist {
    std::unique_ptr<int> artistId;
    std::unique_ptr<std::string> name;
};

struct Album {
    std::unique_ptr<int> albumId;
    std::unique_ptr<std::string> title;
    std::unique_ptr<int> artistId;
};

struct Track {
    int trackId;
    std::string name;
    std::unique_ptr<int> albumId;
    int mediaTypeId;
    std::unique_ptr<int> genreId;
    std::unique_ptr<std::string> composer;
    long milliseconds;
    std::unique_ptr<long> bytes;
    double unitPrice;
};

inline auto initStorage(const std::string& path) {
    using namespace sqlite_orm;

    return make_storage(
        path,
        make_table("artists",
                   make_column("ArtistId", &Artist::artistId, primary_key()),
                   make_column("Title", &Album::title),
                   make_column("ArtistId", &Album::artistId)),

        make_table("tracks",
                   make_column("TrackId", &Track::trackId, primary_key()),
                   make_column("Name", &Track::name),
                   make_column("AlbumId", &Track::albumId),
                   make_column("MediaTypeId", &Track::mediaTypeId),
                   make_column("GenreId", &Track::genreId),
                   make_column("Composer", &Track::composer),
                   make_column("Milliseconds", &Track::milliseconds),
                   make_column("Bytes", &Track::bytes),
                   make_column("UnitPrice", &Track::unitPrice)));
}

int main(int argc, char* argv[]) {
    auto storage = initStorage("chinook.db");
    using namespace sqlite_orm;

    auto rows = storage.select(
        columns(&Artist::artistId, &Album::albumId),
        left_join<Album>(on(c(&Album::artistId) == &Artist::artistId)),
        order_by(&Album::albumId));
    std::cout <<  "rows count = " << rows.size() << std::endl;

    for (auto &row: rows) {
        auto& artistId = std::get<0>(row);
        if (artistId)
            std::cout << *artistId;
        else{
            std::cout << "null";
        }
    }
}