#include "Profile.h"

Profile::Profile(const std::string &email, const std::string &username, SubscriptionPlan plan) {
    this->email = email;
    this->username = username;
    this->plan = plan;
}

const std::string &Profile::getUsername() const {
    return this->username;
}

const std::string &Profile::getEmail() const {
    return this->email;
}

SubscriptionPlan Profile::getPlan() const {
    return this->plan;
}

LinkedList<Playlist> &Profile::getPlaylists() {
    return this->playlists;
}

LinkedList<Profile *> &Profile::getFollowings() {
    return this->following;
}

LinkedList<Profile *> &Profile::getFollowers() {
    return this->followers;
}

void Profile::setPlan(SubscriptionPlan plan) {
    this->plan = plan;
}

void Profile::followProfile(Profile *profile) {
    
    (this -> following).insertAtTheEnd(profile) ;
}

void Profile::unfollowProfile(Profile *profile) {
    
    (this -> following).removeNode(profile) ;
}

void Profile::createPlaylist(const std::string &playlistName) {
    
    Playlist *adele = new Playlist(playlistName) ;
    (this -> playlists).insertAtTheEnd(*adele) ;
    delete adele ;
}

void Profile::deletePlaylist(int playlistId) {
    
    int x = (this -> playlists).getSize() ;
    Node<Playlist> * ok = (this -> playlists).getFirstNode() ;
    for(int i = 0; i < x; i++)
    {
        if((ok -> data).getPlaylistId() == playlistId) { (this -> playlists).removeNode(ok) ; break; }
        ok = ok -> next ;
    }
}

void Profile::addSongToPlaylist(Song *song, int playlistId) {
    
    int x = (this -> playlists).getSize() ;
    Node<Playlist> * ok = (this -> playlists).getFirstNode() ;
    for(int i = 0; i < x; i++)
    {
        if((ok -> data).getPlaylistId() == playlistId) break;
        ok = ok -> next ;
    }
    ((ok -> data).getSongs()).insertAtTheEnd(song) ;
}

void Profile::deleteSongFromPlaylist(Song *song, int playlistId) {
    
    int x = (this -> playlists).getSize() ;
    Node<Playlist> * ok = (this -> playlists).getFirstNode() ;
    for(int i = 0; i < x; i++)
    {
        if((ok -> data).getPlaylistId() == playlistId) break;
        ok = ok -> next ;
    }
    ((ok -> data).getSongs()).removeNode(song) ;
}

Playlist *Profile::getPlaylist(int playlistId) {
    
    int x = (this -> playlists).getSize() ;
    Node<Playlist> * ok = (this -> playlists).getFirstNode() ;
    for(int i = 0; i < x; i++)
    {
        if((ok -> data).getPlaylistId() == playlistId) break; 
        ok = ok -> next ;
    }
    return &(ok -> data) ;
}

LinkedList<Playlist *> Profile::getSharedPlaylists() {
    
    int s = (this -> following).getSize() ;
    LinkedList<Playlist*> * lol = new LinkedList<Playlist*> ;
    Node<Profile*> * say_hi = (this -> following).getFirstNode() ;
    for(int i = 0; i < s; i++)
    {
        Node<Playlist> * hmm = ( (say_hi -> data) -> getPlaylists() ).getFirstNode() ;
        int s = (say_hi -> data) -> getPlaylists()).getSize() ;
        for(i = 0; i < s; i++)
        {
            if( (hmm -> data).isShared() ) lol -> insertAtTheEnd(&(hmm -> data)) ;
            hmm = hmm -> next ;
        }
        say_hi = say_hi -> next ;
    }
    return *lol ;
}

void Profile::shufflePlaylist(int playlistId, int seed) {
    
    Playlist* shake = this -> getPlaylist(playlistId) ;
    shake -> shuffle(seed) ;
}

void Profile::sharePlaylist(int playlistId) {
    
    Playlist* thanksgiving = this -> getPlaylist(playlistId) ;
    thanksgiving -> setShared(true) ;
}

void Profile::unsharePlaylist(int playlistId) {
    
    Playlist* thanksgiving = this -> getPlaylist(playlistId) ;
    thanksgiving -> setShared(false) ;
}

bool Profile::operator==(const Profile &rhs) const {
    return this->email == rhs.email && this->username == rhs.username && this->plan == rhs.plan;
}

bool Profile::operator!=(const Profile &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Profile &profile) {
    os << "email: " << profile.email << " |";
    os << " username: " << profile.username << " |";
    if (profile.plan == free_of_charge) {
        os << " plan: " << "free_of_charge" << " |";
    } else if (profile.plan == premium) {
        os << " plan: " << "premium" << " |";
    } else {
        os << " plan: " << "undefined" << " |";
    }

    os << " playlists: [";
    Node<Playlist> *firstPlaylistNode = profile.playlists.getFirstNode();
    Node<Playlist> *playlistNode = firstPlaylistNode;
    if (playlistNode) {
        do {
            os << playlistNode->data;
            if (playlistNode->next != firstPlaylistNode) os << ", ";
            playlistNode = playlistNode->next;
        } while (playlistNode != firstPlaylistNode);
    }
    os << "] |";
    os << " following: [";
    Node<Profile *> *firstProfileNode = profile.following.getFirstNode();
    Node<Profile *> *profileNode = firstProfileNode;
    if (profileNode) {
        do {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode) os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "] |";
    os << " followers: [";
    firstProfileNode = profile.followers.getFirstNode();
    profileNode = firstProfileNode;
    if (profileNode) {
        do {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode) os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "]";

    return os;
}
