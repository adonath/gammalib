/***************************************************************************
 *    GModelSpectralLogParabola.cpp - Log parabola spectral model class    *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2012 by Michael Mayer                                    *
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
 * @file GModelSpectralLogParabola.cpp
 * @brief Log parabola spectral model class definition
 * @author Michael Mayer 
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cmath>
#include "GException.hpp"
#include "GTools.hpp"
#include "GModelSpectralLogParabola.hpp"
#include "GModelSpectralRegistry.hpp"

/* __ Constants __________________________________________________________ */

/* __ Globals ____________________________________________________________ */
const GModelSpectralLogParabola g_spectral_logparabola_seed;
const GModelSpectralRegistry    g_spectral_logparabola_registry(&g_spectral_logparabola_seed);

/* __ Method name definitions ____________________________________________ */
#define G_FLUX          "GModelSpectralLogParabola::flux(GEnergy&, GEnergy&)"
#define G_EFLUX        "GModelSpectralLogParabola::eflux(GEnergy&, GEnergy&)"
#define G_MC       "GModelSpectralLogParabola::mc(GEnergy&, GEnergy&, GRan&)"
#define G_READ                "GModelSpectralLogParabola::read(GXmlElement&)"
#define G_WRITE              "GModelSpectralLogParabola::write(GXmlElement&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModelSpectralLogParabola::GModelSpectralLogParabola(void) : GModelSpectral()
{
    // Initialise private members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Constructor
 *
 * @param[in] norm Power law normalization.
 * @param[in] index Power law index.
 * @param[in] curvature Curvature.
 *
 * Construct a LogParabola model from a normalization value, a spectral
 * index and a curvature
 ***************************************************************************/
GModelSpectralLogParabola::GModelSpectralLogParabola(const double& norm,
                                                     const double& index,
                                                     const double& curvature) :
                           GModelSpectral()
{
    // Initialise members
    init_members();

    // Set parameters
    m_norm.real_value(norm);
    m_index.real_value(index);
    m_curvature.real_value(curvature);

    // Return
    return;
}


/***********************************************************************//**
 * @brief XML constructor
 *
 * @param[in] xml XML element.
 *
 * Creates instance of a log parabola spectral model by extracting information
 * from an XML element. See GModelSpectralLogParabola::read() for more information
 * about the expected structure of the XML element.
 ***************************************************************************/
GModelSpectralLogParabola::GModelSpectralLogParabola(const GXmlElement& xml) : 
                           GModelSpectral()
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
 * @param[in] model LogParabola model.
 ***************************************************************************/
GModelSpectralLogParabola::GModelSpectralLogParabola(const GModelSpectralLogParabola& model) :
                           GModelSpectral(model)
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
GModelSpectralLogParabola::~GModelSpectralLogParabola(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                Operators                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] model LogParabola model.
 * @return LogParabola model.
 ***************************************************************************/
GModelSpectralLogParabola& GModelSpectralLogParabola::operator=(const GModelSpectralLogParabola& model)
{
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelSpectral::operator=(model);

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
 =                              Public methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear instance
 ***************************************************************************/
void GModelSpectralLogParabola::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GModelSpectral::free_members();

    // Initialise members
    this->GModelSpectral::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone instance
 *
 * @return Pointer to deep copy of log parabola spectrum.
 ***************************************************************************/
GModelSpectralLogParabola* GModelSpectralLogParabola::clone(void) const
{
    return new GModelSpectralLogParabola(*this);
}


/***********************************************************************//**
 * @brief Evaluate function
 *
 * @param[in] srcEng True energy of photon.
 * @return Function value.
 *
 * The logparabola function is defined as
 * \f[I(E)=norm (E/pivot)^{index-curvature*log(E/pivot)}\f]
 * where
 * \f$norm\f$ is the normalization or prefactor,
 * \f$pivot\f$ is the pivot energy, and
 * \f$index\f$ is the spectral index.
 * \f$curvature\f$ is the curvature
 *
 * @todo For the moment the pivot energy is fixed to units of MeV. This may
 * not be ideal and should eventually be improved in the future.
 * Furthermore, the method expects that energy!=0. Otherwise Inf or NaN
 * may result.
 ***************************************************************************/
double GModelSpectralLogParabola::eval(const GEnergy& srcEng) const
{
    // Compute function value
    double energy   = srcEng.MeV() / pivot();
    double exponent = index() + curvature()*log(energy);
    double power    = std::pow(energy,exponent);
    double value    = norm() * power;

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (isnotanumber(value) || isinfinite(value)) {
        std::cout << "*** ERROR: GModelSpectralLogParabola::eval";
        std::cout << "(srcEng=" << srcEng << "):";
        std::cout << " NaN/Inf encountered";
        std::cout << " (value=" << value;
        std::cout << ", energy=" << energy;
        std::cout << ", index=" << index();
        std::cout << ", curvature=" << curvature();
        std::cout << ", pivot=" << pivot();
        std::cout << ", power=" << power;
        std::cout << ")" << std::endl;
    }
    #endif

    // Return
    return value;
}


/***********************************************************************//**
 * @brief Evaluate function and gradients
 *
 * @param[in] srcEng True energy of photon.
 * @return Function value.
 *
 * The logparabol function is defined as
 * \f[I(E)=norm (E/pivot)^{index-curvature\ln(E/pivot)}\f]
 * where
 * \f$norm=n_s n_v\f$ is the normalization or prefactor,
 * \f$pivot=p_s p_v\f$ is the pivot energy, and
 * \f$index=i_s i_v\f$ is the spectral index.
 * \f$curvature=i_s i_v\f$ is the curvature
 * Note that each parameter is factorised into a scaling factor and a value
 * and that the method is expected to return the gradient with respect to
 * the parameter value (i.e. n_v, p_v, i_v, and c_v in this case).
 *
 * The partial derivatives of the parameter values are given by
 *@todo implement formulae here
 *
 * @todo For the moment the pivot energy is fixed to units of MeV. This may
 * not be ideal and should eventually be improved in the futur.
 * Furthermore, the method expects that energy!=0. Otherwise Inf or NaN
 * may result.
 ***************************************************************************/
double GModelSpectralLogParabola::eval_gradients(const GEnergy& srcEng) const
{
    // Compute function value
    double energy   = srcEng.MeV() / pivot();
    double exponent = index() + curvature()*std::log(energy);
    double power    = std::pow(energy,exponent);
    double value    = norm() * power;

    // Compute partial derivatives of the parameter values
    double log_energy  = std::log(energy);
    
    double g_norm      = (m_norm.isfree())  ? m_norm.scale() * power : 0.0;
    double g_index     = (m_index.isfree())
                         ? value * m_index.scale() * log_energy : 0.0;
    double g_curvature = (m_curvature.isfree())
                         ? value * m_curvature.scale() * log_energy * 
                           log_energy
                         : 0.0;
    double g_pivot     = (m_pivot.isfree())
                         ? -value/m_pivot.value() * (exponent + curvature() *
                           log_energy)
                         : 0.0;

    // Set gradients (circumvent const correctness)
    const_cast<GModelSpectralLogParabola*>(this)->m_norm.gradient(g_norm);
    const_cast<GModelSpectralLogParabola*>(this)->m_index.gradient(g_index);
    const_cast<GModelSpectralLogParabola*>(this)->m_curvature.gradient(g_curvature);
    const_cast<GModelSpectralLogParabola*>(this)->m_pivot.gradient(g_pivot);

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (isnotanumber(value) || isinfinite(value)) {
        std::cout << "*** ERROR: GModelSpectralLogParabola::eval_gradients";
        std::cout << "(srcEng=" << srcEng << "):";
        std::cout << " NaN/Inf encountered";
        std::cout << " (value=" << value;
        std::cout << ", energy=" << energy;
        std::cout << ", index=" << index();
        std::cout << ", curvature=" << curvature();
        std::cout << ", pivot=" << pivot();
        std::cout << ", power=" << power;
        std::cout << ", g_norm=" << g_norm;
        std::cout << ", g_index=" << g_index;
        std::cout << ", g_curvature=" << g_curvature;
        std::cout << ", g_pivot=" << g_pivot;
        std::cout << ")" << std::endl;
    }
    #endif

    // Return
    return value;
}


/***********************************************************************//**
 * @brief Returns model photon flux between [emin, emax] (units: ph/cm2/s)
 *
 * @param[in] emin Minimum photon energy.
 * @param[in] emax Maximum photon energy.
 * @return Photon flux (ph/cm2/s).
 *
 * Computes
 * \f[\int_{E_{\rm min}}^{E_{\rm max}} I(E) dE\f]
 * where
 * \f$E_{\rm min}\f$ and \f$E_{\rm max}\f$ are the minimum and maximum
 * energy, respectively, and
 * \f$I(E)\f$ is the spectral model (units: ph/cm2/s/MeV).
 * The integration is done numerically.
 ***************************************************************************/
double GModelSpectralLogParabola::flux(const GEnergy& emin,
                                       const GEnergy& emax) const
{
	// Initialise function to integrate
    flux_kern flux(norm(),index(),curvature(),pivot());

    // Initialise integral class with function
    GIntegral integral(&flux);

    // Set integration precision
    integral.eps(1.0e-8);

    // Calculate integral between emin and emax
    double photonflux = integral.romb(emin.MeV(), emax.MeV());

    //Return value
    return photonflux;
}


/***********************************************************************//**
 * @brief Returns model energy flux between [emin, emax] (units: erg/cm2/s)
 *
 * @param[in] emin Minimum photon energy.
 * @param[in] emax Maximum photon energy.
 * @return Energy flux (erg/cm2/s).
 *
 * Computes
 * \f[\int_{E_{\rm min}}^{E_{\rm max}} I(E) E dE\f]
 * where
 * \f$E_{\rm min}\f$ and \f$E_{\rm max}\f$ are the minimum and maximum
 * energy, respectively, and
 * \f$I(E)\f$ is the spectral model (units: ph/cm2/s/MeV).
 * The integration is done numerically.
 ***************************************************************************/
double GModelSpectralLogParabola::eflux(const GEnergy& emin,
                                        const GEnergy& emax) const
{
	// Initialise function to integrate
    eflux_kern eflux(norm(),index(),curvature(),pivot());

    // Initialise integral class with function
    GIntegral integral(&eflux);

    // Set integration precision
    integral.eps(1.0e-8);

    // Calculate integral between emin and emax
    double energyflux = integral.romb(emin.MeV(), emax.MeV());

    // Return value
    return energyflux;
}


/***********************************************************************//**
 * @brief Returns Monte Carlo energy between [emin, emax]
 *
 * @param[in] emin Minimum photon energy.
 * @param[in] emax Maximum photon energy.
 * @param[in] ran Random number generator.
 * @return Energy.
 *
 * @exception GException::feature_not_implemented
 *            Feature not yet implemented.
 *
 * Returns Monte Carlo energy by randomly drawing from a power law.
 *
 * @todo To be implemented.
 ***************************************************************************/
GEnergy GModelSpectralLogParabola::mc(const GEnergy& emin,
                                      const GEnergy& emax,
                                      GRan& ran) const
{
    // Allocate energy
    GEnergy energy;

    // Throw exception signalling that method is not yet implemented
    throw GException::feature_not_implemented(G_MC);

    // Return energy
    return energy;
}


/***********************************************************************//**
 * @brief Autoscale normalization
 *
 * Based on the actual value of the m_norm parameter, set the scale of m_norm
 * so that the value will be 1. If minimum and/or maximum value boundaries
 * exist, the boundaries are also modified accordingly.
 ***************************************************************************/
void GModelSpectralLogParabola::autoscale(void)
{
    // Autoscale normalization to a value of 1.0
    if (m_norm.value() != 0.0) {

        // Get inverse scaling factor
        double invscale = 1.0 / m_norm.value();

        // Set values, error, min and max
        m_norm.value(m_norm.value() * invscale);
        m_norm.error(m_norm.error() * invscale);
        if (m_norm.hasmin()) {
            m_norm.min(m_norm.min() * invscale);
        }
        if (m_norm.hasmax()) {
            m_norm.max(m_norm.max() * invscale);
        }

        // Set scale
        m_norm.scale(1.0 / invscale);

    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read model from XML element
 *
 * @param[in] xml XML element containing logparabola model information.
 *
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Read the spectral log parabola information from an XML element. The format
 * of the XML elements is
 *
 *     <spectrum type="LogParabola">
 *       <parameter name="Prefactor" scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Index"     scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Curvature" scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Scale"     scale=".." value=".." min=".." max=".." free=".."/>
 *     </spectrum>
 *
 * or for compliance with Fermi-LAT
 *
 *     <spectrum type="LogParabola">
 *       <parameter name="norm"  scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="alpha" scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="beta"  scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Eb"    scale=".." value=".." min=".." max=".." free=".."/>
 *     </spectrum>
 *
 * @todo Add parameter validity check
 ***************************************************************************/
void GModelSpectralLogParabola::read(const GXmlElement& xml)
{
    // Verify that XML element has exactly 3 parameters
    if (xml.elements() != 4 || xml.elements("parameter") != 4) {
        throw GException::model_invalid_parnum(G_READ, xml,
              "LogParabola model requires exactly 4 parameters.");
    }

    // Extract model parameters
    int npar[] = {0, 0, 0,0};
    for (int i = 0; i < 4; ++i) {

        // Get parameter element
        GXmlElement* par = static_cast<GXmlElement*>(xml.element("parameter", i));

        // Handle prefactor
        if (par->attribute("name") == "Prefactor" ||
        	par->attribute("name") == "norm") {
            m_norm.read(*par);
            npar[0]++;
        }

        // Handle index
        else if (par->attribute("name") == "Index" ||
        		 par->attribute("name") == "alpha") {
            m_index.read(*par);
            npar[1]++;
        }

        // Handle index
        else if (par->attribute("name") == "Curvature" ||
        		 par->attribute("name") == "beta") {
            m_curvature.read(*par);
            npar[2]++;
        }

        // Handle pivot energy
        else if (par->attribute("name") == "Scale" ||
			     par->attribute("name") == "Eb") {
            m_pivot.read(*par);
            npar[3]++;
        }

    } // endfor: looped over all parameters

    // Verify that all parameters were found
    if (npar[0] != 1 || npar[1] != 1 || npar[2] != 1 || npar[3]  != 1) {
        throw GException::model_invalid_parnames(G_READ, xml,
              "LogParabola requires \"Prefactor\" or \"norm\","
              " \"Index\" or \"alpha\", \"Curvature\" or \"beta\""
              " and \"Scale\" or \"Eb\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write model into XML element
 *
 * @param[in] xml XML element into which model information is written.
 *
 * @exception GException::model_invalid_spectral
 *            Existing XML element is not of type "LogParabola"
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters or nodes found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Write the LogParabola model information into an XML element. The format
 * of the XML elements is
 *
 *     <spectrum type="LogParabola">
 *       <parameter name="Prefactor" scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Index"     scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Curvature" scale=".." value=".." min=".." max=".." free=".."/>
 *       <parameter name="Scale"     scale=".." value=".." min=".." max=".." free=".."/>
 *     </spectrum>
 ***************************************************************************/
void GModelSpectralLogParabola::write(GXmlElement& xml) const
{
    // Set model type
    if (xml.attribute("type") == "") {
        xml.attribute("type", type());
    }

    // Verify model type
    if (xml.attribute("type") != type()) {
        throw GException::model_invalid_spectral(G_WRITE, xml.attribute("type"),
              "Spectral model is not of type \""+type()+"\".");
    }

    // If XML element has 0 nodes then append 4 parameter nodes
    if (xml.elements() == 0) {
        xml.append(new GXmlElement("parameter name=\"Prefactor\""));
        xml.append(new GXmlElement("parameter name=\"Index\""));
        xml.append(new GXmlElement("parameter name=\"Curvature\""));
        xml.append(new GXmlElement("parameter name=\"Scale\""));
    }

    // Verify that XML element has exactly 4 parameters
    if (xml.elements() != 4 || xml.elements("parameter") != 4) {
        throw GException::model_invalid_parnum(G_WRITE, xml,
              "LogParabola law model requires exactly 4 parameters.");
    }

    // Set or update model parameter attributes
    int npar[] = {0, 0, 0,0};
    for (int i = 0; i < 4; ++i) {

        // Get parameter element
        GXmlElement* par = static_cast<GXmlElement*>(xml.element("parameter", i));

        // Handle prefactor
        if (par->attribute("name") == "Prefactor"){
            npar[0]++;
            m_norm.write(*par);
        }

        // Handle index
        else if (par->attribute("name") == "Index") {
            npar[1]++;
            m_index.write(*par);
        }

        // Handle index
        else if (par->attribute("name") == "Curvature"){
              npar[2]++;
              m_curvature.write(*par);
        }

        // Handle pivot energy
        else if (par->attribute("name") == "Scale") {
            m_pivot.write(*par);
            npar[3]++;
        }

    } // endfor: looped over all parameters

    // Check of all required parameters are present
    if (npar[0] != 1 || npar[1] != 1 || npar[2] != 1 || npar[3] != 1) {
        throw GException::model_invalid_parnames(G_WRITE, xml,
              "LogParabola requires \"Prefactor\", \"Index\", \"Curvature\""
              " and \"Scale\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print powerlaw information
 ***************************************************************************/
std::string GModelSpectralLogParabola::print(void) const
{
    // Initialise result string
    std::string result;

    // Append header
    result.append("=== GModelSpectralLogParabola ===\n");

    // Append information
    result.append(parformat("Number of parameters")+str(size()));
    for (int i = 0; i < size(); ++i) {
        result.append("\n"+m_pars[i]->print());
    }

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpectralLogParabola::init_members(void)
{
    // Initialise powerlaw normalisation
    m_norm.clear();
    m_norm.name("Prefactor");
    m_norm.unit("ph/cm2/s/MeV");
    m_norm.scale(1.0);
    m_norm.value(1.0);          // default: 1.0
    m_norm.min(0.0);            // min:     0.0
    m_norm.free();
    m_norm.gradient(0.0);
    m_norm.hasgrad(true);

    // Initialise powerlaw index
    m_index.clear();
    m_index.name("Index");
    m_index.scale(1.0);
    m_index.value(-2.0);        // default: -2.0
    m_index.range(-10.0,+10.0); // range:   [-10,+10]
    m_index.free();
    m_index.gradient(0.0);
    m_index.hasgrad(true);

    m_curvature.clear();
    m_curvature.name("Curvature");
    m_curvature.scale(1.0);
    m_curvature.value(-.1);        // default: -2.0
    m_curvature.range(-10.0,+10.0); // range:   [-10,+10]
    m_curvature.free();
    m_curvature.gradient(0.0);
    m_curvature.hasgrad(true);

    // Initialise pivot energy
    m_pivot.clear();
    m_pivot.name("PivotEnergy");
    m_pivot.unit("MeV");
    m_pivot.scale(1.0);
    m_pivot.value(100.0);       // default: 100
    m_pivot.fix();
    m_pivot.gradient(0.0);
    m_pivot.hasgrad(true);

    // Set parameter pointer(s)
    m_pars.clear();
    m_pars.push_back(&m_norm);
    m_pars.push_back(&m_index);
    m_pars.push_back(&m_curvature);
    m_pars.push_back(&m_pivot);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model GModelSpectralPlaw members which should be copied.
 ***************************************************************************/
void GModelSpectralLogParabola::copy_members(const GModelSpectralLogParabola& model)
{
    // Copy members
    m_norm  = model.m_norm;
    m_index = model.m_index;
    m_curvature = model.m_curvature;
    m_pivot = model.m_pivot;

    // Set parameter pointer(s)
    m_pars.clear();
    m_pars.push_back(&m_norm);
    m_pars.push_back(&m_index);
    m_pars.push_back(&m_curvature);
    m_pars.push_back(&m_pivot);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpectralLogParabola::free_members(void)
{
    // Return
    return;
}
