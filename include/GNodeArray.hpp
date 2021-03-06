/***************************************************************************
 *                 GNodeArray.hpp  -  Array of nodes class                 *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2012 by Juergen Knoedlseder                         *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GNodeArray.hpp
 * @brief Node array class interface definition
 * @author Juergen Knoedlseder
 */

#ifndef GNODEARRAY_HPP
#define GNODEARRAY_HPP

/* __ Includes ___________________________________________________________ */
#include <vector>
#include <string>
#include "GBase.hpp"
#include "GVector.hpp"


/***********************************************************************//**
 * @class GNodeArray
 *
 * @brief Interface for the node array class
 *
 * The node array class collects nodes \f$x_i\f$ that may be used to describe
 * a functional relation \f$y_i=f(x_i)\f$. This class may be used to perform
 * a linear interpolation between the nodes to determine any value of
 * \f$y=f(x)\f$.
 * Nodes are allocated either from a double precision array, a GVector object
 * or a std::vector using the nodes() method. Alternatively, the node array
 * may be built on the fly using the append() method.
 * Interpolation can be either performed using the interpolate() method
 * or using the set_value(). In the latter case, the node indices and
 * weighting factors can be recovered using inx_left(), inx_right(),
 * wgt_left() and wgt_right().
 * If the nodes are equally spaced, interpolation is more rapid.
 ***************************************************************************/
class GNodeArray : public GBase {

public:
    // Constructors and destructors
    GNodeArray(void);
    GNodeArray(const GNodeArray& array);
    GNodeArray(const int& num, const double* array);
    GNodeArray(const GVector& vector);
    GNodeArray(const std::vector<double>& vector);
    virtual ~GNodeArray(void);

    // Operators
    GNodeArray&   operator= (const GNodeArray & array);
    double&       operator[](const int& index);
    const double& operator[](const int& index) const;

    // Methods
    void          clear(void);
    GNodeArray*   clone(void) const;
    int           size(void) const { return m_node.size(); }
    void          nodes(const int& num, const double* array);
    void          nodes(const GVector& vector);
    void          nodes(const std::vector<double>& vector);
    void          append(const double& node);
    double        interpolate(const double& value,
                              const std::vector<double>& vector) const;
    void          set_value(const double& value) const;
    const int&    inx_left(void) const { return m_inx_left; }
    const int&    inx_right(void) const { return m_inx_right; }
    const double& wgt_left(void) const { return m_wgt_left; }
    const double& wgt_right(void) const { return m_wgt_right; }
    std::string   print(void) const;

private:
    // Methods
    void init_members(void);
    void copy_members(const GNodeArray& array);
    void free_members(void);
    void setup(void) const;
    
    // Node values
    std::vector<double> m_node;                   //!< Array of nodes

    // Evaluation cache
    mutable bool                m_need_setup;     //!< Call of setup is required
    mutable bool                m_is_linear;      //!< Nodes form a linear array
    mutable bool                m_has_last_value; //!< Last value is valid
    mutable std::vector<double> m_step;           //!< Distance to next node
    mutable double              m_last_value;     //!< Last requested value
    mutable double              m_linear_slope;   //!< Slope for linear array
    mutable double              m_linear_offset;  //!< Offset for linear array
    mutable int                 m_inx_left;       //!< Index of left node for linear interpolation
    mutable int                 m_inx_right;      //!< Index of right node for linear interpolation
    mutable double              m_wgt_left;       //!< Weight for left node for linear interpolation
    mutable double              m_wgt_right;      //!< Weight for right node for linear interpolation
};

#endif /* GNODEARRAY_HPP */
