// onlinemovie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "onlinemoviedb.h"
#include "movie.h"
#include "Theather.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Instantiate these two static variables
unique_ptr<onlinemoviedb> onlinemoviedb::singletonDataBase = nullptr;
once_flag onlinemoviedb::initFlag;

onlinemoviedb::onlinemoviedb(std::string pathToDb)
{
    // The assumptions made for the database file are as follows:
    //  1- The file lists each theather in one row along with all movies separated by character comma (,)
    //  2- Same theathres can be present in multiple lines featuring different movie names
    // ie. Theahter London,The Lion King,Terminator
    //     Theather Supreme,Bambi,Ghost
    ifstream myfile;
    myfile.open(pathToDb);
    string line, item;
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            std::getline(myfile, line, '\n');
            std::stringstream ss(line);
            std::vector<string> TheatherContents;
            while (!ss.eof() && !line.empty())
            {
                getline(ss, item, ',');
                TheatherContents.emplace_back(item);
            }
            if (TheatherContents.size() == 0) break; // Last line of file;
            theather* currentTheather = nullptr;
            for (size_t i = 0; i < TheatherContents.size(); i++)
            {
                if (0 == i)
                {
                    // This is the theather in each line
                    string thistheathrename = TheatherContents.at(i);
                    auto findifmethod = [thistheathrename](const unique_ptr<theather>& th) {return th->GetName() == thistheathrename; };
                    auto it = std::find_if(theathersVect.begin(), theathersVect.end(), findifmethod);
                    if (it != theathersVect.end())
                    {
                        // This is an existing theather
                        currentTheather = it->get();
                    }
                    else
                    {
                        // This is a new theather
                        theathersVect.push_back(make_unique<theather>(TheatherContents[i]));
                        currentTheather = theathersVect.back().get();
                    }
                }
                else
                {
                    // These are the movies.
                    std::string moviename = TheatherContents.at(i);
                    movie* pMovie = getMoviePtrFromName(moviename);
                    if (pMovie)
                    {
                        // This is an existing movie
                        pMovie->AddNewTheather(currentTheather);
                        currentTheather->AddNewFilm(pMovie->GetMovieName());
                    }
                    else
                    {
                        // This is a new movie
                        moviesVec.push_back(make_unique<movie>(moviename));
                        moviesVec.back()->AddNewTheather(currentTheather);
                        currentTheather->AddNewFilm(moviesVec.back()->GetMovieName());
                    }
                }
            }
        }
    }
    else
    {
        cout << "Error. Cannot open the file!" << endl;
    }
    myfile.close();
}

onlinemoviedb& onlinemoviedb::getdatabase(std::string pathToDb)
{
    auto callOnceFunction = [pathToDb]() {singletonDataBase.reset(new onlinemoviedb(pathToDb)); };
    std::call_once(initFlag, callOnceFunction);
    return *singletonDataBase;
}

std::vector<std::string> onlinemoviedb::ListMovies(void) const
{
    vector<string>retVal;
    for (auto& movie : moviesVec)
    {
        retVal.push_back(movie->GetMovieName());
    }
    return retVal;
}

std::vector<std::string> onlinemoviedb::ListTheathers(void) const
{
    vector<string>retVal;
    for (auto& th : theathersVect)
    {
        retVal.push_back(th->GetName());
    }
    return retVal;
}

std::vector<std::string> onlinemoviedb::ListTheathersForMovie(const std::string movieName) const
{
    vector<string>retVal;
    movie* pMovie = getMoviePtrFromName(movieName);

    if (pMovie)
    {
        retVal = pMovie->GetTheathersForThisMovie();
    }
    return retVal;
}

movie* onlinemoviedb::getMoviePtrFromName(const std::string movieName) const
{
    movie* retVal = nullptr;
    
    // Find_if should return an iterator different from the end iterator if the movie is found
    auto findifmethod = [movieName](const unique_ptr<movie>& mv) {return mv->GetMovieName() == movieName; };
    auto it = std::find_if(moviesVec.begin(), moviesVec.end(), findifmethod);

    if (it != moviesVec.end())
    {
        retVal = it->get();
    }
    return retVal;
}

std::vector<unsigned> onlinemoviedb::ListAvailableSeats(const std::string movieName, const std::string theatherName)const
{
    std::vector<unsigned> retVal;

    movie* pMovie = getMoviePtrFromName(movieName);

    if (pMovie)
    {
        retVal = pMovie->GetAvailableSeats(theatherName);
    }
    return retVal;
}

std::vector<int> onlinemoviedb::BookSeat(const std::string movieName, const std::string theatherName, const std::vector<int>& seatsToBook) const
{
    std::vector<int> retVal;
    movie* pMovie = getMoviePtrFromName(movieName);

    if (pMovie)
    {
        retVal = pMovie->BookSeat(theatherName, seatsToBook);
    }

    return retVal;
}

std::vector<int> onlinemoviedb::BookSeat(const std::string movieName, const std::string theatherName, const unsigned numRandomSeats) const
{
    std::vector<int> retVal;

    movie* pMovie = getMoviePtrFromName(movieName);

    if (pMovie)
    {
        // If the seats need to be randomly chosen, then I need to pass a vector
        // of size the number of seats to book and contents equals to -1
        std::vector<int> vectorminusone(numRandomSeats, -1);
        retVal = pMovie->BookSeat(theatherName, vectorminusone);
    }
    return retVal;
}

#ifdef PRODUCTION
int main(int argc, char** argv)
{
    using namespace std;
#ifdef CMAKEBUILD
    string dbname = "../../../testdatabase.csv";
#else
    string dbname = "testdatabase.csv";
#endif
    
    if (argc > 1)
    {
        // If the user wants to use a different database file
        dbname = argv[1];
    }

    auto& db = onlinemoviedb::getdatabase(dbname);
    while (true)
    {
        cout << endl << endl;
        cout << "These are the list of movies available" << std::endl;
        auto movies = db.ListMovies();
        for (auto i = 0; i < movies.size(); i++)
        {
            cout << i << "- " << movies.at(i) << endl;
        }
        cout << "Select a movie (type integer associated to movie) or -1 to finish: ";
        int movieId = -1;
        cin >> movieId;
        if (movieId < 0 || movieId >= movies.size())
        {
            break;
        }
        cout << endl << endl;
        cout << "The movie " << movies.at(movieId) << " is available in the following theathres" << std::endl;
        auto theathers = db.ListTheathersForMovie(movies.at(movieId));
        for (auto i = 0; i < theathers.size(); i++)
        {
            cout << i << "- " << theathers.at(i) << endl;
        }
        cout << "Select a theathre (type integer associated to theathre): ";
        int theathreId = -1;
        cin >> theathreId;
        if (theathreId < 0 || theathreId >= theathers.size())
        {
            return 0;
        }
        cout << "The movie " << movies.at(movieId) << " in theathre " << theathers.at(theathreId) << "has the following list of sits available" << endl;
        auto seats = db.ListAvailableSeats(movies.at(movieId), theathers.at(theathreId));
        for (auto seat : seats)
        {
            cout << seat << " ";
        }
        cout << endl << endl;
        cout << "Type the seat numbers in a sequence separated by spaces: ";
        string listofnumbers;
        std::vector<int> nums;
        while (getline(std::cin, listofnumbers))
        {
            bool breakflag = false;
            std::stringstream ss(listofnumbers);
            int ti;
            while (ss >> ti)
            {
                nums.push_back(ti);
                breakflag = true;
            }
            if (breakflag)
                break;
        }
        auto numBooked = db.BookSeat(movies.at(movieId), theathers.at(theathreId), nums);
        cout << "A total number of " << numBooked.size() << " tickets have been booked for you" << endl;
        cout << "These are: ";
        for (auto seat : numBooked)
        {
            cout << seat << " ";
        }
        cout << endl;
    }
    return 0;
}
#endif

