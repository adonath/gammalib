/***************************************************************************
 *          GSkyDir.hpp  -  Class that implements a sky direction          *
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
 * @file GSkyDir.hpp
 * @brief Sky direction class interface definition
 * @author Juergen Knoedlseder
 */

#ifndef GSKYDIR_HPP
#define GSKYDIR_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GBase.hpp"
#include "GVector.hpp"


/***********************************************************************//**
 * @class GSkyDir
 *
 * @brief Sky direction class
 *
 * This class implements a spherical coordinate on the sky. Two coordinate
 * systems are supported: celestial (or equatorial), defined by Right
 * Ascension and Declination, and Galactic, defined by galactic longitude
 * and galactic latitude. The class provides automatic conversion between
 * both systems if required. Coordinates are stored in either of the
 * systems (in units of radians), and conversion is performed (and stored)
 * if requested. Coordinates can be given and returned in radians or in
 * degrees. Note that the epoch for celestial coordinates is fixed to J2000.
 ***************************************************************************/
class GSkyDir : public GBase {

    // Operator friends
    friend bool operator==(const GSkyDir &a, const GSkyDir &b);
    friend bool operator!=(const GSkyDir &a, const GSkyDir &b);

public:
    // Constructors and destructors
    GSkyDir(void);
    GSkyDir(const GSkyDir& dir);
    virtual ~GSkyDir(void);

    // Operators
    GSkyDir& operator= (const GSkyDir& dir);

    // Methods
    void        clear(void);
    GSkyDir*    clone(void) const;
    void        radec(const double& ra, const double& dec);
    void        radec_deg(const double& ra, const double& dec);
    void        lb(const double& l, const double& b);
    void        lb_deg(const double& l, const double& b);
    void        celvector(const GVector& vector);
    void        rotate_deg(const double& phi, const double& theta);
    double      l(void) const;
    double      l_deg(void) const;
    double      b(void) const;
    double      b_deg(void) const;
    double      ra(void) const;
    double      ra_deg(void) const;
    double      dec(void) const;
    double      dec_deg(void) const;
    GVector     celvector(void) const;
    double      dist(const GSkyDir& dir) const;
    double      dist_deg(const GSkyDir& dir) const;
    double      posang(const GSkyDir& dir) const;
    double      posang_deg(const GSkyDir& dir) const;
    std::string print(void) const;

private:
    // Private methods
    void init_members(void);
    void copy_members(const GSkyDir& dir);
    void free_members(void);
    void equ2gal(void) const;
    void gal2equ(void) const;
    void euler(const int& type, const double& xin, const double &yin,
               double* xout, double *yout) const;

    // Private members
    bool   m_has_lb;     //!< Has galactic coordinates
    bool   m_has_radec;  //!< Has equatorial coordinates
    double m_l;          //!< Galactic longitude in radians
    double m_b;          //!< Galactic latitude in radians
    double m_ra;         //!< Right Ascension in radians
    double m_dec;        //!< Declination in radians
};

#endif /* GSKYDIR_HPP */
