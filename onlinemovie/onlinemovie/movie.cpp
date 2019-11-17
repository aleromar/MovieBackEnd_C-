#include "movie.h"
#include "theather.h"

std::string movie::GetMovieName(void)
{
    return name;
}

void movie::AddNewTheather(theather* theather)
{
    theathersMap.push_back(theather);
}

std::vector<std::string> movie::GetTheathersForThisMovie(void)
{
    std::vector<std::string> retVal;
    for (theather* pth : theathersMap)
    {
        // Select only those that have at least one seat available
        if (pth->NumberSeatsAvailable(name) > 0)
        {
            retVal.push_back(pth->GetName());
        }
    }
    return retVal;
}

std::vector<unsigned> movie::GetAvailableSeats(std::string theathername)
{
    std::vector<unsigned> retVal;
    for (theather* pth : theathersMap)
    {
        if (pth->GetName() == theathername)
        {
            retVal = pth->GetAvailableSeats(name);
        }
    }
    return retVal;
}

std::vector<int> movie::BookSeat(std::string theathername, const std::vector<int>& seatsToBook)
{
    std::vector<int> retVal; 
    for (theather* pth : theathersMap)
    {
        if (pth->GetName() == theathername)
        {
            retVal = pth->BookSeat(name, seatsToBook);
        }
    }
    return retVal;
}