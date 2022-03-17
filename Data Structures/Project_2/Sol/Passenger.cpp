#include "Passenger.h"

Passenger::Passenger(std::string firstname, std::string lastname) {
    this->firstname = firstname;
    this->lastname = lastname;
}

const std::string &Passenger::getFirstname() const {
    return firstname;
}

const std::string &Passenger::getLastname() const {
    return lastname;
}

bool Passenger::operator<(const Passenger &rhs) const {

    const std::string pas1, pas2 ;
    if(this -> getLastname() == rhs.getLastname())
        pas1 = this -> getFirstname() , pas2 = rhs.getFirstname() ;
    else
        pas1 = this -> getLastname() , pas2 = rhs.getLastname() ;
    int a, b ;
    
    for(int i = 0, a=pas1[0], b = pas2[0]; a; i++)
    {
        if(!b) return false ;
        else 
        {
            a = pas1[i] ;
            b = pas2[i] ;
            if(a < b) return true ;
            if(a > b) return false
        }   
    }
    return true ;
}

bool Passenger::operator>(const Passenger &rhs) const {
    return rhs < *this;
}

bool Passenger::operator==(const Passenger &rhs) const {
    return firstname == rhs.firstname && lastname == rhs.lastname;
}

bool Passenger::operator!=(const Passenger &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Passenger &passenger) {
    os << "Passenger(firstname: " << passenger.firstname;
    os << ", lastname: " << passenger.lastname << ")";
    return os;
}
