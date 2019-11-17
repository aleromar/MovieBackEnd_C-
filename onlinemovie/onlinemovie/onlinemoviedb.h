#pragma once
#include <string>
#include <vector>
#include <memory>
#include <mutex>

//#define PRODUCTION

// Forward declare to avoid compile dependencies
class movie;
class theather;

/**
 * The purpose of this class is to enable an
 * interface between the user and the database.
 * It has been assummed that the database is a 
 * csv file containing a map between theathers and
 * movies. This class offers functionality to read
 * in the database and creates objects of types 
 * theathre and movie and the relationships between
 * them.
 * Please note that this class is a singleton. The
 * singleton design pattern was understood to provide
 * a simple solution for small-size databases. The
 * sigleton has been designed with care to enable concurrent
 * access to the booking mechanism.
 */
class onlinemoviedb
{
public:
    /**
     * This is the factory method for this class. Note that 
     * the constructor is private and so the only way to get
     * access to the functionality is by means of this static
     * method. The parameter is a very relaxed form of dependency
     * inversion, so it's possible to detach unit testing from
     * production
     *
     * @param path to the database holding information for movies
     * @return a reference to an instance of onlinemoviedb class
     */
    static onlinemoviedb& getdatabase(std::string pathToDb);
    /**
     * This method should be queried by the user whenever the 
     * list of movies available is required. This method will
     * return a set (no repetitions) of movies given in all theathres.
     *
     * @return a list of all movies available in the database.
     */
    std::vector<std::string> ListMovies()const;
    /**
     * This method should be queried by the user whenever the
     * list of theathers available is required. This method will
     * return a set (no repetitions) of all theathers.
     *
     * @return a list of all theathers available in the database.
     */
    std::vector<std::string> ListTheathers()const;
    /**
     * This method should be queried by a user whenever the user
     * wants to find out what are the theathers broadcasting a 
     * particular movie.
     *
     * @param the name of the movie for which theathers need be listed.
     * @return a list of all theathers displaying the movie passed as argument.
     */
    std::vector<std::string> ListTheathersForMovie(const std::string movieName) const;
    /**
     * The user calls this method when a film and a theather have already 
     * been selected, and she wants to know what are the seats available
     * If the movie does not exist or it's not given in a particular theather or
     * any other error including movie been sold out, then a vector of size 0
     * is returned.
     *
     * @param the name of the movie.
     * @param the name of the theathre.
     * @return a list of all available seats for a given combination movie-theathre.
     */
    std::vector<unsigned> ListAvailableSeats(const std::string movieName, const std::string theatherName)const ;
    /**
     * The user will call into this method when she wants to book specific seats. 
     * The number of seats selected corresponds to the size of the passed vector.
     * The actual seats are the contents of the vector. It may be possible that
     * due to concurrent access, not all seats can be allocated and so the method
     * returns a list with the seats that have indeed been reserved.
     *
     * @param the name of the movie.
     * @param the name of the theathre.
     * @param the list of chosen seats
     * @return a list of all available seats that have been successfully booked.
     */
    std::vector<int> BookSeat(const std::string movieName, const std::string theatherName,const std::vector<int>& seatsToBook) const;
    /**
     * The user will call into this method when she wants to book RANDOM seats.
     * The number of seats selected corresponds to parameter numRandomSeats.
     * The seats will be booked randomly by the engine. It may be possible that
     * due to concurrent access, not all seats can be allocated and so the method
     * returns a list with the seats that have indeed been reserved.
     *
     * @param the name of the movie.
     * @param the name of the theathre.
     * @param the number of seats to be randomly booked
     * @return a list of all available seats that have been successfully booked.
     */
    std::vector<int> BookSeat(const std::string movieName, const std::string theatherName, const unsigned numRandomSeats)const;
    /**
     * The destructor has been moved here so as to enable the unique_ptr deleter to
     * access it.
     */
    ~onlinemoviedb() = default;
private:
    /**
     * Constructor made private to enforce the singleton pattern.
     * explicit enforces appropriate argument type
     */
    explicit onlinemoviedb( std::string pathToDb);
    /**
     * A singleton class cannot be copied or asigned, hence
     * these methods are deleted
     */
    onlinemoviedb(const onlinemoviedb&) = delete;
    onlinemoviedb& operator=(const onlinemoviedb&) = delete;
    /**
     * This is just a helper method to reduce code duplication
     * The idea is that it returns the pointer to a movie object
     * associated to a particular movie name
     *
     * @param the name of the movie
     * @return a raw pointer to a movie object
     */
    movie* getMoviePtrFromName(std::string movieName) const;

    /**
     * The following two vectors have the ownership
     * for all movies and theathers so far created.
     * These pointers are used to access methods to
     * support the requested functionality
     */
    std::vector < std::unique_ptr<movie>> moviesVec;
    std::vector < std::unique_ptr<theather>> theathersVect;

    /**
     * These static variables are part of the singleton
     * design. In particular the flag is used to initialize
     * the onlinemoviedb instance just once. This is of importance
     * under multithreading scenarios
     */
    static std::unique_ptr<onlinemoviedb> singletonDataBase;
    static std::once_flag initFlag;
};