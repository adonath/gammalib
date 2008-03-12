/***************************************************************************
 *          GLATAeff.cpp  -  GLAST LAT Response class Aeff methods         *
 * ----------------------------------------------------------------------- *
 *  copyright            : (C) 2008 by Jurgen Knodlseder                   *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
/**
 * @file GLATAeff.cpp
 * @brief GLATResponse class effective area implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#include <iostream>
#include "GTools.hpp"
#include "GException.hpp"
#include "GLATResponse.hpp"
#include "GFitsDblImage.hpp"
#include "GFitsTableFltCol.hpp"

/* __ Namespaces _________________________________________________________ */

/* __ Method name definitions ____________________________________________ */
#define G_INIT_AEFF "GLATResponse::init_aeff()"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */

/* __ Constants __________________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Return effective area
 *
 * @param[in] obsDir Observed photon direction
 * @param[in] obsEng Observed energy of photon
 * @param[in] srcDir True photon direction
 * @param[in] srcEng True energy of photon
 * @param[in] instPntDir Instrument pointing direction (e.g. z-axis)
 * @param[in] instPosAng Instrument position angle
 * @param[in] time Photon arrival time
 ***************************************************************************/
double GLATResponse::aeff(const GSkyDir& obsDir, const double& obsEng,
                          const GSkyDir& srcDir, const double& srcEng,
                          const GSkyDir& instPntDir, const double& instPosAng,
                          const double& time)
{
    // Return Aeff value
    return 0.0; //!< @todo Not yet implemented
}


/***********************************************************************//**
 * @brief Return effective area in units of cm2
 *
 * @param[in] logE Logarithm of the true photon energy (MeV).
 * @param[in] ctheta Cosine of the zenith angle with respect to the pointing 
 *            axis.
 ***************************************************************************/
double GLATResponse::aeff(const double& logE, const double& ctheta)
{
    // Get effective area value
    double aeff = (ctheta >= m_aeff_ctheta_min) 
                  ? m_aeff_bins.interpolate(logE, ctheta, m_aeff) : 0.0;

    // Return effective area value
    return aeff;
}


/***********************************************************************//**
 * @brief Set minimum cos(theta) angle for effective area access
 *
 * @param[in] ctheta Cosine of the maximum zenith angle for which effective
 *                   areas will be returned (0 is returned for larger values)
 ***************************************************************************/
void GLATResponse::aeff_ctheta_min(const double& ctheta)
{
    // Set minimum cos(theta) value
    m_aeff_ctheta_min = ctheta;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Get minimum cos(theta) angle for effective area access
 ***************************************************************************/
double GLATResponse::aeff_ctheta_min(void) const
{
    // Return minimum cos(theta) value
    return m_aeff_ctheta_min;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise effective area
 *
 * The effective area is converted into units of cm2.
 ***************************************************************************/
void GLATResponse::aeff_init(void)
{
    // Build filename
    std::string filename = "aeff_"  + m_rspname + "_" + m_rsptype + ".fits";

    // Open FITS file
    GFits file;
    file.open(m_caldb + "/" + filename);

    // Get pointer to effective area HDU
    GFitsHDU* hdu = file.hdu("EFFECTIVE AREA");
    if (hdu == NULL)
        throw GException::fits_hdu_not_found(G_INIT_AEFF, "EFFECTIVE AREA");

    // Get the energy and cos(theta) bins
    m_aeff_bins.load(hdu);

    // Allocate memory for effective area
    int size = m_aeff_bins.num_energy() * m_aeff_bins.num_ctheta();
    if (size > 0)
        m_aeff = new double[size];

    // Get pointer to effective area column
    GFitsTableCol* ptr = hdu->column("EFFAREA");
    if (ptr == NULL)
        throw GException::fits_column_not_found(G_INIT_AEFF, "EFFAREA");

    // Copy data and convert from m2 into cm2
    int num = ptr->number();
    for (int i = 0; i < num; ++i)
        m_aeff[i] = ptr->real(0,i) * 1.0e4;

    // Set minimum cos(theta)
    m_aeff_ctheta_min = m_aeff_bins.ctheta_lo(0);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Append Aeff HDUs tu FOTS file
 *
 * @param[in] file FITS file into which the Aeff HDUs will be appended
 *
 * Append 2 HDUs to the FITS file:
 * ABOUNDS (Aeff energy and zenith angle boundaries)
 * AEFF (Aeff array)
 ***************************************************************************/
void GLATResponse::aeff_append(GFits& file) const
{
    // Get Aeff boundary table
    GFitsHDU hdu_bounds;
    m_aeff_bins.save(&hdu_bounds);
    hdu_bounds.extname("ABOUNDS");

    // Build effective area image
    int naxes[] = {m_aeff_bins.num_energy(), m_aeff_bins.num_ctheta()};
    GFitsDblImage image_aeff(2, naxes, m_aeff);

    // Construct effective area HDU
    GFitsHDU hdu_aeff(image_aeff);
    hdu_aeff.extname("AEFF");
    hdu_aeff.header()->update(GFitsHeaderCard("CTYPE1", "Energy", "Energy binning"));
    hdu_aeff.header()->update(GFitsHeaderCard("CTYPE2", "cos(theta)", "cos(theta) binning"));
    hdu_aeff.header()->update(GFitsHeaderCard("BUNIT", "cm2", "Pixel unit"));

    // Append HDUs to FITS file
    file.append_hdu(hdu_bounds);
    file.append_hdu(hdu_aeff);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GLATResponse::aeff_init_members(void)
{
    // Initialise Aeff members
    m_aeff_ctheta_min = 0.0;
    m_aeff            = NULL;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 ***************************************************************************/
void GLATResponse::aeff_copy_members(const GLATResponse& rsp)
{
    // Copy attributes
    m_aeff_bins       = rsp.m_aeff_bins;
    m_aeff_ctheta_min = rsp.m_aeff_ctheta_min;

    // Copy effective area array
    if (rsp.m_aeff != NULL) {
        int size = m_aeff_bins.num_energy() * m_aeff_bins.num_ctheta();
        if (size > 0) {
            m_aeff = new double[size];
            memcpy(m_aeff, rsp.m_aeff, size*sizeof(double));
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
***************************************************************************/
void GLATResponse::aeff_free_members(void)
{
    // Free effective area memory
    if (m_aeff != NULL) delete [] m_aeff;

    // Signal that effective area memory is free
    m_aeff = NULL;

    // Return
    return;
}
