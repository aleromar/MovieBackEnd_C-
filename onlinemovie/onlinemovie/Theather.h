#pragma once

#include <string>
#include <map>
#include <array>
#include <vector>
#include <mutex>
#include <random>

/**
 * The purpose of this class is to represent
 * a theather similar to what a theather is.
 * This means that the theather has a number of 
 * rooms with a movie being played in each of them
 * each room in this case has equal size (20) as suggested
 * in the problem description
 *
 * This class needs to be designed with multithreading in mind
 * as the map data structure may be subject to data races when
 * used concurrently. I've decided to use a very simple approach
 * using a mutex, but if performance was an issue, some research
 * on lock-free data structures should be made.
 */
class theather
{
public:
    /**
     * Constructor made explicit enforces appropriate argument type.
     * @param the name of the theather.
     */
    explicit theather(std::string name) :name(name) { };
    /**
     * This method returns the name of this theather
     *
     * @return a string containing the name of this theather
     */
    std::string GetName() const { return name; };
    /**
     * This method assigns a new movie to a particular theather.
     */
    void AddNewFilm(std::string movieName);
    /**
     * This method returns the number of seats still available
     * for a given movie
     * @param the name of the movie for which the number of seats is requested
     */
    int NumberSeatsAvailable(std::string movieName);
    /**
     * This method returns the number of seats still available
     * for a given movie
     * @param the name of the movie for which the number of seats is requested
     */
    std::vector<unsigned> GetAvailableSeats(std::string movieName);
    /**
     * This method attempts to reserve seats specified in the vector "seatsToBook".
     * It's possible that not all seats are available and so the assumption is to 
     * book as many seats as it's possible.
     *
     * @param the name of the movie for which seats are to be booked
     * @param the list of seats to be booked (best effort).
     * @return a list with the seats successfully booked
     */
    std::vector<int> BookSeat(std::string movieName, const std::vector<int>& seatsToBook);
    /**
     * Helper method to initialize the seed of the random number generator
     * This method is made static as it's not object specific, rather class specific.
     */
    static void setRandomSeed();
private:
    /**
     * Helper method to obtain a random seat out of a number of them available
     * @param the name of the movie for which random seat needs be generated
     * @return the seat number randomly allocated for the movie
     */
    unsigned getRandomSeat(std::string movieName);
    /**
    * Name of the theathre
    */
    std::string name;
    /**
     * Map that contains the movies given in this cinema and the 
     * seats available for that movie
     */
    static const int NumberOfSeatsPerTheather = 20;
    std::map<std::string, std::array<bool, NumberOfSeatsPerTheather>> listedFilmsAvailableSeats;
    /**
     * Each theather uses this mutex to protect himself against data races arising from concurrent access
     */
    std::mutex mtx;
    /**
     * Utility for generating random seats
     */
    static std::mt19937 rng;

};