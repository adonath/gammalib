/***************************************************************************
 *        GModelSpatialDiffuseCube.hpp - Spatial map cube model class      *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2013 by Juergen Knoedlseder                         *
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
 * @file GModelSpatialDiffuseCube.hpp
 * @brief Spatial map cube model class interface definition
 * @author Juergen Knoedlseder
 */

#ifndef GMODELSPATIALDIFFUSECUBE_HPP
#define GMODELSPATIALDIFFUSECUBE_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GModelSpatialDiffuse.hpp"
#include "GModelPar.hpp"
#include "GSkyDir.hpp"
#include "GXmlElement.hpp"


/***********************************************************************//**
 * @class GModelSpatialDiffuseCube
 *
 * @brief Spatial map cube model
 *
 * This class implements the spatial component of the factorised source
 * model for a map cube. A map cube is a set of sky maps for different
 * energies.
 *
 * @todo Loading not yet implemented. Introduce a loading flag to load cube
 *       only when it is required. This avoid unnecessary memory allocation.
 * @todo eval() and eval_gradients() methods not yet implemented.
 ***************************************************************************/
class GModelSpatialDiffuseCube : public GModelSpatialDiffuse {

public:
    // Constructors and destructors
    GModelSpatialDiffuseCube(void);
    explicit GModelSpatialDiffuseCube(const GXmlElement& xml);
    GModelSpatialDiffuseCube(const GModelSpatialDiffuseCube& model);
    virtual ~GModelSpatialDiffuseCube(void);

    // Operators
    virtual GModelSpatialDiffuseCube& operator= (const GModelSpatialDiffuseCube& model);

    // Implemented pure virtual methods
    virtual void                      clear(void);
    virtual GModelSpatialDiffuseCube* clone(void) const;
    virtual std::string               type(void) const { return "MapCubeFunction"; }
    virtual double                    eval(const GSkyDir& srcDir) const;
    virtual double                    eval_gradients(const GSkyDir& srcDir) const;
    virtual GSkyDir                   mc(GRan& ran) const;
    virtual void                      read(const GXmlElement& xml);
    virtual void                      write(GXmlElement& xml) const;
    virtual std::string               print(void) const;

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GModelSpatialDiffuseCube& model);
    void free_members(void);
    void load_cube(const std::string& filename);

    // Protected members
    GModelPar   m_value;           //!< Value
    std::string m_filename;        //!< Name of map cube
};

#endif /* GMODELSPATIALDIFFUSECUBE_HPP */
