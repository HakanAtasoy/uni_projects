#include "MusicStream.h"

#include <iostream>

void MusicStream::addProfile(const std::string &email, const std::string &username, SubscriptionPlan plan) {
    Profile * please = new Profile(email, username, plan) ;
    (this -> profiles).insertAtTheEnd(*please) ;
    delete please ;
}

void MusicStream::deleteProfile(const std::string &email) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    Node<Profile*> * fling, *flwer ;
    int s = (this -> profiles).getSize(), s1 ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    
    flwer = ( (eyo -> data).getFollowers() ).getFirstNode() ;
    s1 = ( (eyo -> data).getFollowers() ).getSize() ;
    for(int i = 0; i < s1; i++)
    {
        flwer -> data -> unfollowProfile(&(eyo -> data)) ;
        flwer = flwer -> next ;
    }
    
    fling = ( (eyo -> data).getFollowings() ).getFirstNode() ;
    s1 = ( (eyo -> data).getFollowings() ).getSize() ;
    for(int i = 0; i < s1; i++)
    {
        ((fling -> data) -> getFollowers()).removeNode(&(eyo -> data)) ;
        fling = fling -> next ;
    }

    (this -> profiles).removeNode(eyo) ;

}

void MusicStream::addArtist(const std::string &artistName) {

    Artist * nt = new Artist(artistName) ;
    (this -> artists).insertAtTheEnd(*nt) ;
    delete nt ;
}

void MusicStream::addAlbum(const std::string &albumName, int artistId) {
    
    Album *t = new Album(albumName) ;
    Node<Artist> * fkths = (this -> artists).getFirstNode() ;
    int s = (this -> artists).getSize() ;
    for (int i = 0; i < s; i++)
    {
        if((fkths -> data).getArtistId() == artistId) break;
        fkths = fkths -> next ;
    }
    (this -> albums).insertAtTheEnd(*t) ;
    delete t ;
    t = & ( (this -> albums).getLastNode() ) -> data ;
    (fkths -> data).addAlbum(t) ;


}

void MusicStream::addSong(const std::string &songName, int songDuration, int albumId) {
    
   Song* pff = new Song(songName, songDuration) ;
   Node<Album> * fkths = (this -> albums).getFirstNode() ;
   int s = (this -> albums).getSize() ;
   for (int i = 0; i < s; i++)
   {
       if((fkths -> data).getAlbumId() == albumId)  break ;
       fkths = fkths -> next ;
   }
   (this -> songs).insertAtTheEnd(*pff) ;
   delete pff;
   pff = & ( ( this -> songs ).getLastNode() ) -> data ;
   (fkths -> data).addSong(pff) ;

}

void MusicStream::followProfile(const std::string &email1, const std::string &email2) {

    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo1, *eyo2 ;
    int s = (this -> profiles).getSize() ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email1 ) eyo1 = ok  ;
        if ( (ok -> data).getEmail() == email2 ) eyo2 = ok  ;
        ok = ok -> next ;
    }

    ( (eyo1 -> data) .getFollowings() ).insertAtTheEnd(&(eyo2 -> data)) ;
    ( (eyo2 -> data) .getFollowers() ).insertAtTheEnd(&(eyo1 -> data)) ;
}

void MusicStream::unfollowProfile(const std::string &email1, const std::string &email2) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo1, *eyo2 ;
    int s = (this -> profiles).getSize() ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email1 ) eyo1 = ok  ;
        if ( (ok -> data).getEmail() == email2 ) eyo2 = ok  ;
        ok = ok -> next ;
    }

    ( (eyo1 -> data) .getFollowings() ).removeNode(&(eyo2 -> data)) ;
    ( (eyo2 -> data) .getFollowers() ).removeNode(&(eyo1 -> data)) ;
}

void MusicStream::createPlaylist(const std::string &email, const std::string &playlistName) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode() ;
    Playlist* to_be_added = new Playlist(playlistName) ;
    int s = (this -> profiles).getSize() ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) break; 
        ok = ok -> next ;
    }

    ( (ok -> data).getPlaylists() ).insertAtTheEnd(*to_be_added) ;
    delete to_be_added ;

}

void MusicStream::deletePlaylist(const std::string &email, int playlistId) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode() ;
    int s = (this -> profiles).getSize(), s2 ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email )  break; 
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (ok-> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (ok -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }

    ( (ok -> data).getPlaylists() ).removeNode( heh ) ;
}

void MusicStream::addSongToPlaylist(const std::string &email, int songId, int playlistId) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode() ;
    int s = (this -> profiles).getSize(), s2, s3 ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) break; 
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (ok-> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (ok-> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }
    Node<Song> * ser = (this -> songs).getFirstNode() ;
    s3= (this -> songs).getSize() ;
    for(int i = 0; i < s3; i++)
    {
        if( (ser -> data).getSongId() == songId ) break;
        ser = ser -> next ;
    }
    ( (heh -> data).getSongs() ).insertAtTheEnd(&(ser -> data)) ;

}

void MusicStream::deleteSongFromPlaylist(const std::string &email, int songId, int playlistId) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    int s = (this -> profiles).getSize(), s2, s3 ;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (eyo -> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (eyo -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }
    Node<Song> * ser = (this -> songs).getFirstNode() ;
    s3= (this -> songs).getSize() ;
    for(int i = 0; i < s3; i++)
    {
        if( (ser -> data).getSongId() == songId ) break;
        ser = ser -> next ;
    }
    ( (heh -> data).getSongs() ).removeNode(&(ser -> data)) ;
}

LinkedList<Song *> MusicStream::playPlaylist(const std::string &email, Playlist *playlist) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode();
    LinkedList<Song*> result playlist -> songs ;
    int s = (this -> profiles).getSize();
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email )  break; 
        ok = ok -> next ;
    }

    if ( (ok -> data).getPlan() == 0 ) ;
    {
        result.insertAsEveryKthNode(&ADVERTISEMENT_SONG, 2) ;
    }
    return result ;

Playlist *MusicStream::getPlaylist(const std::string &email, int playlistId) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    int s = (this -> profiles).getSize(), s2;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (eyo -> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (eyo -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }
    return &(heh -> data) ;
}

LinkedList<Playlist *> MusicStream::getSharedPlaylists(const std::string &email) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    LinkedList<Playlist*> * same = new LinkedList<Playlist*> ;
    int s = (this -> profiles).getSize(), s2, s3;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }

    Node<Profile*> * yeah = ( (ok -> data).getFollowings() ).getFirstNode() ;
    s2 = ( (ok -> data).getFollowings() ).getSize() ;
    for(int i = 0; i < s2; i++)
    {
        Node<Playlist> * bob = ( (yeah -> data) -> getPlaylists() ).getFirstNode() ;
        s3 = ( (yeah -> data) -> getPlaylists() ).getSize() ;
        for(int i = 0; i < s3; i++)
        {
            if((bob -> data).isShared()) same -> insertAtTheEnd(&(bob -> data)) ;
            bob = bob -> next ;
        }
        yeah = yeah -> next ;
    }
}

void MusicStream::shufflePlaylist(const std::string &email, int playlistId, int seed) {

    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    int s = (this -> profiles).getSize(), s2;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (eyo -> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (eyo -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }

    (heh -> data).shuffle(seed) ;
}

void MusicStream::sharePlaylist(const std::string &email, int playlistId) {

    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    int s = (this -> profiles).getSize(), s2;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (eyo -> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (eyo -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }
    (heh -> data).setShared(true) ;
}

void MusicStream::unsharePlaylist(const std::string &email, int playlistId) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode(), *eyo ;
    int s = (this -> profiles).getSize(), s2;
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email ) { eyo = ok  ; break; }
        ok = ok -> next ;
    }
    Node<Playlist> * heh = ( (eyo -> data).getPlaylists() ).getFirstNode() ;
    s2 = ( (eyo -> data).getPlaylists() ).getSize() ;
    for(int i= 0; i < s2; i++)
    {
        if ( (heh -> data).getPlaylistId() == playlistId ) break ;
        heh = heh -> next ;
    }
    (heh -> data).setShared(false) ;
}

void MusicStream::subscribePremium(const std::string &email) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode();
    int s = (this -> profiles).getSize();
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email )  break; 
        ok = ok -> next ;
    }
    (ok -> data).setPlan(premium) ;
}

void MusicStream::unsubscribePremium(const std::string &email) {
    
    Node<Profile> * ok = (this -> profiles).getFirstNode();
    int s = (this -> profiles).getSize();
    for(int i = 0; i < s; i++)
    {
        if ( (ok -> data).getEmail() == email )  break; 
        ok = ok -> next ;
    }
    (ok -> data).setPlan(free_of_charge) ;
}

void MusicStream::print() const {
    std::cout << "# Printing the music stream ..." << std::endl;

    std::cout << "# Number of profiles is " << this->profiles.getSize() << ":" << std::endl;
    this->profiles.print();

    std::cout << "# Number of artists is " << this->artists.getSize() << ":" << std::endl;
    this->artists.print();

    std::cout << "# Number of albums is " << this->albums.getSize() << ":" << std::endl;
    this->albums.print();

    std::cout << "# Number of songs is " << this->songs.getSize() << ":" << std::endl;
    this->songs.print();

    std::cout << "# Printing is done." << std::endl;
}
