/*
 * time.hpp
 *
 * Time utilities, adapted from Timer.h by Christian Conti on 10/1/13.
 * Copyright 2013 ETH Zurich. All rights reserved.
 */
#ifndef TIME_HPP
#define TIME_HPP

#include <sys/time.h>
#include <iostream>

class Timer
{
	struct timeval t_start, t_end, t_tmp;
	struct timezone t_zone;
	
public:
    Timer(){ gettimeofday(&t_start,  &t_zone); }
	
	void start(){
        gettimeofday(&t_start,  &t_zone);
    }
	
	double stop() {
		gettimeofday(&t_end,  &t_zone);
		return (t_end.tv_usec  - t_start.tv_usec)*1e-6  + (t_end.tv_sec  - t_start.tv_sec);
	}

    double read() {
		gettimeofday(&t_tmp,  &t_zone);
		return (t_tmp.tv_usec  - t_start.tv_usec)*1e-6  + (t_tmp.tv_sec  - t_start.tv_sec);
	}

    void print() {
        std::cout << "t = " << read() << "s\n";
    }
};
