/***************************************************************************
 *   GModelSpatialEllipticalDisk.hpp - Elliptical disk source model class  *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2013 by Michael Mayer                                    *
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
 * @file GModelSpatialEllipticalDisk.hpp
 * @brief Elliptical disk model class interface definition
 * @author Michael Mayer
 */

#ifndef GMODELSPATIALELLIPTICALDISK_HPP
#define GMODELSPATIALELLIPTICALDISK_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GModelSpatialElliptical.hpp"
#include "GModelPar.hpp"
#include "GSkyDir.hpp"
#include "GXmlElement.hpp"


/**************************************************************************
 * @class GModelSpatialEllipticalDisk
 *
 * @brief Elliptical disk source model class
 *
 * This class implements the spatial component of the factorised source
 * model for an elliptical disk source, i.e. constant surface brightness
 * within ellipse and no emission outside.
 ***************************************************************************/
class GModelSpatialEllipticalDisk : public GModelSpatialElliptical {

public:
    // Constructors and destructors
    GModelSpatialEllipticalDisk(void);
    explicit GModelSpatialEllipticalDisk(const GSkyDir& dir,
                                         const double&  semiminor,
                                         const double&  semimajor,
                                         const double&  posangle);
    explicit GModelSpatialEllipticalDisk(const GXmlElement& xml);
    GModelSpatialEllipticalDisk(const GModelSpatialEllipticalDisk& model);
    virtual ~GModelSpatialEllipticalDisk(void);

    // Operators
    virtual GModelSpatialEllipticalDisk& operator=(const GModelSpatialEllipticalDisk& model);

    // Implemented pure virtual base class methods
    virtual void                         clear(void);
    virtual GModelSpatialEllipticalDisk* clone(void) const;
    virtual std::string                  type(void) const { return "EllipticalDisk"; }
    virtual double                       eval(const double& theta,
                                              const double& posangle) const;
    virtual double                       eval_gradients(const double& theta,
                                                        const double& posangle) const;
    virtual GSkyDir                      mc(GRan& ran) const;
    virtual double                       theta_max(void) const;
    virtual void                         read(const GXmlElement& xml);
    virtual void                         write(GXmlElement& xml) const;
    virtual std::string                  print(void) const;

    // Other methods
    double semiminor(void) const { return m_semiminor.value(); }
    double semimajor(void) const { return m_semimajor.value(); }
    void   semiminor(const double& semiminor) { m_semiminor.value(semiminor); }
    void   semimajor(const double& semimajor) { m_semimajor.value(semimajor); }

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GModelSpatialEllipticalDisk& model);
    void free_members(void);
    void update(void) const;

    // Protected members
    GModelPar      m_semiminor;        //!< semi-minor axis of ellipse (degrees)
    GModelPar      m_semimajor;        //!< semi-major axis of ellipse (degrees)

    // Cached members used for pre-computations
    mutable double m_last_semiminor;   //!< Last semi-minor axis
    mutable double m_last_semimajor;   //!< Last semi-major axis
    mutable double m_semiminor_rad;    //!< Radius in radians
    mutable double m_semimajor_rad;    //!< Radius in radians
    mutable double m_norm;             //!< Normalization
};

#endif /* GMODELSPATIALELLIPTICALDISK_HPP */
