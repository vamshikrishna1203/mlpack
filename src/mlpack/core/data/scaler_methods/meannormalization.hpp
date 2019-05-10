/**
 * @file meannormalization.hpp
 * @author Jeffin Sam
 *
 * MeanNormalization class to scale features.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_DATA_MEAN_NORMALIZATION_HPP
#define MLPACK_CORE_DATA_MEAN_NORMALIZATION_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace data {
/**
 * A simple Mean Normalization class
 *
 * Given an input dataset this class helps you to normalize each
 * feature.
 *
 * @code
 * arma::Mat<double> input = loadData();
 * arma::Mat<double> output;
 *
 * // Scale the features.
 * MeanNormalization<double> scale;
 * scale.Tranform(input, output);
 *
 * // Retransform the input.
 * scale.InverseTransform(output, input);
 * @endcode
 */
class MeanNormalization
{
 public:

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
    itemMean = arma::mean(input, 1);
    itemMin = arma::min(input, 1);
    itemMax = arma::max(input, 1);
    scale = itemMax - itemMin;
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
        output(i, j) = input(i, j) - itemMean(i);
        output(i, j) /= scale(i);
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
        output(i, j) = input(i, j) * scale(i);
        output(i, j) += itemMean(i);
      }
    }
  }

  //! Get the Mean row vector.
  const arma::colvec& ItemMean() const { return itemMean; }
  //! Get the Min row vector.
  const arma::colvec& ItemMin() const { return itemMin; }
  //! Get the Max row vector.
  const arma::colvec& ItemMax() const { return itemMax; }
  //! Get the Scale row vector.
  const arma::colvec& Scale() const { return scale; }

 private:
  // Vector which holds mean of each feature.
  arma::colvec itemMean;
  // Vector which holds minimum of each feature.
  arma::colvec itemMin;
  // Vector which holds maximum of each feature.
  arma::colvec itemMax;
  // Vector which is used to scale up each feature.
  arma::colvec scale;
}; // class MeanNormalization

} // namespace data
} // namespace mlpack

#endif
