/***************************************************************************
 *                   GCTAPointing.i  -  CTA pointing class                 *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
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
 * @file GCTAPointing.i
 * @brief CTA pointing class Python interface definition
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAPointing.hpp"
%}


/***********************************************************************//**
 * @class GCTAPointing
 *
 * @brief CTA pointing class
 ***************************************************************************/
class GCTAPointing : public GPointing {
public:
    // Constructors and destructors
    GCTAPointing(void);
    GCTAPointing(const GCTAPointing& pnt);
    ~GCTAPointing(void);

    // Implemented pure virtual methods
    virtual void           clear(void);
    virtual GCTAPointing*  clone(void) const;
    virtual const GSkyDir& dir(void) const;

    // Other methods
    void   dir(const GSkyDir& dir);
    const  GMatrix& rot(void) const;
    double zenith(void) const;
    double azimuth(void) const;
};


/***********************************************************************//**
 * @brief GCTAPointing class extension
 ***************************************************************************/
%extend GCTAPointing {
    GCTAPointing copy() {
        return (*self);
    }
};
