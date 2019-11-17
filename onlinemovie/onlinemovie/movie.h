#pragma once

#include <string>
#include <map>
#include <array>
#include <vector>
#include "theather.h"
class movie
{
public:
    explicit movie(std::string name) :name(name) { };
    std::string GetMovieName(void);
    void AddNewTheather(theather* theather);
    std::vector<std::string> GetTheathersForThisMovie(void);
    std::vector<unsigned> GetAvailableSeats(std::string theathername);
    std::vector<int> BookSeat(std::string theathername,const std::vector<int>& seatsToBook);
private:
    std::string name;
    std::vector<theather*> theathersMap;
};