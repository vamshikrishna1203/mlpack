/**
 * @file minmaxscaler.hpp
 * @author Jeffin Sam
 *
 * MinMaxScaler class to scale features.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_DATA_SCALE_HPP
#define MLPACK_CORE_DATA_SCALE_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace data {
/**
 * A simple MinMax Scaler class
 *
 * Given an input dataset this class helps you to scale each
 * feature to a given range.
 *
 * @code
 * arma::Mat<double> input = loadData();
 * arma::Mat<double> output;
 *
 * // Scale the features.
 * MinMaxScaler<double> scale;
 * scale.Tranform(input, output);
 *
 * // Retransform the input.
 * scale.InverseTransform(output, input);
 * @endcode
 */
class MinMaxScaler
{
 public:
  /**
  * Default constructor
  *
  * @param min Lower range of scaling.
  * @param max Upper range of scaling. 
  */
  MinMaxScaler(const double min = 0, const double max = 1)
  {
    scalemin = min;
    scalemax = max;
  }

  /**
  * Function to scale Features.
  *
  * @param input Dataset to scale features.
  * @param output Output matrix with scaled features.
  */
  template<typename MatType>
  void Transform(const MatType& input, MatType& output)
  {
    output.copy_size(input);
    itemMin = arma::min(input, 1);
    itemMax = arma::max(input, 1);
    scale = (scalemax - scalemin) / (itemMax - itemMin);
    // Handline Zeroes in scale vector.
    for (size_t i = 0; i < scale.n_elem; i++)
    {
      if (scale(i) == 0)
      {
        scale(i) = 1;
      }
    }
    for (size_t i = 0; i < input.n_rows; i++)
    {
      for (size_t j = 0; j < input.n_cols; j++)
      {
        output(i, j) = scale(i) * input(i, j) + scalemin - itemMin(i) *
            scale(i);
      }
    }
  }

  /**
  * Function to retrive original dataset.
  *
  * @param input Scaled dataset.
  * @param output Output matrix with original Dataset.
  */
  template<typename MatType>
  void InverseTransform(const MatType& input, MatType& output)
  {
    output.copy_size(input);
    for (size_t i = 0; i < input.n_rows; i++)
    {
      for (size_t j = 0; j < input.n_cols; j++)
      {
        output(i, j) = (input(i, j) - scalemin + itemMin(i) * scale(i)) /
            scale(i);
      }
    }
  }

  //! Get the Min row vector.
  const arma::colvec& ItemMin() const { return itemMin; }
  //! Get the Max row vector.
  const arma::colvec& ItemMax() const { return itemMax; }
  //! Get the Scale row vector.
  const arma::colvec& Scale() const { return scale; }
  //! Get the upper range parameter.
  const double ScaleMax() const { return scalemax; }
  //! Get the lower range parameter.
  const double ScaleMin() const { return scalemin; }

 private:
  // Vector which holds minimum of each feature.
  arma::colvec itemMin;
  // Vector which holds maximum of each feature.
  arma::colvec itemMax;
  // Scale vector which is used to scale up each feature.
  arma::colvec scale;
  // Lower value for range.
  double scalemin;
  // Upper value for range.
  double scalemax;
}; // class MinMaxScaler

} // namespace data
} // namespace mlpack

#endif
