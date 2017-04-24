/**
 * algorithm EfficientClosestPair from Levitin 3rd p194
 * closest pair by divide and conquer
 * @author Zorig Magnaituvshin
 * @version 4 March 2017
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <cfloat>

using namespace std;

typedef unsigned int uint;

/**
 * Make a class that sorts ascending by the first element of a pair
 */
struct SortFirst
{
  bool operator()(const pair<double,double> & left,
                  const pair<double,double> & right)
  {
    if( left.first == right.first )
      return left.second < right.second;
    return left.first < right.first;
  }
};

/**
 * Make a class that sorts ascending by the second element of a pair
 */
struct SortSecond
{
  bool operator()(const pair<double,double> & left,
                  const pair<double,double> & right)
  {
    if( left.second == right.second )
      return left.first < right.first;
    return left.second < right.second;
  }
};

/**
 * simple cartesian distance between two points imlemented
 * as pairs of doubles
 */
double distance( const pair<double,double> & p1,
                 const pair<double,double> & p2 )
{
  double diff_xs = p1.first - p2.first;
  double diff_ys = p1.second - p2.second;
  return sqrt( diff_xs * diff_xs + diff_ys * diff_ys );
}

/**
 * divide and conquer closest pair
 * if the number of elements is less than 4, solve by brute force
 * @param p a vector of points modeled as pairs of doubles, in
 * ascending order by the x coordinate
 * @param q a vector of points modeled as pairs of doubles, in
 * ascending order by the y coordinate
 * @return the Cartesian distance between the closest pair of points
 */
double dc_close_pair_dist( const vector<pair<double,double>> & p,
                           const vector<pair<double,double>> & q, uint & dc_basic_op )
{
  if( p.size() < 2 )
    return 0.0;
  if( p.size() == 2 )
    return distance( p.at( 0 ), p.at( 1 ));
  if( p.size() == 3 )
  {
    double dist01 = distance( p.at( 0 ), p.at( 1 ));
    double dist02 = distance( p.at( 0 ), p.at( 2 ));
    double dist12 = distance( p.at( 1 ), p.at( 2 ));
    double min01 = dist01 < dist02 ? dist01 : dist02;
    return min01 < dist12 ? min01 : dist12;
  }

  vector<pair<double,double>> pl;
  vector<pair<double,double>> pr;
  vector<pair<double,double>> ql;
  vector<pair<double,double>> qr;

  for( uint i = 0; i < p.size() / 2; i++ )
  {
    pl.push_back( p.at( i ));
    ql.push_back( p.at( i ));
    dc_basic_op++; //dc basic operation
  }

  for( uint i = p.size() / 2; i < p.size(); i++ )
  {
    pr.push_back( p.at( i ));
    qr.push_back( p.at( i ));
    dc_basic_op++; //dc basic operation
  }

  // pl and pr and in order, but need to sort ql and qr
  sort( ql.begin(), ql.end(), SortSecond() );
  sort( qr.begin(), qr.end(), SortSecond() );

  double dl = dc_close_pair_dist( pl, ql, dc_basic_op );
  double dr = dc_close_pair_dist( pr, qr, dc_basic_op );
  double d = min( dl, dr );
  double m = p.at( p.size() / 2 - 1 ).first;

  vector<pair<double,double>> s;
  for( auto point : q )
  {
    if( abs( point.first - m ) < d )
    {
      s.push_back( point );
    }
  }

  double dminsq = d * d;
  if( s.size() == 0 )
  {
    return sqrt( dminsq );
  }

  for( uint i = 0; i < s.size() - 1; i++ )
  {
    uint k = i + 1;
    dc_basic_op++;
    while( k < s.size() &&
           pow( s.at( k ).second - s.at( i ).second, 2.0 ) < dminsq )
    {
      dminsq = min( pow( s.at( k ).first - s.at( i ).first, 2.0 ) +
                    pow( s.at( k ).second - s.at( i ).second, 2.0 ),
                    dminsq );
      k++;
    }
  }
  return sqrt( dminsq );
}

double br_close_pair_dist( const vector<pair<double,double>> & arr, uint & br_basic_op )
{
    if(arr.size() < 2)
    {
        return 0.0;
    }
    
    if(arr.size() == 2)
    {
        return distance( arr.at( 0 ), arr.at( 1 ));
    }
    
    if( arr.size() == 3 )
    {
    double dist01 = distance( arr.at( 0 ), arr.at( 1 ));
    double dist02 = distance( arr.at( 0 ), arr.at( 2 ));
    double dist12 = distance( arr.at( 1 ), arr.at( 2 ));
    double min01 = dist01 < dist02 ? dist01 : dist02;
    
    return min01 < dist12 ? min01 : dist12;
    }
    
    double min = DBL_MAX;
    
    for( uint i = 0; i < arr.size() - 1; i++)
    {
        for( uint j = i + 1; j < arr.size(); j++)
        {
            br_basic_op++;
            if( distance( arr.at( i ), arr.at( j )) < min )
            {
                min = distance(arr.at( i ), arr.at(j));
            }
        }
        
    }
    return min;
    
}


/*
 * standard input of points must be of the form
 * x-value y-value
 * x-value y-value
 * ...
 */
int main()
{
  vector<pair<double, double>> p; // Levitin's P array
  vector<pair<double, double>> q;// Levitin's Q array
  vector<pair<double, double>> myarray;
  uint br_basic_op = 0;
  uint dc_basic_op = 0;
  uint input_size = 0;

  while( !cin.eof() )
  {
    double xvalue;
    double yvalue;

    cin >> xvalue >> yvalue;
    if( !cin.eof() )
    {
      p.push_back( make_pair( xvalue, yvalue ));
      q.push_back( make_pair( xvalue, yvalue ));
      myarray.push_back( make_pair( xvalue, yvalue));
      input_size++;
    }
  }
  sort( p.begin(), p.end(), SortFirst() );
  sort( q.begin(), q.end(), SortSecond() );

  double mindist = dc_close_pair_dist( p, q, dc_basic_op );
  double mindistBR = br_close_pair_dist( myarray, br_basic_op ); // set to something
  
  cout << "Miniumum dc distance: " << mindist << endl;
  cout << "Miniumum brute force distance: " << mindistBR << endl;
  cerr << input_size <<  "\t" << br_basic_op << "\t" << dc_basic_op<< endl;
  
  return 0;
}
