// @HEADER
// ************************************************************************
//
//                           Intrepid2 Package
//                 Copyright (2007) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Kyungjoo Kim  (kyukim@sandia.gov), or
//                    Mauro Perego  (mperego@sandia.gov)
//
// ************************************************************************
// @HEADER

/** \file   Intrepid_HGRAD_TRI_C2_FEM_Def.hpp
    \brief  Definition file for FEM basis functions of degree 2 for H(grad) functions on TRI.
    \author Created by P. Bochev and D. Ridzal.
            Kokkorized by Kyungjoo Kim
 */

#ifndef __INTREPID2_HGRAD_TRI_C2_FEM_DEF_HPP__
#define __INTREPID2_HGRAD_TRI_C2_FEM_DEF_HPP__

namespace Intrepid2 {

  // -------------------------------------------------------------------------------------

  template<typename SpT, typename OT, typename PT>
  template<EOperator opType>
  template<typename outputValueValueType, class ...outputValueProperties,
           typename inputPointValueType,  class ...inputPointProperties>
  KOKKOS_INLINE_FUNCTION
  void
  Basis_HGRAD_TRI_C2_FEM<SpT,OT,PT>::Serial<opType>::
  getValues( /**/  Kokkos::DynRankView<outputValueValueType,outputValueProperties...> output,
             const Kokkos::DynRankView<inputPointValueType, inputPointProperties...>  input ) {
    switch (opType) {
    case OPERATOR_VALUE: {
      const auto x = input(0);
      const auto y = input(1);
      
      // output is a rank-2 array with dimensions (basisCardinality_, dim0)
      output(0) = (x + y - 1.0)*(2.0*x + 2.0*y - 1.0);
      output(1) = x*(2.0*x - 1.0);
      output(2) = y*(2.0*y - 1.0);
      output(3) = -4.0*x*(x + y - 1.0);
      output(4) =  4.0*x*y;
      output(5) = -4.0*y*(x + y - 1.0);
      break;
    }
    case OPERATOR_D1: 
    case OPERATOR_GRAD: {
      const auto x = input(0);
      const auto y = input(1);
      // output is a rank-3 array with dimensions (basisCardinality_, dim0, spaceDim)
      output(0, 0) =  4.0*x + 4.0*y - 3.0;
      output(0, 1) =  4.0*x + 4.0*y - 3.0;

      output(1, 0) =  4.0*x - 1.0;
      output(1, 1) =  0.0;

      output(2, 0) =  0.0;
      output(2, 1) =  4.0*y - 1.0;
      
      output(3, 0) = -4.0*(2.0*x + y - 1.0);
      output(3, 1) = -4.0*x;

      output(4, 0) =  4.0*y;
      output(4, 1) =  4.0*x;

      output(5, 0) = -4.0*y;
      output(5, 1) = -4.0*(x + 2.0*y - 1.0);
      break;
    }
    case OPERATOR_CURL: {
      const auto x = input(0);
      const auto y = input(1);
      // CURL(u) = (u_y, -u_x), is rotated GRAD
      output(0, 1) =-(4.0*x + 4.0*y - 3.0);
      output(0, 0) =  4.0*x + 4.0*y - 3.0;
      
      output(1, 1) =-(4.0*x - 1.0);
      output(1, 0) =  0.0;
      
      output(2, 1) =  0.0;
      output(2, 0) =  4.0*y - 1.0;
      
      output(3, 1) =  4.0*(2.0*x + y - 1.0);
      output(3, 0) = -4.0*x;
      
      output(4, 1) = -4.0*y;
      output(4, 0) =  4.0*x;
      
      output(5, 1) =  4.0*y;
      output(5, 0) = -4.0*(x + 2.0*y - 1.0);
      break;
    }
    case OPERATOR_D2: {
      // output is a rank-3 array with dimensions (basisCardinality_, dim0, DkCardinality)
      // D2 -> (2,0) -> dx^2. 
      output(0, 0) = 4.0;
      output(1, 0) = 4.0;
      output(2, 0) = 0.0;
      output(3, 0) =-8.0;
      output(4, 0) = 0.0;
      output(5, 0) = 0.0;
      
      // D2 -> (1,1) -> dx dy
      output(0, 1) = 4.0;
      output(1, 1) = 0.0;
      output(2, 1) = 0.0;
      output(3, 1) =-4.0;
      output(4, 1) = 4.0;
      output(5, 1) =-4.0;
      
      // D2 -> (0,2) -> dy^2
      output(0, 2) = 4.0;
      output(1, 2) = 0.0;
      output(2, 2) = 4.0;
      output(3, 2) = 0.0;
      output(4, 2) = 0.0;
      output(5, 2) =-8.0;
      break;
    }
    case OPERATOR_MAX: {
      const auto jend = output.dimension(1);
      const auto iend = output.dimension(0);

      for (size_type j=0;j<jend;++j)
        for (size_type i=0;i<iend;++i)
          output(i, j) = 0.0;
      break;
    }
    default: {
      INTREPID2_TEST_FOR_ABORT( opType != OPERATOR_VALUE &&
                                opType != OPERATOR_GRAD &&
                                opType != OPERATOR_CURL &&
                                opType != OPERATOR_D1 &&
                                opType != OPERATOR_D2 &&
                                opType != OPERATOR_MAX,
                                ">>> ERROR: (Intrepid2::Basis_HGRAD_TRI_C2_FEM::Serial::getValues) operator is not supported");
    }
    }
  }
  
  // -------------------------------------------------------------------------------------


  template< typename SpT, typename OT, typename PT>
  Basis_HGRAD_TRI_C2_FEM<SpT,OT,PT>::
  Basis_HGRAD_TRI_C2_FEM()
    : impl_(this)
  {
    this -> basisCardinality_  = 6;
    this -> basisDegree_       = 2;    
    this -> basisCellTopology_ = shards::CellTopology(shards::getCellTopologyData<shards::Triangle<3> >() );
    this -> basisType_         = BASIS_FEM_DEFAULT;
    this -> basisCoordinates_  = COORDINATES_CARTESIAN;
  
    // initialize tags
    {
      // Basis-dependent initializations
      const ordinal_type tagSize  = 4;        // size of DoF tag, i.e., number of fields in the tag
      const ordinal_type posScDim = 0;        // position in the tag, counting from 0, of the subcell dim 
      const ordinal_type posScOrd = 1;        // position in the tag, counting from 0, of the subcell ordinal
      const ordinal_type posDfOrd = 2;        // position in the tag, counting from 0, of DoF ordinal relative to the subcell
    
      // An array with local DoF tags assigned to the basis functions, in the order of their local enumeration 
      ordinal_type tags[24]  = { 0, 0, 0, 1,
                                 0, 1, 0, 1,
                                 0, 2, 0, 1,
                                 1, 0, 0, 1,
                                 1, 1, 0, 1,
                                 1, 2, 0, 1};
    
      //host tags
      ordinal_type_array_1d_host tagView(&tags[0], 24);

      // Basis-independent function sets tag and enum data in tagToOrdinal_ and ordinalToTag_ arrays:
      this->setOrdinalTagData(this -> tagToOrdinal_,
                              this -> ordinalToTag_,
                              tagView,
                              this -> basisCardinality_,
                              tagSize,
                              posScDim,
                              posScOrd,
                              posDfOrd);
    }  

    // dofCoords on host and create its mirror view to device
    Kokkos::DynRankView<PT,typename SpT::array_layout,Kokkos::HostSpace>
      dofCoords("dofCoordsHost", this->basisCardinality_,this->basisCellTopology_.getDimension());

    dofCoords(0,0) =  0.0;   dofCoords(0,1) =  0.0;
    dofCoords(1,0) =  1.0;   dofCoords(1,1) =  0.0;
    dofCoords(2,0) =  0.0;   dofCoords(2,1) =  1.0;
    dofCoords(3,0) =  0.5;   dofCoords(3,1) =  0.0;
    dofCoords(4,0) =  0.5;   dofCoords(4,1) =  0.5;
    dofCoords(5,0) =  0.0;   dofCoords(5,1) =  0.5;

    this->dofCoords_ = Kokkos::create_mirror_view(typename SpT::memory_space(), dofCoords);
    Kokkos::deep_copy(this->dofCoords_, dofCoords);

  }  


  template<typename SpT, typename OT, typename PT>
  template<typename outputValueValueType, class ...outputValueProperties,
           typename inputPointValueType,  class ...inputPointProperties>
  void
  Basis_HGRAD_TRI_C2_FEM<SpT,OT,PT>::Internal::
  getValues( /**/  Kokkos::DynRankView<outputValueValueType,outputValueProperties...> outputValues,
             const Kokkos::DynRankView<inputPointValueType, inputPointProperties...>  inputPoints,
             const EOperator operatorType ) const {
#ifdef HAVE_INTREPID2_DEBUG
    Intrepid2::getValues_HGRAD_Args(outputValues,
                                    inputPoints,
                                    operatorType,
                                    obj_->getBaseCellTopology(),
                                    obj_->getCardinality() );
#endif

    typedef          Kokkos::DynRankView<outputValueValueType,outputValueProperties...>         outputValueViewType;
    typedef          Kokkos::DynRankView<inputPointValueType, inputPointProperties...>          inputPointViewType;
    typedef typename ExecSpace<typename inputPointViewType::execution_space,SpT>::ExecSpaceType ExecSpaceType;

    // Number of evaluation points = dim 0 of inputPoints
    const auto loopSize = inputPoints.dimension(0);
    Kokkos::RangePolicy<ExecSpaceType,Kokkos::Schedule<Kokkos::Static> > policy(0, loopSize);

    switch (operatorType) {
    
    case OPERATOR_VALUE: {
      typedef Functor<outputValueViewType,inputPointViewType,OPERATOR_VALUE> FunctorType;
      Kokkos::parallel_for( policy, FunctorType(outputValues, inputPoints) );
      break;
    }
    case OPERATOR_GRAD:
    case OPERATOR_D1: {
      typedef Functor<outputValueViewType,inputPointViewType,OPERATOR_GRAD> FunctorType;
      Kokkos::parallel_for( policy, FunctorType(outputValues, inputPoints) );
      break;
    }
    case OPERATOR_CURL: {
      typedef Functor<outputValueViewType,inputPointViewType,OPERATOR_CURL> FunctorType;
      Kokkos::parallel_for( policy, FunctorType(outputValues, inputPoints) );
      break;
    }
    case OPERATOR_DIV: {
      INTREPID2_TEST_FOR_EXCEPTION( (operatorType == OPERATOR_DIV), std::invalid_argument,
                                    ">>> ERROR (Basis_HGRAD_TRI_C2_FEM): DIV is invalid operator for rank-0 (scalar) fields in 2D.");
      break;
    }
    case OPERATOR_D2: {
      typedef Functor<outputValueViewType,inputPointViewType,OPERATOR_D2> FunctorType;
      Kokkos::parallel_for( policy, FunctorType(outputValues, inputPoints) );
      break;
    } 
    case OPERATOR_D3:
    case OPERATOR_D4:
    case OPERATOR_D5:
    case OPERATOR_D6:
    case OPERATOR_D7:
    case OPERATOR_D8:
    case OPERATOR_D9:
    case OPERATOR_D10: {
      typedef Functor<outputValueViewType,inputPointViewType,OPERATOR_MAX> FunctorType;
      Kokkos::parallel_for( policy, FunctorType(outputValues, inputPoints) );
      break;
    }
    default: {
      INTREPID2_TEST_FOR_EXCEPTION( !( Intrepid2::isValidOperator(operatorType) ), std::invalid_argument,
                                    ">>> ERROR (Basis_HGRAD_TRI_C2_FEM): Invalid operator type");
    }
    }
  }
  
  template<typename SpT, typename OT, typename PT>
  template<typename dofCoordValueType, class ...dofCoordProperties>
  void
  Basis_HGRAD_TRI_C2_FEM<SpT,OT,PT>::Internal::
  getDofCoords( Kokkos::DynRankView<dofCoordValueType,dofCoordProperties...> dofCoords ) const {
#ifdef HAVE_INTREPID2_DEBUG
    // Verify rank of output array.
    INTREPID2_TEST_FOR_EXCEPTION( dofCoords.rank() != 2, std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::Basis_HGRAD_TRI_C2_FEM::getDofCoords) rank = 2 required for dofCoords array");
    // Verify 0th dimension of output array.
    INTREPID2_TEST_FOR_EXCEPTION( dofCoords.dimension(0) != obj_->basisCardinality_, std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::Basis_HGRAD_TRI_C2_FEM::getDofCoords) mismatch in number of dof and 0th dimension of dofCoords array");
    // Verify 1st dimension of output array.
    INTREPID2_TEST_FOR_EXCEPTION( dofCoords.dimension(1) != obj_->basisCellTopology_.getDimension(), std::invalid_argument,
                                  ">>> ERROR: (Intrepid2::Basis_HGRAD_TRI_C2_FEM::getDofCoords) incorrect reference cell (1st) dimension in dofCoords array");
#endif
    Kokkos::deep_copy(dofCoords, obj_->dofCoords_);
  }

}// namespace Intrepid2
#endif
