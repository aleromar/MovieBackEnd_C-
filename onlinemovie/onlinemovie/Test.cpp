
#include "onlinemoviedb.h"
#ifndef PRODUCTION
#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(myTestCase)
{
  auto& db = onlinemoviedb::getdatabase("testdatabase.csv");
  auto movies = db.ListMovies();
  auto theathers = db.ListTheathers();
  
  // Test 1. Check that there are 7 movies
  BOOST_TEST(movies.size() == 7);

  // Test 2. Check there are 3 theathers
  BOOST_TEST(theathers.size() == 3);

  // Test 3. Check that the following movies exist
  std::vector<std::string> checkmovies{ "the lion king","batman","little marmaid", "superman", "mad max" };
  for (auto moviestring : checkmovies)
  {
      auto result = std::find(movies.begin(), movies.end(), moviestring) != movies.end();
      BOOST_TEST(result);
  }

  // Test 4. Check that "Dumbo" is not displayed in any theather
  std::string dumbo = "Dumbo";
  auto dumboresults = db.ListTheathersForMovie(dumbo);
  BOOST_TEST(dumboresults.size() == 0);

  // Test 5 - Check that batman is displayed in london and fleet
  std::string batman = "batman";
  auto batmanresults = db.ListTheathersForMovie(batman);
  BOOST_TEST(batmanresults.size() == 2);
  std::vector<std::string> checktheathers{"london","fleet"};
  for (auto thstring : checktheathers)
  {
      auto result = std::find(batmanresults.begin(), batmanresults.end(), thstring) != batmanresults.end();
      BOOST_TEST(result);
  }

  // Test 6- - Check that when booking a seat randomly, then this seat is not available
  std::string movieBatman = "batman";
  std::string theatherBatman = "fleet";
  auto listAvailableBatman = db.ListAvailableSeats(movieBatman, theatherBatman);
  auto seatbooked = db.BookSeat(movieBatman, theatherBatman,1);
  BOOST_TEST(seatbooked[0] == 12); // First randomly generated seat is 12
  std::vector<int> randomseatvector{ seatbooked[0] };
  seatbooked = db.BookSeat(movieBatman, theatherBatman, randomseatvector);
  BOOST_TEST(seatbooked.size() == 0);

  // Test 7 - Check that going outside seat limits doesn't generate any booking
  std::vector<int> invalidSeat{ 22 };
  seatbooked = db.BookSeat(movieBatman, theatherBatman, invalidSeat);
  BOOST_TEST(seatbooked.size() == 0);
  invalidSeat[0] = -4;
  seatbooked = db.BookSeat(movieBatman, theatherBatman, invalidSeat);
  BOOST_TEST(seatbooked.size() == 0);

  // Test 8 - Randomly generating
  seatbooked = db.BookSeat(movieBatman, theatherBatman, 20);
  BOOST_TEST(seatbooked.size() == 19);
  seatbooked = db.BookSeat(movieBatman, theatherBatman, 2);
  BOOST_TEST(seatbooked.size() == 0);

  BOOST_TEST(1 == 1);
  BOOST_TEST(true);
}
#endif