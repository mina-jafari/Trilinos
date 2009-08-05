// @HEADER
// ************************************************************************
//
//                           Intrepid Package
//                 Copyright (2007) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Pavel Bochev (pbboche@sandia.gov) or
//                    Denis Ridzal (dridzal@sandia.gov) or
//                    Robert Kirby (robert.c.kirby@ttu.edu)
//
// ************************************************************************
// @HEADER


/** \file
\brief  Unit test of orthogonal tetrahedral polynomial basis class
\author Created by R. Kirby
*/

#include "Intrepid_FieldContainer.hpp"
#include "Teuchos_oblackholestream.hpp"
#include "Teuchos_RCP.hpp"
#include "Teuchos_GlobalMPISession.hpp"
#include "Intrepid_HGRAD_TET_Cn_FEM_ORTH.hpp"
#include "Intrepid_CubatureDirectTetDefault.hpp"
#include "Shards_CellTopology.hpp"
#include "Intrepid_PointTools.hpp"

#include <iostream>

using namespace Intrepid;

int main(int argc, char *argv[]) {

  Teuchos::GlobalMPISession mpiSession(&argc, &argv);
  
  // This little trick lets us print to std::cout only if a (dummy) command-line argument is provided.
  int iprint     = argc - 1;
  
  Teuchos::RCP<std::ostream> outStream;
  Teuchos::oblackholestream bhs; // outputs nothing
  
  if (iprint > 0)
    outStream = Teuchos::rcp(&std::cout, false);
  else
    outStream = Teuchos::rcp(&bhs, false);
  
  // Save the format state of the original std::cout.
  Teuchos::oblackholestream oldFormatState;
  oldFormatState.copyfmt(std::cout);
  
  *outStream \
    << "===============================================================================\n" \
    << "|                                                                             |\n" \
    << "|                           Unit Test OrthogonalBases                         |\n" \
    << "|                                                                             |\n" \
    << "|     1) Tests orthogonality of tetrahedral orthogonal basis                  |\n" \
    << "|                                                                             |\n" \
    << "|  Questions? Contact  Pavel Bochev (pbboche@sandia.gov) or                   |\n" \
    << "|                      Denis Ridzal (dridzal@sandia.gov) or                   |\n" \
    << "|                      Robert Kirby (robert.c.kirby@ttu.edu)                  |\n" \
    << "|                                                                             |\n" \
    << "|  Intrepid's website: http://trilinos.sandia.gov/packages/intrepid           |\n" \
    << "|  Trilinos website:   http://trilinos.sandia.gov                             |\n" \
    << "|                                                                             |\n" \
    << "===============================================================================\n";
  
  int errorFlag  = 0;

  const int deg = 3;

  Basis_HGRAD_TET_Cn_FEM_ORTH<double,FieldContainer<double> > myBasis( deg );
  const int polydim = myBasis.getCardinality();
  
  // First, get a reference quadrature rule

  CubatureDirectTetDefault<double,FieldContainer<double> > myCub(2*deg);
  FieldContainer<double> cubPts( myCub.getNumPoints() , 3 );
  FieldContainer<double> cubWts( myCub.getNumPoints() );

  myCub.getCubature( cubPts , cubWts );


  // Tabulate the basis functions at the cubature points
  FieldContainer<double> basisAtCubPts( polydim , myCub.getNumPoints() );

  myBasis.getValues( basisAtCubPts , cubPts , OPERATOR_VALUE );


  // Now let's compute the mass matrix
  for (int i=0;i<polydim;i++) {
    for (int j=i;j<polydim;j++) {
      double cur = 0.0;
      for (int k=0;k<myCub.getNumPoints();k++) {
	cur += cubWts(k) * basisAtCubPts( i , k ) * basisAtCubPts( j , k );
      }
      if (i != j && fabs( cur ) > 100. * INTREPID_TOL) {
	std::cout << "not diagonal" << i << " " << j << " " << fabs( cur ) << std::endl;
	errorFlag++;
      }
      if (i == j && fabs( cur ) <= 100. * INTREPID_TOL) {
        std::cout << "zero on diagonal" << i << " " << j << std::endl;
      }
    }
  }

  
  // compare the points against FIAT-tabulated values on a lattice 
  shards::CellTopology myTet_4( shards::getCellTopologyData< shards::Tetrahedron<4> >() );  
  const int np_lattice = PointTools::getLatticeSize( myTet_4 , deg , 0 );
  FieldContainer<double> lattice( np_lattice , 3 );
  PointTools::getLattice<double,FieldContainer<double> >( lattice , 
                                                          myTet_4 , 
                                                          deg , 
                                                          0 , 
                                                          POINTTYPE_EQUISPACED );        
                                 
  FieldContainer<double> dBasisAtLattice( polydim , np_lattice , 3 );
  myBasis.getValues( dBasisAtLattice , lattice , OPERATOR_D1 );

  double fiat_vals[] = {
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.999999999999999e-01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    4.000000000000000e+00,
    -6.000000000000000e+00,
    -2.000000000000000e+00,
    -2.000000000000000e+00,
    -2.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.000000000000000e+00,
    2.000000000000000e+00,
    2.000000000000000e+00,
    6.000000000000000e+00,
    4.000000000000000e+00,
    4.000000000000000e+00,
    -4.000000000000000e+00,
    -1.333333333333333e+00,
    -1.333333333333333e+00,
    -3.330669073875470e-16,
    6.666666666666665e-01,
    6.666666666666665e-01,
    3.999999999999999e+00,
    2.666666666666666e+00,
    2.666666666666666e+00,
    -2.000000000000000e+00,
    -6.666666666666667e-01,
    -6.666666666666667e-01,
    2.000000000000000e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -4.000000000000000e+00,
    -1.333333333333333e+00,
    -1.333333333333333e+00,
    -3.330669073875470e-16,
    6.666666666666665e-01,
    6.666666666666665e-01,
    3.999999999999999e+00,
    2.666666666666666e+00,
    2.666666666666666e+00,
    -2.000000000000000e+00,
    -6.666666666666666e-01,
    -6.666666666666666e-01,
    2.000000000000000e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    -3.330669073875470e-16,
    -1.110223024625157e-16,
    -1.110223024625157e-16,
    -2.000000000000000e+00,
    -6.666666666666667e-01,
    -6.666666666666667e-01,
    2.000000000000000e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    -3.330669073875470e-16,
    -1.110223024625157e-16,
    -1.110223024625157e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -2.000000000000000e+00,
    -6.000000000000000e+00,
    -2.000000000000000e+00,
    -2.000000000000000e+00,
    -2.666666666666667e+00,
    -1.333333333333333e+00,
    -2.000000000000000e+00,
    6.666666666666663e-01,
    -6.666666666666667e-01,
    -2.000000000000000e+00,
    4.000000000000000e+00,
    0.000000000000000e+00,
    1.333333333333333e+00,
    -2.666666666666667e+00,
    -1.110223024625157e-16,
    1.333333333333333e+00,
    6.666666666666663e-01,
    6.666666666666665e-01,
    1.333333333333333e+00,
    3.999999999999999e+00,
    1.333333333333333e+00,
    4.666666666666666e+00,
    6.666666666666661e-01,
    2.000000000000000e+00,
    4.666666666666666e+00,
    3.999999999999999e+00,
    2.666666666666666e+00,
    8.000000000000000e+00,
    4.000000000000000e+00,
    4.000000000000000e+00,
    -1.333333333333333e+00,
    -4.000000000000001e+00,
    -1.333333333333333e+00,
    -1.333333333333333e+00,
    -6.666666666666670e-01,
    -6.666666666666667e-01,
    -1.333333333333333e+00,
    2.666666666666666e+00,
    -2.220446049250313e-16,
    2.000000000000000e+00,
    -6.666666666666670e-01,
    6.666666666666666e-01,
    2.000000000000000e+00,
    2.666666666666666e+00,
    1.333333333333333e+00,
    5.333333333333333e+00,
    2.666666666666666e+00,
    2.666666666666667e+00,
    -6.666666666666667e-01,
    -2.000000000000000e+00,
    -6.666666666666667e-01,
    -6.666666666666667e-01,
    1.333333333333333e+00,
    -1.110223024625157e-16,
    2.666666666666667e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -7.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -3.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    9.999999999999996e-01,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    5.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -5.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -1.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    2.999999999999999e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -3.000000000000000e+00,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    9.999999999999996e-01,
    -2.000000000000000e+00,
    -1.000000000000000e+00,
    -1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    -3.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    9.999999999999997e-01,
    2.000000000000000e+00,
    1.000000000000000e+00,
    4.999999999999999e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    -1.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    3.000000000000000e+00,
    2.000000000000000e+00,
    1.000000000000000e+00,
    9.999999999999997e-01,
    6.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+00,
    6.000000000000000e+00,
    3.000000000000000e+00,
    5.000000000000000e+00,
    6.000000000000000e+00,
    3.000000000000000e+00,
    3.000000000000000e+00,
    1.000000000000000e+01,
    5.000000000000000e+00,
    5.000000000000000e+00,
    -0.000000000000000e+00,
    -8.000000000000000e+00,
    -2.000000000000000e+00,
    -0.000000000000000e+00,
    -8.000000000000000e+00,
    -2.000000000000000e+00,
    -0.000000000000000e+00,
    -7.999999999999999e+00,
    -2.000000000000000e+00,
    -0.000000000000000e+00,
    -8.000000000000000e+00,
    -2.000000000000000e+00,
    0.000000000000000e+00,
    -1.333333333333334e+00,
    6.666666666666665e-01,
    0.000000000000000e+00,
    -1.333333333333334e+00,
    6.666666666666665e-01,
    0.000000000000000e+00,
    -1.333333333333334e+00,
    6.666666666666665e-01,
    0.000000000000000e+00,
    5.333333333333332e+00,
    3.333333333333333e+00,
    0.000000000000000e+00,
    5.333333333333332e+00,
    3.333333333333333e+00,
    0.000000000000000e+00,
    1.200000000000000e+01,
    6.000000000000000e+00,
    -0.000000000000000e+00,
    -5.333333333333334e+00,
    -1.333333333333333e+00,
    -0.000000000000000e+00,
    -5.333333333333334e+00,
    -1.333333333333333e+00,
    -0.000000000000000e+00,
    -5.333333333333334e+00,
    -1.333333333333333e+00,
    0.000000000000000e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    0.000000000000000e+00,
    1.333333333333333e+00,
    1.333333333333333e+00,
    0.000000000000000e+00,
    8.000000000000000e+00,
    4.000000000000000e+00,
    -0.000000000000000e+00,
    -2.666666666666667e+00,
    -6.666666666666666e-01,
    -0.000000000000000e+00,
    -2.666666666666667e+00,
    -6.666666666666666e-01,
    0.000000000000000e+00,
    3.999999999999999e+00,
    2.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -0.000000000000000e+00,
    -3.000000000000000e+00,
    -7.000000000000000e+00,
    -0.000000000000000e+00,
    -3.000000000000000e+00,
    -7.000000000000000e+00,
    -0.000000000000000e+00,
    -3.000000000000000e+00,
    -6.999999999999999e+00,
    -0.000000000000000e+00,
    -3.000000000000000e+00,
    -7.000000000000000e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    -1.000000000000000e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    -1.000000000000000e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    -1.000000000000000e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    4.999999999999999e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    4.999999999999999e+00,
    0.000000000000000e+00,
    -3.000000000000000e+00,
    1.100000000000000e+01,
    0.000000000000000e+00,
    3.000000000000000e+00,
    -3.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    -3.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    -3.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    3.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    3.000000000000000e+00,
    0.000000000000000e+00,
    3.000000000000000e+00,
    9.000000000000000e+00,
    0.000000000000000e+00,
    8.999999999999998e+00,
    9.999999999999996e-01,
    0.000000000000000e+00,
    8.999999999999998e+00,
    9.999999999999996e-01,
    0.000000000000000e+00,
    8.999999999999998e+00,
    6.999999999999998e+00,
    0.000000000000000e+00,
    1.500000000000000e+01,
    5.000000000000000e+00,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -9.999999999999998e+00,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -9.999999999999998e+00,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -9.999999999999998e+00,
    -0.000000000000000e+00,
    -0.000000000000000e+00,
    -1.000000000000000e+01,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -6.661338147750939e-16,
    0.000000000000000e+00,
    0.000000000000000e+00,
    9.999999999999996e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    9.999999999999996e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    9.999999999999996e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.000000000000000e+01,
    1.200000000000000e+01,
    3.000000000000000e+00,
    3.000000000000000e+00,
    -1.333333333333333e+00,
    -1.666666666666667e+00,
    -1.666666666666667e+00,
    -1.333333333333334e+00,
    3.333333333333327e-01,
    3.333333333333327e-01,
    1.200000000000000e+01,
    9.000000000000000e+00,
    9.000000000000000e+00,
    5.333333333333335e+00,
    1.333333333333334e+00,
    1.333333333333334e+00,
    -1.333333333333333e+00,
    -6.666666666666670e-01,
    -6.666666666666670e-01,
    5.333333333333331e+00,
    3.999999999999999e+00,
    3.999999999999999e+00,
    1.333333333333334e+00,
    3.333333333333335e-01,
    3.333333333333335e-01,
    1.333333333333333e+00,
    9.999999999999997e-01,
    9.999999999999997e-01,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    5.333333333333335e+00,
    1.333333333333334e+00,
    1.333333333333334e+00,
    -1.333333333333333e+00,
    -6.666666666666670e-01,
    -6.666666666666670e-01,
    5.333333333333331e+00,
    3.999999999999999e+00,
    3.999999999999999e+00,
    1.333333333333334e+00,
    3.333333333333334e-01,
    3.333333333333334e-01,
    1.333333333333333e+00,
    9.999999999999997e-01,
    9.999999999999997e-01,
    3.697785493223493e-32,
    9.244463733058732e-33,
    9.244463733058732e-33,
    1.333333333333334e+00,
    3.333333333333335e-01,
    3.333333333333335e-01,
    1.333333333333333e+00,
    9.999999999999997e-01,
    9.999999999999997e-01,
    3.697785493223493e-32,
    9.244463733058732e-33,
    9.244463733058732e-33,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    6.000000000000000e+00,
    9.000000000000000e+00,
    3.000000000000000e+00,
    2.000000000000000e+00,
    -2.333333333333333e+00,
    -3.333333333333333e-01,
    -2.000000000000000e+00,
    -4.333333333333333e+00,
    -2.333333333333333e+00,
    -6.000000000000000e+00,
    3.000000000000000e+00,
    -3.000000000000000e+00,
    -5.333333333333333e+00,
    1.333333333333333e+00,
    -1.333333333333333e+00,
    -4.440892098500626e-16,
    -6.666666666666672e-01,
    6.666666666666663e-01,
    5.333333333333332e+00,
    6.666666666666663e+00,
    3.999999999999999e+00,
    -7.333333333333333e+00,
    -1.666666666666667e+00,
    -2.333333333333333e+00,
    7.333333333333331e+00,
    5.666666666666665e+00,
    4.999999999999998e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.666666666666667e+00,
    4.000000000000000e+00,
    1.333333333333333e+00,
    2.220446049250313e-16,
    -2.000000000000000e+00,
    -6.666666666666667e-01,
    -2.666666666666667e+00,
    1.333333333333331e+00,
    -1.333333333333333e+00,
    -3.333333333333333e+00,
    -3.333333333333331e-01,
    -9.999999999999998e-01,
    3.333333333333332e+00,    2.999999999999999e+00,    2.333333333333333e+00,
    -1.332267629550188e-15,    -4.440892098500626e-16,    -4.440892098500626e-16,
    6.666666666666667e-01,    1.000000000000000e+00,    3.333333333333334e-01,
    -6.666666666666666e-01,    3.333333333333329e-01,    -3.333333333333334e-01,
    -6.661338147750939e-16,    -2.220446049250313e-16,    -2.220446049250313e-16,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    6.000000000000000e+00,    2.000000000000000e+00,    1.000000000000000e+01,
    2.000000000000000e+00,    -0.000000000000000e+00,    -2.666666666666667e+00,
    -2.000000000000000e+00,    -2.000000000000000e+00,    -4.666666666666666e+00,
    -6.000000000000000e+00,    -4.000000000000000e+00,    4.000000000000000e+00,
    4.000000000000000e+00,    1.333333333333333e+00,    4.888888888888889e+00,
    3.330669073875470e-16,    -6.666666666666665e-01,    -2.444444444444445e+00,
    -3.999999999999999e+00,    -2.666666666666666e+00,    8.888888888888871e-01,
    2.000000000000000e+00,    6.666666666666667e-01,    1.555555555555556e+00,
    -2.000000000000000e+00,    -1.333333333333333e+00,    -4.444444444444446e-01,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    -6.666666666666666e+00,    -2.222222222222222e+00,    1.333333333333334e+00,
    -5.551115123125783e-16,    1.111111111111111e+00,    -6.666666666666674e-01,
    6.666666666666664e+00,    4.444444444444443e+00,    7.999999999999996e+00,
    -3.333333333333333e+00,    -1.111111111111111e+00,    -2.222222222222220e-01,
    3.333333333333332e+00,    2.222222222222221e+00,    3.111111111111110e+00,
    -5.551115123125783e-16,    -1.850371707708594e-16,    -1.850371707708594e-16,
    -8.666666666666666e+00,    -2.888888888888889e+00,    -2.000000000000000e+00,
    8.666666666666664e+00,    5.777777777777776e+00,    6.666666666666664e+00,
    -1.443289932012704e-15,    -4.810966440042345e-16,    -4.810966440042345e-16,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    2.000000000000000e+00,    1.300000000000000e+01,    3.000000000000000e+00,
    2.000000000000000e+00,    5.000000000000002e+00,    1.666666666666667e+00,
    2.000000000000000e+00,    -2.999999999999999e+00,    3.333333333333336e-01,
    2.000000000000000e+00,    -1.100000000000000e+01,    -1.000000000000000e+00,
    -1.333333333333333e+00,    -2.000000000000000e+00,    -2.000000000000000e+00,
    -1.333333333333333e+00,    -6.666666666666669e-01,    -6.666666666666669e-01,
    -1.333333333333333e+00,    6.666666666666661e-01,    6.666666666666663e-01,
    4.666666666666665e+00,    -3.000000000000001e+00,    3.333333333333323e-01,
    4.666666666666665e+00,    7.666666666666663e+00,    4.333333333333331e+00,
    2.000000000000000e+01,    1.000000000000000e+01,    1.000000000000000e+01,
    8.888888888888891e-01,    5.777777777777779e+00,    1.333333333333333e+00,
    8.888888888888891e-01,    4.444444444444449e-01,    4.444444444444446e-01,
    8.888888888888891e-01,    -4.888888888888888e+00,    -4.444444444444443e-01,
    2.222222222222219e-01,    -1.888888888888889e+00,    -7.777777777777781e-01,
    2.222222222222219e-01,    2.111111111111110e+00,    9.999999999999997e-01,
    8.888888888888888e+00,    4.444444444444442e+00,    4.444444444444444e+00,
    2.222222222222223e-01,    1.444444444444445e+00,    3.333333333333334e-01,
    2.222222222222223e-01,    -1.222222222222222e+00,    -1.111111111111111e-01,
    2.222222222222222e+00,    1.111111111111110e+00,    1.111111111111111e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    2.000000000000000e+00,    6.000000000000000e+00,    1.000000000000000e+01,
    2.000000000000000e+00,    2.666666666666667e+00,    4.000000000000000e+00,
    2.000000000000000e+00,    -6.666666666666663e-01,    -1.999999999999999e+00,
    2.000000000000000e+00,    -4.000000000000000e+00,    -8.000000000000000e+00,
    -1.333333333333333e+00,    2.666666666666667e+00,    -3.555555555555556e+00,
    -1.333333333333333e+00,    -6.666666666666663e-01,    -6.666666666666669e-01,
    -1.333333333333333e+00,    -3.999999999999999e+00,    2.222222222222221e+00,
    -4.666666666666666e+00,    -6.666666666666661e-01,    -8.222222222222221e+00,
    -4.666666666666666e+00,    -3.999999999999999e+00,    3.555555555555554e+00,
    -8.000000000000000e+00,    -4.000000000000000e+00,    -4.000000000000000e+00,
    -2.222222222222222e+00,    -6.666666666666668e+00,    1.333333333333334e+00,
    -2.222222222222222e+00,    -1.111111111111112e+00,    -1.111111111111111e+00,
    -2.222222222222222e+00,    4.444444444444443e+00,    -3.555555555555556e+00,
    3.333333333333333e+00,    -1.111111111111112e+00,    -1.555555555555556e+00,
    3.333333333333333e+00,    4.444444444444443e+00,    4.888888888888888e+00,
    8.888888888888888e+00,    4.444444444444443e+00,    4.444444444444443e+00,
    -2.888888888888889e+00,    -8.666666666666668e+00,    -2.000000000000000e+00,
    -2.888888888888889e+00,    5.777777777777776e+00,    -8.888888888888893e-01,
    1.155555555555555e+01,    5.777777777777776e+00,    5.777777777777776e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    1.500000000000000e+01,
    2.000000000000000e+00,    1.000000000000000e+00,    5.666666666666668e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    -3.666666666666666e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    -1.300000000000000e+01,
    2.000000000000000e+00,    1.000000000000000e+00,    1.033333333333333e+01,
    2.000000000000000e+00,    1.000000000000000e+00,    1.000000000000001e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    -8.333333333333332e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    5.666666666666668e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    -3.666666666666666e+00,
    2.000000000000000e+00,    1.000000000000000e+00,    1.000000000000000e+00,
    -1.111111111111111e+00,    -5.555555555555557e-01,    -3.666666666666667e+00,
    -1.111111111111111e+00,    -5.555555555555557e-01,    -5.555555555555559e-01,
    -1.111111111111111e+00,    -5.555555555555557e-01,    2.555555555555554e+00,
    -1.111111111111111e+00,    -5.555555555555557e-01,    -2.111111111111112e+00,
    -1.111111111111111e+00,    -5.555555555555557e-01,    9.999999999999996e-01,
    -1.111111111111111e+00,    -5.555555555555557e-01,    -5.555555555555559e-01,
    8.222222222222221e+00,    4.111111111111111e+00,    -3.666666666666667e+00,
    8.222222222222221e+00,    4.111111111111111e+00,    1.188888888888889e+01,
    8.222222222222221e+00,    4.111111111111111e+00,    4.111111111111109e+00,
    3.000000000000000e+01,    1.500000000000000e+01,    1.500000000000000e+01,
    0.000000000000000e+00,    1.500000000000000e+01,    3.000000000000000e+00,
    0.000000000000000e+00,    1.500000000000000e+01,    3.000000000000000e+00,
    0.000000000000000e+00,    1.500000000000000e+01,    3.000000000000000e+00,
    0.000000000000000e+00,    1.500000000000000e+01,    3.000000000000000e+00,
    -0.000000000000000e+00,    -3.333333333333333e+00,    -2.000000000000000e+00,
    -0.000000000000000e+00,    -3.333333333333333e+00,    -2.000000000000000e+00,
    -0.000000000000000e+00,    -3.333333333333333e+00,    -2.000000000000000e+00,
    0.000000000000000e+00,    1.666666666666666e+00,    2.999999999999999e+00,
    0.000000000000000e+00,    1.666666666666666e+00,    2.999999999999999e+00,
    0.000000000000000e+00,    3.000000000000000e+01,    1.800000000000000e+01,
    0.000000000000000e+00,    6.666666666666668e+00,    1.333333333333333e+00,
    0.000000000000000e+00,    6.666666666666668e+00,    1.333333333333333e+00,
    0.000000000000000e+00,    6.666666666666668e+00,    1.333333333333333e+00,
    0.000000000000000e+00,    -1.666666666666667e+00,    -3.333333333333336e-01,
    0.000000000000000e+00,    -1.666666666666667e+00,    -3.333333333333336e-01,
    0.000000000000000e+00,    1.333333333333333e+01,    8.000000000000000e+00,
    0.000000000000000e+00,    1.666666666666667e+00,    3.333333333333333e-01,
    0.000000000000000e+00,    1.666666666666667e+00,    3.333333333333333e-01,
    0.000000000000000e+00,    3.333333333333332e+00,    2.000000000000000e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,    8.000000000000000e+00,    1.000000000000000e+01,
    0.000000000000000e+00,    8.000000000000000e+00,    1.000000000000000e+01,
    0.000000000000000e+00,    7.999999999999999e+00,    9.999999999999998e+00,
    0.000000000000000e+00,    8.000000000000000e+00,    1.000000000000000e+01,
    -0.000000000000000e+00,    1.333333333333334e+00,    -5.111111111111111e+00,
    -0.000000000000000e+00,    1.333333333333334e+00,    -5.111111111111111e+00,
    -0.000000000000000e+00,    1.333333333333334e+00,    -5.111111111111111e+00,
    0.000000000000000e+00,    -5.333333333333332e+00,    -2.444444444444446e+00,
    0.000000000000000e+00,    -5.333333333333332e+00,    -2.444444444444446e+00,
    0.000000000000000e+00,    -1.200000000000000e+01,    1.800000000000000e+01,
    0.000000000000000e+00,    -8.888888888888889e+00,    1.333333333333334e+00,
    0.000000000000000e+00,    -8.888888888888889e+00,    1.333333333333334e+00,
    0.000000000000000e+00,    -8.888888888888889e+00,    1.333333333333334e+00,
    0.000000000000000e+00,    2.222222222222221e+00,    4.444444444444433e-01,
    0.000000000000000e+00,    2.222222222222221e+00,    4.444444444444433e-01,
    0.000000000000000e+00,    1.333333333333333e+01,    1.733333333333333e+01,
    0.000000000000000e+00,    -1.155555555555556e+01,    -2.000000000000000e+00,
    0.000000000000000e+00,    -1.155555555555556e+01,    -2.000000000000000e+00,
    0.000000000000000e+00,    1.733333333333333e+01,    1.133333333333333e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    0.000000000000000e+00,
    0.000000000000000e+00,    3.000000000000001e+00,    1.500000000000000e+01,
    0.000000000000000e+00,    3.000000000000001e+00,    1.500000000000000e+01,
    0.000000000000000e+00,    3.000000000000000e+00,    1.500000000000000e+01,    
    0.000000000000000e+00,    3.000000000000001e+00,    1.500000000000000e+01,
    -0.000000000000000e+00,    3.000000000000001e+00,    1.000000000000000e+00,
    -0.000000000000000e+00,    3.000000000000001e+00,    1.000000000000000e+00,
    -0.000000000000000e+00,    3.000000000000001e+00,    1.000000000000000e+00,
    -0.000000000000000e+00,    3.000000000000000e+00,    -1.300000000000000e+01,
    -0.000000000000000e+00,    3.000000000000000e+00,    -1.300000000000000e+01,
    -0.000000000000000e+00,    3.000000000000001e+00,    -2.700000000000001e+01,
    -0.000000000000000e+00,    -1.666666666666667e+00,    -3.666666666666667e+00,
    -0.000000000000000e+00,    -1.666666666666667e+00,    -3.666666666666667e+00,
    -0.000000000000000e+00,    -1.666666666666667e+00,    -3.666666666666667e+00,
    0.000000000000000e+00,    -1.666666666666667e+00,    9.999999999999998e-01,
    0.000000000000000e+00,    -1.666666666666667e+00,    9.999999999999998e-01,
    0.000000000000000e+00,    -1.666666666666667e+00,    5.666666666666666e+00,
    0.000000000000000e+00,    1.233333333333333e+01,    -3.666666666666668e+00,
    0.000000000000000e+00,    1.233333333333333e+01,    -3.666666666666668e+00,
    0.000000000000000e+00,    1.233333333333333e+01,    1.966666666666666e+01,
    0.000000000000000e+00,    4.500000000000000e+01,    1.500000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    0.000000000000000e+00,    0.000000000000000e+00,    1.800000000000000e+01,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    -0.000000000000000e+00,    -0.000000000000000e+00,    -5.333333333333334e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    8.666666666666661e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    8.666666666666661e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    8.666666666666661e+00,
    0.000000000000000e+00,    0.000000000000000e+00,    6.000000000000000e+01

};

  int fiat_index_cur = 0;
  for (int i=0;i<polydim;i++) {
    for (int j=0;j<np_lattice;j++) {
      for (int k=0;k<3;k++) {
        if (std::abs( dBasisAtLattice(i,j,k) - fiat_vals[fiat_index_cur] ) > INTREPID_TOL ) {
          errorFlag++;
          *outStream << std::setw(70) << "^^^^----FAILURE!" << "\n";
          
          // Output the multi-index of the value where the error is:
          *outStream << " At multi-index { ";
          *outStream << i << " " << j << " " << k;
          *outStream << "}  computed value: " << dBasisAtLattice(i,j,k)
                     << " but correct value: " << fiat_vals[fiat_index_cur] << "\n";
          *outStream << "Difference: " << std::abs( dBasisAtLattice(i,j,k) - fiat_vals[fiat_index_cur] ) << "\n";
        }
        fiat_index_cur++;
      }
    }
  }



  if (errorFlag != 0)
    std::cout << "End Result: TEST FAILED\n";
  else
    std::cout << "End Result: TEST PASSED\n";
  
  // reset format state of std::cout
  std::cout.copyfmt(oldFormatState);
  
  return errorFlag;
}
