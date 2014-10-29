// $Author: benine $
// $Date$
// $Log$
// Contains the Contig class for afin

#include "contig.hpp"
#include "process.hpp"

using namespace std;

////////// Contig FUNCTIONS ////////////
Contig::Contig(){};

Contig::Contig( Readlist *reads, string str, string id ) : reads(reads), contig(str), contig_id(id){
  cov = 0;
  doub_cov = false;
  extension = new Extension( reads, extend_len );
}

Contig::~Contig(){
  delete extension;
}
    
// copy, move, =
Contig::Contig( const Contig& rhs ) : reads(rhs.reads), contig(rhs.contig), contig_id(rhs.contig_id), cov(rhs.cov), doub_cov(rhs.doub_cov){
  extension = new Extension( reads, extend_len );
  extension->matches = rhs.extension->matches;
  extension->ATCG = rhs.extension->ATCG;
  extension->missed_bp = rhs.extension->missed_bp;
  extension->missed_bp_tot = rhs.extension->missed_bp_tot;
  extension->missed_bp_avg = rhs.extension->missed_bp_avg;
  extension->start = rhs.extension->start;
  extension->back = rhs.extension->back;
  extension->contig = rhs.extension->contig;
  extension->exten_seq = rhs.extension->exten_seq;
  extension->pos_mult = rhs.extension->pos_mult;
} 

Contig::Contig( Contig&& rhs ) : Contig(){
  swap( *this, rhs );
}

Contig& Contig::operator=( Contig rhs ){
  swap( *this, rhs );
  return *this;
}

void swap( Contig& data1, Contig& data2 ){
  using std::swap;

  swap( data1.extension, data2.extension );
  swap( data1.reads, data2.reads );
  swap( data1.contig, data2.contig );
  swap( data1.contig_id, data2.contig_id );
  swap( data1.cov, data2.cov );
  swap( data1.doub_cov, data2.doub_cov );
}

// return contig_id
string Contig::get_sequence(){
  return contig;
}

string Contig::get_contig_id(){
  return contig_id;
}

// return cov
double Contig::get_cov(){
  return cov;
}

// set cov
void Contig::set_cov( int cov ){
  this->cov = cov;
}

// extend performs loops iterations of get_extension with length extend_len of each extension, at each iteration the extension is added to contig, and uses contig_sub_len characters from the front or back of the contig, which end is determined by the boolean value of back
void Contig::extend( bool back ){
  string exten_seq("");
  string contig_sub("");
 
  // skip over any contigs that present at least double coverage
  if( doub_cov ){
    return;
  }

  // get extension sequence through get_extension
  if( back ){
    contig_sub = contig.substr( contig.length() - ( contig_sub_len ) );
  }
  else{
    contig_sub = contig.substr( 0, contig_sub_len );
  }

  exten_seq = extension->get_extension( contig_sub, back );
  
  if( exten_seq.length() == 0 ){
    return;
  }

  if( back ){
    contig.append( exten_seq );
  }
  else{
    contig.insert( 0, exten_seq );
  }
}

// set doub_cov var
void Contig::set_doub_cov( bool doub_cov ){
  this->doub_cov = doub_cov;
}
