#pragma once

#include <iostream>
#include <set>
#include <vector>

struct Counter {
	Counter(int rssi, int counter) : rssi(rssi), counter(counter){};
	int rssi{0};
	mutable int counter{0};
	bool operator<(const Counter &rhs) const {
		return this->rssi < rhs.rssi;
	}
};

class Beacons {
public:

  Beacons(int interval) : interval(interval){};
	void add(int rssi) {
		auto result_ = beacons.insert(Counter(round_to(rssi, interval), 1));
		if (!result_.second) {
			(*result_.first).counter++;
		}
	}

	void print_me() {

        Serial.printf("rssi : count\n");
		for (auto beacon : beacons) {
            Serial.printf("%d   : %d   \n", beacon.rssi, beacon.counter);
		}
	}

    void clear() {
        beacons.clear();
    }

private:
  int interval{0};
	std::set<Counter> beacons{};

	int round_to(int value, int rounder) {
		if (value % rounder == 0) {
			return value;
		} else {
			return value + (rounder - (value % rounder));
		}
	}
};

