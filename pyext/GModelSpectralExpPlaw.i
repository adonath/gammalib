/***************************************************************************
 *      GModelSpectralExpPlaw.i - Exponential cut off power law model      *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2013 by Juergen Knoedlseder                         *
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
 * @file GModelSpectralExpPlaw.i
 * @brief Exponential cut off power law spectral class interface definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GModelSpectralExpPlaw.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GModelSpectralExpPlaw
 *
 * @brief Exponential cut off power law spectral class
 ***************************************************************************/
class GModelSpectralExpPlaw : public GModelSpectral {
public:
    // Constructors and destructors
    GModelSpectralExpPlaw(void);
    explicit GModelSpectralExpPlaw(const double& norm, const double& index,
                                   const double& ecut);
    explicit GModelSpectralExpPlaw(const GXmlElement& xml);
    GModelSpectralExpPlaw(const GModelSpectralExpPlaw& model);
    virtual ~GModelSpectralExpPlaw(void);

    // Implemented pure virtual methods
    virtual void                   clear(void);
    virtual GModelSpectralExpPlaw* clone(void) const;
    virtual std::string            type(void) const;
    virtual double                 eval(const GEnergy& srcEng) const;
    virtual double                 eval_gradients(const GEnergy& srcEng) const;
    virtual double                 flux(const GEnergy& emin, const GEnergy& emax) const;
    virtual double                 eflux(const GEnergy& emin, const GEnergy& emax) const;
    virtual GEnergy                mc(const GEnergy& emin, const GEnergy& emax, GRan& ran) const;
    virtual void                   read(const GXmlElement& xml);
    virtual void                   write(GXmlElement& xml) const;

    // Other methods
    double norm(void) const;
    double index(void) const;
    double ecut(void) const;
    double pivot(void) const;
};


/***********************************************************************//**
 * @brief GModelSpectralExpPlaw class extension
 ***************************************************************************/
%extend GModelSpectralExpPlaw {
    GModelSpectralExpPlaw copy() {
        return (*self);
    }
};
