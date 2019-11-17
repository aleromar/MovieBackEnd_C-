// onlinemovie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "onlinemoviedb.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

unique_ptr<onlinemoviedb> onlinemoviedb::singletonDataBase = nullptr;
once_flag onlinemoviedb::initFlag;

onlinemoviedb::onlinemoviedb(std::string pathToDb)
{
    // This is thread safe as it's the constructor of a singleton class, therefore only accessed once
    // by one thread.

    // The file lists each theather in one row along with all movies separated by character comma (,)
    // ie. Theahter London,The Lion King,Terminator\nTheather Supreme,Bambi,Ghost
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
                    // These are the moviest.
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
    // Singleton pattern is not implemented with a local static onlinemoviedb
    // as I want to be able to inject a different database for testing.
    // Ref: https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton
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
        cout << retVal.back() << endl;
    }
    return retVal;
}

std::vector<std::string> onlinemoviedb::ListTheathers(void) const
{
    vector<string>retVal;
    for (auto& th : theathersVect)
    {
        retVal.push_back(th->GetName());
        cout << retVal.back() << endl;
    }
    return retVal;
}

std::vector<std::string> onlinemoviedb::ListTheathersForMovie(const std::string movieName) const
{
    vector<string>retVal;
    movie* pMovie = getMoviePtrFromName(movieName);

    if (pMovie)
    {
        // Movie exist
        retVal = pMovie->GetTheathersForThisMovie();
    }
    return retVal;
}

movie* onlinemoviedb::getMoviePtrFromName(const std::string movieName) const
{
    movie* retVal = nullptr;
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
        std::vector<int> vectorminusone(numRandomSeats, -1);
        retVal = pMovie->BookSeat(theatherName, vectorminusone);
    }
    return retVal;
}

/*int main()
{
    std::cout << "Hello World!\n";
}
*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
