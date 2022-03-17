#include "AirlineReservationSystem.h"

void AirlineReservationSystem::addPassenger(const std::string &firstname, const std::string &lastname) {
    
    Passenger * black = new Passenger(firstname, lastname);
    passengers.insert(*black) ;
    delete black ;
}

Passenger *AirlineReservationSystem::searchPassenger(const std::string &firstname, const std::string &lastname) {
    
    Passenger * pink = new Passenger(firstname, lastname);
    BSTNode<Passenger> * val ;
    val = passengers.search(*pink) ;
    delete pink ;
    return &(val -> data) ;
}

void AirlineReservationSystem::addFlight(const std::string &flightCode, const std::string &departureTime, const std::string &arrivalTime, const std::string &departureCity, const std::string &arrivalCity, int economyCapacity, int businessCapacity) {
    
    Flight * inspirational = new Flight(flightCode, departureTime, arrivalTime, departureCity, arrivalCity, economyCapacity, businessCapacity) ;
    flights.insert(*inspirational) ;
    delete inspirational ;
}

std::vector<Flight *> AirlineReservationSystem::searchFlight(const std::string &departureCity, const std::string &arrivalCity) {
    
    std::vector<Flight *> hehe ;
    search_flight_helper(hehe, flights.getRoot(), departureCity, arrivalCity) ;
    return hehe ;
}

void AirlineReservationSystem::issueTicket(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType) {
  
    Passenger* tell = searchPassenger(firstname, lastname) ;
    const std::string a = "does_it_matter" ;
    int b = 0 ;
    Flight* huh = new Flight(flightCode,a,a,a,a,b,b) ;
    Flight* me = & ( flights.search(*huh) -> data ); 
    if(!tell || !me) ;
    else
    {
        Ticket* why = new Ticket(tell, me, ticketType) ;
        me -> addTicket(*why) ;
        delete why ;
    }
    delete huh ;
}

void AirlineReservationSystem::saveFreeTicketRequest(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType) {
    
    Passenger* tell = searchPassenger(firstname, lastname) ;
    const std::string a = "does_it_matter" ;
    int b = 1 ;
    Flight* huh = new Flight(flightCode,a,a,a,a,b,b) ;
    Flight* me = & ( flights.search(*huh) -> data );
    if(!tell || !me) ;
    else
    {
        Ticket* why = new Ticket(tell, me, ticketType) ;
        freeTicketRequests.enqueue(*why) ;
        delete why ;
    }
    delete huh ;
}

void AirlineReservationSystem::executeTheFlight(const std::string &flightCode) {
    
    const std::string a = "does_it_matter" ;
    int b = 2 ;
    Flight* huh = new Flight(flightCode,a,a,a,a,b,b) ;
    Flight* me = & ( flights.search(*huh) -> data );
    if(!me) ;
    else
    {
        Queue<Ticket> backup ;
        Ticket wanting ;
        bool success = false;
        while(!freeTicketRequests.isEmpty())
        {
            wanting = freeTicketRequests.dequeue() ;
            if ( ( wanting.getFlight() ).getFlightCode() == flightCode ) 
                success = me -> addTicket(wanting) ;
            if(!success) backup.enqueue(wanting) ;
            success = false ; 
        }
        while(!backup.isEmpty())
        {
            freeTicketRequests.enqueue(backup.dequeue()) ;
        }
        me -> setCompleted(true) ;
    }
    delete huh ;
}

void AirlineReservationSystem::print() const {
    std::cout << "# Printing the airline reservation system ..." << std::endl;

    std::cout << "# Passengers:" << std::endl;
    passengers.print(inorder);

    std::cout << "# Flights:" << std::endl;
    flights.print(inorder);

    std::cout << "# Free ticket requests:" << std::endl;
    freeTicketRequests.print();

    std::cout << "# Printing is done." << std::endl;
}




void AirlineReservationSystem::search_flight_helper(std::vector<Flight*> &haha, BSTNode<Flight> * r, const std::string &departurecity, const std::string &arrivalcity) 
{
    if(!r) ;
    else
    {
        if( (r -> data).getDepartureCity() == departurecity && (r->data).getArrivalCity() == arrivalcity ) 
            haha.push_back(&(r -> data)) ;
        search_flight_helper(haha, r -> left, departurecity, arrivalcity) ;
        search_flight_helper(haha, r -> right, departurecity, arrivalcity) ;
    }
}



























