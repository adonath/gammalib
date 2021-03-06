/***************************************************************************
 *      GModelSpatialRadialShell.cpp - Radial shell source model class     *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011-2013 by Christoph Deil                              *
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
 * @file GModelSpatialRadialShell.cpp
 * @brief Radial shell model class implementation
 * @author Christoph Deil
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GModelSpatialRadialShell.hpp"
#include "GModelSpatialRegistry.hpp"

/* __ Constants __________________________________________________________ */

/* __ Globals ____________________________________________________________ */
const GModelSpatialRadialShell g_radial_shell_seed;
const GModelSpatialRegistry    g_radial_shell_registry(&g_radial_shell_seed);

/* __ Method name definitions ____________________________________________ */
#define G_READ                 "GModelSpatialRadialShell::read(GXmlElement&)"
#define G_WRITE               "GModelSpatialRadialShell::write(GXmlElement&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_DEBUG_MC                                     //!< Debug MC method


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModelSpatialRadialShell::GModelSpatialRadialShell(void) : GModelSpatialRadial()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Shell constructor
 *
 * @param[in] dir Sky position of shell centre.
 * @param[in] radius Inner shell radius (degrees).
 * @param[in] width Shell width (degrees).
 * @param[in] small_angle Use small angle approximation
 *
 * The small angle approximation is (a little) faster, but incorrect for
 * shells larger than a few degrees.
 ***************************************************************************/
GModelSpatialRadialShell::GModelSpatialRadialShell(const GSkyDir& dir,
                                     const double&  radius,
                                     const double&  width,
                                     const bool& small_angle) :
                          GModelSpatialRadial()
{
    // Initialise members
    init_members();

    // Assign parameters
    this->dir(dir);
    this->radius(radius);
    this->width(width);
    this->small_angle(small_angle);

    // Return
    return;
}


/***********************************************************************//**
 * @brief XML constructor
 *
 * @param[in] xml XML element.
 *
 * Creates instance of shell spatial model by extracting information from an
 * XML element. See GModelSpatialRadialShell::read() for more information about the
 * expected structure of the XML element.
 ***************************************************************************/
GModelSpatialRadialShell::GModelSpatialRadialShell(const GXmlElement& xml) :
                          GModelSpatialRadial()
{
    // Initialise members
    init_members();

    // Read information from XML element
    read(xml);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Radial shell source model.
 ***************************************************************************/
GModelSpatialRadialShell::GModelSpatialRadialShell(const GModelSpatialRadialShell& model) :
                          GModelSpatialRadial(model)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(model);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelSpatialRadialShell::~GModelSpatialRadialShell(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] model Radial shell source model.
 * @return Radial shell source model.
 ***************************************************************************/
GModelSpatialRadialShell& GModelSpatialRadialShell::operator=(const GModelSpatialRadialShell& model)
{
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelSpatialRadial::operator=(model);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(model);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                            Public methods                               =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear instance
 ***************************************************************************/
void GModelSpatialRadialShell::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GModelSpatialRadial::free_members();
    this->GModelSpatial::free_members();

    // Initialise members
    this->GModelSpatial::init_members();
    this->GModelSpatialRadial::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone instance
 *
 * @return Pointer to deep copy of model.
 ***************************************************************************/
GModelSpatialRadialShell* GModelSpatialRadialShell::clone(void) const
{
    return new GModelSpatialRadialShell(*this);
}


/***********************************************************************//**
 * @brief Evaluate function
 *
 * @param[in] theta Angular distance from shell centre (radians).
 *
 * Evaluates the spatial part for a shell source model. The shell source
 * model is a radial function \f$f(\theta)\f$, where \f$\theta\f$ is the
 * angular separation between shell centre and the actual location.
 *
 * In the small angle approximation,
 * \f[
 * f(\theta) = {\tt m\_norm} \left \{
 *  \begin{array}{l l}
 *     \displaystyle
 *     \sqrt{ \theta_{\rm out}^2 - \theta^2 } -
 *     \sqrt{ \theta_{\rm in}^2  - \theta^2 }
 *     & \mbox{if $\theta \le \theta_{\rm in}$} \\
 *     \\
 *    \displaystyle
 *     \sqrt{ \theta_{\rm out}^2 - \theta^2 }
 *     & \mbox{if $\theta_{\rm in} < \theta \le \theta_{\rm out}$} \\
 *     \\
 *    \displaystyle
 *    0 & \mbox{if $\theta > \theta_{\rm out}$}
 *  \end{array}
 *  \right .
 * \f]
 * where \f${\tt m\_norm}\f$ is a normalization constant (see the update()
 * method).
 *
 * In the general form that is also valid for large angles
 * \f[
 * f(\theta) = {\tt m\_norm} \left \{
 *  \begin{array}{l l}
 *     \displaystyle
 *     \sqrt{ \sin^2 \theta_{\rm out} - \sin^2 \theta } -
 *     \sqrt{ \sin^2 \theta_{\rm in}  - \sin^2 \theta }
 *     & \mbox{if $\theta \le \theta_{\rm in}$} \\
 *     \\
 *    \displaystyle
 *     \sqrt{ \sin^2 \theta_{\rm out} - \sin^2 \theta }
 *     & \mbox{if $\theta_{\rm in} < \theta \le \theta_{\rm out}$} \\
 *     \\
 *    \displaystyle
 *    0 & \mbox{if $\theta > \theta_{\rm out}$}
 *  \end{array}
 *  \right .
 * \f]
 * 
 * Here, \f$\theta_{\rm in}\f$ and \f$\theta_{\rm out}\f$ are the shell inner
 * and outer radius.
 ***************************************************************************/
double GModelSpatialRadialShell::eval(const double& theta) const
{
    // Update precomputation cache
    update();

    // Set x appropriately for the small angle approximation or not
    double x;
    if (m_small_angle) {
        x = theta * theta;
    }
    else {
        x  = std::sin(theta);
        x *= x;
    }

    // Compute value
    double value = 0.0;
    if (x < m_x_out) {
        value = std::sqrt(m_x_out - x);
        if (x < m_x_in) {
            value -= std::sqrt(m_x_in - x);
        }
    }
    
    // Normalise value
    double result = m_norm * value;

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (isnotanumber(result) || isinfinite(result)) {
        std::cout << "*** ERROR: GModelSpatialRadialShell::eval";
        std::cout << "(theta=" << theta << "): NaN/Inf encountered";
        std::cout << " (result=" << result;
        std::cout << ", m_norm=" << m_norm;
        std::cout << ", value=" << value;
        std::cout << ", x=" << x;
        std::cout << ", m_x_out=" << m_x_out;
        std::cout << ", m_x_in=" << m_x_in;
        std::cout << ", m_small_angle=" << m_small_angle;
        std::cout << ")" << std::endl;
    }
    #endif

    // Return normalised value
    return result;
}


/***********************************************************************//**
 * @brief Evaluate function and gradients
 *
 * @param[in] theta Angular distance from shell centre (radians).
 *
 * This method simply calls GModelSpatialRadialShell::eval() as no analytical
 * gradients will be computed. See GModelSpatialRadialShell::eval() for details
 * about the implemented method.
 ***************************************************************************/
double GModelSpatialRadialShell::eval_gradients(const double& theta) const
{
    // Return value
    return (eval(theta));
}


/***********************************************************************//**
 * @brief Returns MC sky direction
 *
 * @param[in] ran Random number generator.
 *
 * Draws an arbitrary sky position from the 2D shell distribution.
 ***************************************************************************/
GSkyDir GModelSpatialRadialShell::mc(GRan& ran) const
{
    // Update precomputation cache
    update();

    // Simulate offset from photon arrival direction
    #if defined(G_DEBUG_MC)
    int    n_samples = 0;
    #endif
    double theta_max     = this->theta_max();
    double sin_theta_max = std::sin(theta_max);
    double u_max         = (m_small_angle) ? m_norm * sin_theta_max * theta_max
                                           : m_norm * sin_theta_max * sin_theta_max;
    double value         = 0.0;
    double u             = 1.0;
    double theta         = 0.0;
    do {
        theta = ran.uniform() * theta_max;
        value = eval(theta) * std::sin(theta);
        u     = ran.uniform() * u_max;
        #if defined(G_DEBUG_MC)
        n_samples++;
        #endif
    } while (u > value);
    #if defined(G_DEBUG_MC)
    std::cout << "#=" << n_samples << " ";
    #endif

    // Simulate azimuth angle
    double phi = 360.0 * ran.uniform();

    // Rotate pointing direction by offset and azimuth angle
    GSkyDir sky_dir = dir();
    sky_dir.rotate_deg(phi, theta*rad2deg);

    // Return sky direction
    return sky_dir;
}


/***********************************************************************//**
 * @brief Return maximum model radius (in radians)
 ***************************************************************************/
double GModelSpatialRadialShell::theta_max(void) const
{
    // Return value
    return ((radius()+width())*deg2rad);
}


/***********************************************************************//**
 * @brief Read model from XML element
 *
 * @param[in] xml XML element.
 *
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Read the point source information from an XML element. The XML element
 * is required to have 4 parameters.
 * The position is named either "RA" and "DEC" or "GLON" and "GLAT", the
 * shell inner and outer radius are named "Radius" and "Width".
 *
 * @todo Implement tests of the radius and radius boundary and the width and
 *       width boundary. The radius and radius boundary should be >=0, the
 *       width and width boundary should be >0.
 ***************************************************************************/
void GModelSpatialRadialShell::read(const GXmlElement& xml)
{
    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Verify that XML element has exactly 4 parameters
    if (xml.elements() != 4 || npars != 4) {
        throw GException::model_invalid_parnum(G_READ, xml,
              "Shell model requires exactly 4 parameters.");
    }

    // Read shell location
    GModelSpatialRadial::read(xml);

    // Extract remaining model parameters. Note that we have to loop over
    // all parameters to find the remaining 2 parameters that are
    // implemented by this class (the location is implemented by the base
    // class)
    int npar[2] = {0, 0};
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        const GXmlElement* par = xml.element("parameter", i);

        // Handle Radius
        if (par->attribute("name") == "Radius") {
            
            // Read parameter
            m_radius.read(*par);
            
            //TODO: Check parameter
            
            // Increment parameter counter
            npar[0]++;
        }

        // Handle Width
        else if (par->attribute("name") == "Width") {
            
            // Read parameter
            m_width.read(*par);
            
            //TODO: Check parameter
            
            // Increment parameter counter
            npar[1]++;
        }

    } // endfor: looped over all parameters

    // Verify that all parameters were found
    if (npar[0] != 1 || npar[1] != 1) {
        throw GException::model_invalid_parnames(G_READ, xml,
              "Require \"Radius\" and \"Width\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write model into XML element
 *
 * @param[in] xml XML element into which model information is written.
 *
 * @exception GException::model_invalid_spatial
 *            Existing XML element is not of type 'GaussFunction'
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Write the radial shell source information into an XML element. The XML
 * element will have 4 parameter leafs named "RA", "DEC", "Radius" and 
 * "Width". The location leafs are handled by the GModelRadial base class.
 ***************************************************************************/
void GModelSpatialRadialShell::write(GXmlElement& xml) const
{
    // Write shell location
    GModelSpatialRadial::write(xml);

    // If XML element has 2 nodes (which should be the location nodes)
    // then append 2 parameter nodes
    if (xml.elements() == 2) {
        xml.append(GXmlElement("parameter name=\"Radius\""));
        xml.append(GXmlElement("parameter name=\"Width\""));
    }

    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Verify that XML element has exactly 4 parameters
    if (xml.elements() != 4 || npars != 4) {
        throw GException::model_invalid_parnum(G_WRITE, xml,
              "Shell source model requires exactly 4 parameters.");
    }

    // Set or update model parameter attributes
    int npar[2] = {0, 0};
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        GXmlElement* par = xml.element("parameter", i);

        // Handle Radius
        if (par->attribute("name") == "Radius") {
            m_radius.write(*par);
            npar[0]++;
        }

        // Handle Width
        else if (par->attribute("name") == "Width") {
            m_width.write(*par);
            npar[1]++;
        }

    } // endfor: looped over all parameters

    // Check of all required parameters are present
    if (npar[0] != 1 || npar[1] != 1) {
        throw GException::model_invalid_parnames(G_WRITE, xml,
              "Require \"Radius\" and \"Width\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print information
 *
 * @return String containing model information.
 ***************************************************************************/
std::string GModelSpatialRadialShell::print(void) const
{
    // Initialise result string
    std::string result;

    // Append header
    result.append("=== GModelSpatialRadialShell ===\n");

    // Append parameters
    result.append(parformat("Number of parameters")+str(size()));
    for (int i = 0; i < size(); ++i) {
        result.append("\n"+m_pars[i]->print());
    }

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpatialRadialShell::init_members(void)
{
    // Initialise Radius
    m_radius.clear();
    m_radius.name("Radius");
    m_radius.unit("deg");
    m_radius.value(0.0);
    m_radius.min(0.0);
    m_radius.free();
    m_radius.scale(1.0);
    m_radius.gradient(0.0);
    m_radius.hasgrad(false);  // Radial components never have gradients

    // Initialise Width
    m_width.clear();
    m_width.name("Width");
    m_width.unit("deg");
    m_width.value(2.778e-4);
    m_width.min(2.778e-4);   // 1 arcsec
    m_width.free();
    m_width.scale(1.0);
    m_width.gradient(0.0);
    m_width.hasgrad(false);  // Radial components never have gradients

    // Set parameter pointer(s)
    m_pars.push_back(&m_radius);
    m_pars.push_back(&m_width);

    // Initialise other members
    m_small_angle = false;

    // Initialise precomputation cache. Note that zero values flag
    // uninitialised as a zero radius and width shell is not meaningful
    m_last_radius = 0.0;
    m_last_width  = 0.0;
    m_theta_in    = 0.0;
    m_x_in        = 0.0;
    m_theta_out   = 0.0;
    m_x_out       = 0.0;
    m_norm        = 0.0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model Radial shell source model.
 *
 * We do not have to push back the members on the parameter stack as this
 * should have been done by init_members() that was called before. Otherwise
 * we would have the radius and width twice on the stack.
 ***************************************************************************/
void GModelSpatialRadialShell::copy_members(const GModelSpatialRadialShell& model)
{
    // Copy members
    m_radius      = model.m_radius;
    m_width       = model.m_width;
    m_small_angle = model.m_small_angle;

    // Copy precomputation cache
    m_last_radius = model.m_last_radius;
    m_last_width  = model.m_last_width;
    m_theta_in    = model.m_theta_in;
    m_x_in        = model.m_x_in;
    m_theta_out   = model.m_theta_out;
    m_x_out       = model.m_x_out;
    m_norm        = model.m_norm;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpatialRadialShell::free_members(void)
{
    // Return
    return;
}


/***********************************************************************//**
 * @brief Update precomputation cache
 *
 * Performs precomputations that are valid for a given pair of radius and
 * width values. The following members are set by this method:
 * m_last_radius, m_last_width, m_theta_in, m_theta_out, m_x_in, m_x_out
 * and m_norm.
 *
 * m_theta_in contains the inner shell radius in radians, while m_theta_out
 * contains the outer shell radius in radians.
 *
 * In the small angle approximation, 
 * \f${\tt m\_x\_in} = {\tt m\_theta\_in}^2\f$,
 * \f${\tt m\_x\_out} = {\tt m\_theta\_out}^2\f$, and
 * \f[{\tt m\_norm} = \frac{3}{2 \pi}
 *    \frac{1}{\left( {\tt m\_theta\_out}^3 - {\tt m\_theta\_in}^3 \right)}\f].
 *
 * In the general case that is also valid for large angles,
 * \f${\tt m\_x\_in} = \sin^2 {\tt m\_theta\_in}\f$,
 * \f${\tt m\_x\_out} = \sin^2 {\tt m\_theta\_out}\f$, and
 * \f[{\tt m\_norm} = \frac{1}{2 \pi}
 *    \frac{\sqrt{1-\cos 2 {\tt m\_theta\_out}} -
 *          \sqrt{1-\cos 2 {\tt m\_theta\_in}}}{2 \sqrt{2}} +
 *    \frac{1+\cos 2 {\tt m\_theta\_out}}{4} \ln \left(
 *          \frac{\sqrt{2} \cos {\tt m\_theta\_out}}
 *               {\sqrt{2} + \sqrt{1 - \cos 2 {\tt m\_theta\_out}}} \right) -
 *    \frac{1+\cos 2 {\tt m\_theta\_in}}{4} \ln \left(
 *          \frac{\sqrt{2} \cos {\tt m\_theta\_in}}
 *               {\sqrt{2} + \sqrt{1 - \cos 2 {\tt m\_theta\_in}}} \right)\f]
 ***************************************************************************/
void GModelSpatialRadialShell::update() const
{
    // Set constants
    const double c1 = twopi / 3.0;
    const double c2 = 1.0 / (2.0 * sqrt_two);

    // Update if radius or width have changed
    if (m_last_radius != radius() || m_last_width != width()) {

        // Store last values
        m_last_radius = radius();
        m_last_width  = width();
        
        // Perform precomputations
        m_theta_in   = radius() * deg2rad;
        m_theta_out  = (radius() + width()) * deg2rad;
        if (m_small_angle) {
            m_x_in       = m_theta_in  * m_theta_in;
            m_x_out      = m_theta_out * m_theta_out;
            double denom = c1 * (m_x_out*m_theta_out - m_x_in*m_theta_in);
            m_norm       = (denom > 0.0) ? 1.0 / denom : 0.0;
        } 
        else {
            double sin_theta_in  = std::sin(m_theta_in);
            double sin_theta_out = std::sin(m_theta_out);
            double term1         = (f1(m_theta_out) - f1(m_theta_in)) * c2;
            double term2         = f2(m_theta_out);
            double term3         = f2(m_theta_in);
            double denom         = twopi * (term1 + term2 - term3);
            m_norm               = (denom > 0.0) ? 1.0 / denom : 0.0;
            m_x_in               = sin_theta_in*sin_theta_in;
            m_x_out              = sin_theta_out * sin_theta_out;
        }

    } // endif: update required
    
    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (isnotanumber(m_norm) || isinfinite(m_norm)) {
        std::cout << "*** ERROR: GModelSpatialRadialShell::update:";
        std::cout << " NaN/Inf encountered";
        std::cout << " (m_norm=" << m_norm;
        std::cout << ", radius=" << radius();
        std::cout << ", width=" << width();
        std::cout << ", m_theta_in=" << m_theta_in;
        std::cout << ", m_theta_out=" << m_theta_out;
        std::cout << ", term1=" << (f1(m_theta_out) - f1(m_theta_in)) * c2;
        std::cout << ", term2=" << f2(m_theta_out);
        std::cout << ", term3=" << f2(m_theta_in);
        std::cout << ")" << std::endl;
    }
    #endif

    // Return
    return;
}


/***********************************************************************//**
 * @brief Return function 1 value needed for precomputation
 *
 * Computes \f$f1(x) = \sqrt{1 - \cos 2 x}\f$.
 ***************************************************************************/
double GModelSpatialRadialShell::f1(double x)
{
    // Compute value
    double f1 = std::sqrt(1.0 - std::cos(2.0 * x));

    // Return value
    return f1;
}


/***********************************************************************//**
 * @brief Return function 2 value needed for precomputation
 *
 * Compute
 * \f[f2(x) = \frac{1+\cos 2x}{4} 
 *    \ln \left( \frac{\sqrt{2} \cos x}{\sqrt{2} + \sqrt{ 1 - \cos 2 x}}
 *        \right)\f].
 ***************************************************************************/
double GModelSpatialRadialShell::f2(double x)
{
    // Compute value
    double t1 = (1.0 + std::cos(2.0*x)) / 4.0;
    double t2 = sqrt_two * std::cos(x);
    double t3 = sqrt_two + f1(x);
    double f2 = t1 * std::log(t2 / t3);

    // Return value
    return f2;
}
