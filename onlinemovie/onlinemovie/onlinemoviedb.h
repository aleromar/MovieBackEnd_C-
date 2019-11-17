#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Theather.h"
#include "movie.h"


// This class will be a singleton class for managing the database
class onlinemoviedb
{
public:
    static onlinemoviedb& getdatabase(std::string pathToDb);
    
    std::vector<std::string> ListMovies()const;
    std::vector<std::string> ListTheathers()const;
    std::vector<std::string> ListTheathersForMovie(const std::string movieName) const;
    std::vector<unsigned> ListAvailableSeats(const std::string movieName, const std::string theatherName)const ;
    // This method to be used when the user knows exactly the seats she wants
    std::vector<int> BookSeat(const std::string movieName, const std::string theatherName,const std::vector<int>& seatsToBook) const;
    std::vector<int> BookSeat(const std::string movieName, const std::string theatherName, const unsigned numRandomSeats)const;
    // Moving it here so the unique_ptr can call it on destruction
    ~onlinemoviedb() = default;
private:
    explicit onlinemoviedb( std::string pathToDb);
    onlinemoviedb(const onlinemoviedb&) = delete;
    onlinemoviedb& operator=(const onlinemoviedb&) = delete;
    
    std::vector < std::unique_ptr<movie>> moviesVec;
    std::vector < std::unique_ptr<theather>> theathersVect;

    movie* getMoviePtrFromName(std::string movieName) const;

    static std::unique_ptr<onlinemoviedb> singletonDataBase;
    static std::once_flag initFlag;
};