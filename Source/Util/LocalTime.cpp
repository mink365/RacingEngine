//
//  LocalTime.cpp
//  MagicEra
//
//  Created by linzi on 12-12-10.
//
//

#include <ctime>
#include "LocalTime.h"

LocalTime::LocalTime() {
    
}

long LocalTime::getCurrentTime() {
    return time(NULL);
}
