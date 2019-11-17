#pragma once

#include <string>
#include <map>
#include <array>
#include <vector>
#include <mutex>
#include <random>
class theather
{
public:
    explicit theather(std::string name) :name(name) { };
    std::string GetName() const { return name; };
    void AddNewFilm(std::string movieName);
    int NumberSeatsAvailable(std::string movieName);
    std::vector<unsigned> GetAvailableSeats(std::string movieName);
    std::vector<int> BookSeat(std::string movieName, const std::vector<int>& seatsToBook);
    static void setRandomSeed();
private:
    unsigned getRandomSeat(std::string movieName);
    static const int NumberOfSeatsPerTheather = 20;
    std::string name;
    // Initially all set to true as all are available
    std::map<std::string, std::array<bool, NumberOfSeatsPerTheather>> listedFilmsAvailableSeats;
    std::mutex mtx;
    // Generating random seats
    static std::mt19937 rng;

};