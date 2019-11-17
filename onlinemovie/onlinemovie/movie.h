#pragma once

#include <string>
#include <map>
#include <array>
#include <vector>

// Forward declare to avoid compile dependencies
class theather;

/**
 * The purpose of this class is to represent
 * a movie object. It's a very simple object
 * that mainly keeps its name and a list of 
 * cinemas where this movie is featured.
 *
 * It works together with theather objects 
 * to make the booking process for a tuple(movie, theather)
 * All methods here are thread-safe as information is written
 * initially only once by one thread, and then it's just 
 * read only operations
 */
class movie
{
public:
    /**
     * Constructor made explicit enforces appropriate argument type.
     * @param the name of the movie.
     */
    explicit movie(std::string name) :name(name) { };
    /**
     * This method returns the name of this movie
     *
     * @return a string containing the name of this movie
     */
    std::string GetMovieName(void);
    /**
     * This method informs this movie that it's being broadcasted in
     * a new cinema
     */
    void AddNewTheather(theather* theather);
    /**
     * Creates and returns a list of all cinemas featuring this movie
     *
     * @return a vector containing the theathers featuring this movie
     */
    std::vector<std::string> GetTheathersForThisMovie(void);
    /**
     * This method finds out what are the available seats for a tuple
     * (movie, theather)
     *
     * @param the name of the theather.
     * @return a vector containing the available seats
     */
    std::vector<unsigned> GetAvailableSeats(std::string theathername);
    /**
     * This method just forwards the booking request from the user to
     * the theather object as this object owns the tickets.
     *
     * @param the name of the theather.
     * @params the list of seats to reserve
     * @return a vector containing the successfully allocated seats
     */
    std::vector<int> BookSeat(std::string theathername,const std::vector<int>& seatsToBook);
private:
    std::string name;
    std::vector<theather*> theathersMap;
};