
#include "onlinemoviedb.h"
#ifndef PRODUCTION
#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <thread>


BOOST_AUTO_TEST_CASE(myTestCase)
{
  auto& db = onlinemoviedb::getdatabase("testdatabase.csv");
  auto movies = db.ListMovies();
  auto theathers = db.ListTheathers();
  
  // Test 1. 
  // Check that there are 7 movies in total
  BOOST_TEST(movies.size() == 7);

  // Test 2. 
  // Check there are 3 theathers in total
  BOOST_TEST(theathers.size() == 3);

  // Test 3. 
  // Check that the following movies exist
  std::vector<std::string> checkmovies{ "the lion king","batman","little marmaid", "superman", "mad max" };
  for (auto moviestring : checkmovies)
  {
      auto result = std::find(movies.begin(), movies.end(), moviestring) != movies.end();
      BOOST_TEST(result);
  }

  // Test 4. 
  // Check that "Dumbo" (not part of the database) is not displayed in any theather
  std::string dumbo = "Dumbo";
  auto dumboresults = db.ListTheathersForMovie(dumbo);
  BOOST_TEST(dumboresults.size() == 0);

  // Test 5.
  // Check that batman is displayed in london and fleet
  std::string batman = "batman";
  auto batmanresults = db.ListTheathersForMovie(batman);
  BOOST_TEST(batmanresults.size() == 2);
  std::vector<std::string> checktheathers{"london","fleet"};
  for (auto thstring : checktheathers)
  {
      auto result = std::find(batmanresults.begin(), batmanresults.end(), thstring) != batmanresults.end();
      BOOST_TEST(result);
  }

  // Test 6.
  // Check that when booking a seat randomly, then this seat is not available
  std::string movieBatman = "batman";
  std::string theatherBatman = "fleet";
  auto listAvailableBatman = db.ListAvailableSeats(movieBatman, theatherBatman);
  auto seatbooked = db.BookSeat(movieBatman, theatherBatman,1);
  BOOST_TEST(seatbooked[0] == 12); // First randomly generated seat is 12
  std::vector<int> randomseatvector{ seatbooked[0] };
  seatbooked = db.BookSeat(movieBatman, theatherBatman, randomseatvector);
  BOOST_TEST(seatbooked.size() == 0);

  // Test 7.
  // Check that going outside seat limits doesn't generate any booking
  std::vector<int> invalidSeat{ 22 };
  seatbooked = db.BookSeat(movieBatman, theatherBatman, invalidSeat);
  BOOST_TEST(seatbooked.size() == 0);
  invalidSeat[0] = -4;
  seatbooked = db.BookSeat(movieBatman, theatherBatman, invalidSeat);
  BOOST_TEST(seatbooked.size() == 0);

  // Test 8.
  // Randomly generating seats. Since one has been allocated only 19 remaining
  seatbooked = db.BookSeat(movieBatman, theatherBatman, 20);
  BOOST_TEST(seatbooked.size() == 19);
  seatbooked = db.BookSeat(movieBatman, theatherBatman, 2);
  BOOST_TEST(seatbooked.size() == 0);

  // Test 9.
  // Let's do some multithreading.  Threads accessing the API and booking concurrently
  // They all want to book tickets for x-men in farnborough
  // Test that among all of them they book all possible tickets that is 20
  // Also test that none of them has booked them all, that proves there was concurrent
  // access to the data structures
  auto threadfunction = [](std::vector<int>&results,int thnumb) 
  {
      auto& db = onlinemoviedb::getdatabase("");
      results[thnumb] = 0;
      while (true)
      {
          auto booked = db.BookSeat("x-men", "farnborough", 1);
          results[thnumb] += booked.size();
          if (db.ListAvailableSeats("x-men", "farnborough").size() == 0)
          {
              break;
          }
      }
  };
  static const int numbercontestingthreads = 8;

  std::vector<int> numberBookedSeats;
  std::vector<std::thread> threadarray;
  for (int i = 0; i < numbercontestingthreads; i++)
  {
      numberBookedSeats.push_back(0);
      threadarray.push_back(std::thread(threadfunction,std::ref(numberBookedSeats),i ));
  }
  for (auto& th : threadarray)
  {
      th.join();
  }
  for (auto res : numberBookedSeats)
  {
      std::cout << "Thread booked: " << res << " seats" << std::endl;
  }
  BOOST_TEST(std::accumulate(numberBookedSeats.begin(), numberBookedSeats.end(), 0) == 20);
  for (auto number : numberBookedSeats)
  {
      BOOST_TEST(number != 20);
  }
}
#endif