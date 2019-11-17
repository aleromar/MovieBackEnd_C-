#include "Theather.h"


std::mt19937 theather::rng;

void theather::AddNewFilm(std::string movieName)
{
    std::array<bool, NumberOfSeatsPerTheather> initialArray;
    initialArray.fill(true);
    listedFilmsAvailableSeats[movieName] = initialArray;
}

void theather::setRandomSeed(void)
{
    rng.seed(42);
}

unsigned theather::getRandomSeat(std::string movieName)
{
    unsigned retVal = 0;
    auto vector = GetAvailableSeats(movieName);
    // Common resource rng is already protected by calling method
    if (vector.size() > 0)
    {
        std::uniform_int_distribution<unsigned> seatDistr(0, vector.size() - 1);
        {
            std::lock_guard<std::mutex> lock(mtx);
            retVal = vector.at(seatDistr(rng));
        }
    }
    return retVal;
}

int theather::NumberSeatsAvailable(std::string movieName)
{
    int count = 0;
    std::lock_guard<std::mutex> lock(mtx);
    if (listedFilmsAvailableSeats.find(movieName) != listedFilmsAvailableSeats.end())
    {
        auto arr = listedFilmsAvailableSeats[movieName];
        for (auto it : arr)
        {
            count += it ? 1 : 0;
        }
    }
    return count;
}

std::vector<unsigned> theather::GetAvailableSeats(std::string movieName)
{
    std::vector<unsigned> retVec;
    std::lock_guard<std::mutex> lock(mtx);
    if (listedFilmsAvailableSeats.find(movieName) != listedFilmsAvailableSeats.end())
    {
        auto arr = listedFilmsAvailableSeats[movieName];
        int index = 0;
        for (auto it : arr)
        {
            if (it)
            {
                retVec.push_back(index);
            }
            index++;
        }
    }
    return retVec;
}

std::vector<int> theather::BookSeat(std::string movieName, const std::vector<int>& seatsToBook)
{
    std::vector<int> retVal;
    
    for (auto seat : seatsToBook)
    {
        if (!(seat >= NumberOfSeatsPerTheather || seat < -1))
        {
            if (listedFilmsAvailableSeats.find(movieName) != listedFilmsAvailableSeats.end())
            {
                if (seat == -1)
                {
                    // need to generate randomly
                    seat = getRandomSeat(movieName);
                }
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (listedFilmsAvailableSeats[movieName].at(seat))
                    {
                        // Success, switch flag to unavailable
                        listedFilmsAvailableSeats[movieName].at(seat) = false;
                        retVal.push_back(seat);
                    }
                }
            }
        }
    }
    return retVal;
}