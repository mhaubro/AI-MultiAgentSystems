/*
 * Location.h
 *
 *  Created on: 19 May 2018
 *      Author: martin
 */

#ifndef SEARCHER_SRC_LOCATION_H_
#define SEARCHER_SRC_LOCATION_H_

#include <utility>
#include <string>

class Location {
public:
	Location();
	Location(int x, int y);
	Location(const Location& location);

	std::pair<int,int> getLocation();
	int getX() const;
	int getY() const;

	void setLocation(Location loc);
	void setLocation(int x, int y);
	void setDLocation(Location loc);
	void setDLocation(int x, int y);

	double getDistance(Location otherLoc) const;
	int getManhattan(Location otherLoc) const;

	void setX(int x);
	void setY(int y);

	Location operator=(const Location& b);
	Location operator+(const Location& b) const;
	Location operator-(const Location& b) const;
	Location operator+=(const Location& b);
	Location operator-=(const Location& b);
	bool operator==(const Location& b) const;

	Location operator-();//Unary

	std::string toString() const;
private:
	int x;
	int y;

};

#endif /* SEARCHER_SRC_LOCATION_H_ */
