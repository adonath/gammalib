/***************************************************************************
 *                GCaldb.cpp  -  Calibration database class                *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011 by Juergen Knoedlseder                              *
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
 * @file GCaldb.cpp
 * @brief Calibration database class implementation.
 * @author J. Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <unistd.h>        // access() function
#include <cstdlib>         // std::getenv() function
#include "GException.hpp"
#include "GTools.hpp"
#include "GCaldb.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_CALDB                                                    "GCaldb()"
#define G_SET_DATABASE                    "GCaldb::set_database(std::string)"
#define G_PATH                       "GCaldb::path(std::string, std::string)"
#define G_CIF                         "GCaldb::cif(std::string, std::string)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */



/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 *
 * @exception GException::env_not_found
 *            CALDB environment variable not found
 *
 * The void constructor determines the calibration database root directory
 * from the CALDB environment variable. If this environment variable is not
 * set the constructor will throw an exception.
 ***************************************************************************/
GCaldb::GCaldb(void)
{
    // Initialise class members
    init_members();
    
    // Get root directory from CALDB environment variable
    char* ptr = std::getenv("CALDB");
    
    // Throw an exception if the environment variable is not found
    if (ptr == NULL) {
        throw GException::env_not_found(G_CALDB, "CALDB",
              "Please set the CALDB environment variable to a valid"
              " calibration database root directory.");
    }

    // ... otherwise set calibration database
    set_database(std::string(ptr));

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] caldb Calibration database.
 ***************************************************************************/
GCaldb::GCaldb(const GCaldb& caldb)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(caldb);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Initalisation constructor
 *
 * @param[in] pathname Calibration database root directory.
 *
 * This constructor sets the calibration database using the specified root
 * directory. Any existing CALDB environment variable will be ignored.
 ***************************************************************************/
GCaldb::GCaldb(const std::string& pathname)
{
    // Initialise members
    init_members();

    // Set the root directory
    set_database(pathname);

    // Return
    return;
}



/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GCaldb::~GCaldb(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                              Operators                                  =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] caldb Calibration database.
 ***************************************************************************/
GCaldb& GCaldb::operator= (const GCaldb& caldb)
{
    // Execute only if object is not identical
    if (this != &caldb) {

        // Free members
        free_members();

        // Initialise private members
        init_members();

        // Copy members
        copy_members(caldb);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear calibration database
 ***************************************************************************/
void GCaldb::clear(void)
{
    // Free members
    free_members();

    // Initialise private members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone calibration database
 *
 * Cloning provides a copy of the calibration database.
 ***************************************************************************/
GCaldb* GCaldb::clone(void) const
{
    // Clone this image
    return new GCaldb(*this);
}


/***********************************************************************//**
 * @brief Returns number of entries in calibration database
 *
 * Cloning provides a copy of the calibration database.
 ***************************************************************************/
int GCaldb::size(void) const
{
    // Initialise size
    int size = 0;

    // If CIF is open then determine number of entries
    if (m_cif != NULL) {
        size = m_cif->nrows();
    }

    // Return size
    return size;
}


/***********************************************************************//**
 * @brief Set calibration database root directory
 *
 * @param[in] pathname Calibration database root directory.
 *
 * Set a new root directory for the calibration database.
 ***************************************************************************/
void GCaldb::dir(const std::string& pathname)
{
    // Clear any existing database
    clear();

    // Set calibration database
    set_database(pathname);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Open calibration database
 *
 * @param[in] mission Mission name (case insensitive).
 * @param[in] instrument Instrument name (case insensitive; optional).
 *
 * Opens the calibration database for a given mission and instrument.
 ***************************************************************************/
void GCaldb::open(const std::string& mission, const std::string& instrument)
{
    // Close any open database
    close();

    // Set full CIF filename (this also validates the mission and
    // instrument names)
    m_cifname = cifname(mission, instrument);

    // Store mission and instrument
    m_mission    = mission;
    m_instrument = instrument;

    // Open CIF FITS file
    m_fits.open(m_cifname);

    // Store pointer to first extension which holds the CIF table
    m_cif = m_fits.table(1);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Close calibration database
 *
 * Close a specific calibration database.
 ***************************************************************************/
void GCaldb::close(void)
{
    // Reset database parameters
    m_mission.clear();
    m_instrument.clear();
    m_cifname.clear();
    
    // Close CIF FITS file
    m_fits.close();

    // Reset CIF table pointer
    m_cif = NULL;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Print calibration database
 ***************************************************************************/
std::string GCaldb::print(void) const
{
    // Initialise result string
    std::string result;

    // Append Header
    result.append("=== GCaldb ===");
    result.append("\n"+parformat("Database root")+m_caldb);

    // Append information about opened database
    if (m_cif != NULL) {
        result.append("\n"+parformat("Selected Mission")+toupper(m_mission));
        result.append("\n"+parformat("Selected Instrument")+toupper(m_instrument));
        result.append("\n"+parformat("Calibration Index File")+m_cifname);
        result.append("\n"+parformat("Number of entries")+str(size()));
    }
    
    // Return
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
void GCaldb::init_members(void)
{
    // Initialise members
    m_caldb.clear();
    m_mission.clear();
    m_instrument.clear();
    m_cifname.clear();
    m_fits.clear();
    m_cif = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] caldb Calibration database.
 ***************************************************************************/
void GCaldb::copy_members(const GCaldb& caldb)
{
    // Copy attributes
    m_caldb      = caldb.m_caldb;
    m_mission    = caldb.m_mission;
    m_instrument = caldb.m_instrument;
    m_cifname    = caldb.m_cifname;
    m_fits       = caldb.m_fits;

    // Set CIF table pointer. We do not copy over the pointer as the FITS
    // file has been copied here.
    if (caldb.m_cif != NULL) {
        m_cif = m_fits.table(1);
    }
    else {
        m_cif = NULL;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GCaldb::free_members(void)
{
    // Close any open database
    close();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set calibration database
 *
 * @param[in] pathname Calibration database root directory.
 *
 * @exception GException::directory_not_found
 *            Calibration database root directory not found.
 * @exception GException::directory_not_accessible
 *            No read permission granted for calibration database root
 *            directory.
 *
 * Set calibration database to a specific root directory. The method verifies
 * the existence of the root directory prior to setting.
 ***************************************************************************/
void GCaldb::set_database(const std::string& pathname)
{
    // Verify that specified root directory exists
    if (access(pathname.c_str(), F_OK) != 0) {
        throw GException::directory_not_found(G_SET_DATABASE, pathname);
    }

    // Verify that specified root directory allows read access
    if (access(pathname.c_str(), R_OK) != 0) {
        throw GException::directory_not_accessible(G_SET_DATABASE, pathname,
              "Requested read permission not granted.");
    }

    // Set calibration database root directory
    m_caldb = pathname;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Return path to calibration directory
 *
 * @param[in] mission Mission name (case insensitive).
 * @param[in] instrument Instrument name (case insensitive; optional).
 *
 * @exception GException::directory_not_found
 *            Calibration directory not found.
 * @exception GException::directory_not_accessible
 *            No read permission granted to calibration directory.
 *
 * The calibration directory path is given by
 * $CALDB/data/<mission>
 * or
 * $CALDB/data/<mission>/<instrument>
 * where <mission> is the name of the mission and <instrument> is the
 * optional instrument name (all lower case). The arguments provided to the
 * method are transformed to lower case.
 ***************************************************************************/
std::string GCaldb::path(const std::string& mission, const std::string& instrument)
{
    // Verify that mission name is valid and directory is accessible
    std::string path = m_caldb + "/data/" + tolower(mission);
    if (access(path.c_str(), F_OK) != 0) {
        throw GException::directory_not_found(G_PATH, path,
              "Requested mission \""+toupper(mission)+"\" not found in"
              " calibration database.");
    }
    if (access(path.c_str(), R_OK) != 0) {
        throw GException::directory_not_accessible(G_PATH, path,
              "Requested read permission not granted for mission \""+
              toupper(mission)+"\".");
    }

    // If an instrument has been specified, verify that instrument name is
    // valid and directory is accessible
    if (instrument.length() > 0) {

        // Add instrument to path
        path += "/" + tolower(instrument);

        // Verify path
        if (access(path.c_str(), F_OK) != 0) {
            throw GException::directory_not_found(G_PATH, path,
                  "Requested instrument \""+toupper(instrument)+"\" on"
                  " mission \""+toupper(mission)+"\" not found in"
                  " calibration database.");
        }
        if (access(path.c_str(), R_OK) != 0) {
            throw GException::directory_not_accessible(G_PATH, path,
                "Requested read permission not granted for instrument \""+
                toupper(instrument)+"\" on mission \""+toupper(mission)+
                "\".");
        }
        
    } // endif: instrument has been specified
    
    // Return path
    return path;
}


/***********************************************************************//**
 * @brief Return absolute CIF filename
 *
 * @param[in] mission Mission name (case insensitive).
 * @param[in] instrument Instrument name (case insensitive; optional).
 *
 * @exception GException::file_not_found
 *            CIF not found.
 *
 * The calibration directory path is given by
 * $CALDB/data/<mission>/caldb.indx
 * or
 * $CALDB/data/<mission>/<instrument>/caldb.indx
 * where <mission> is the name of the mission and <instrument> is the
 * optional instrument name (all lower case). The arguments provided to the
 * method are transformed to lower case.
 ***************************************************************************/
std::string GCaldb::cifname(const std::string& mission, const std::string& instrument)
{
    // Get path to calibration directory
    std::string cif = path(mission, instrument);

    // Add calibration index filename
    cif += "/caldb.indx";

    // Verify that CIF exists
    if (access(cif.c_str(), F_OK) != 0) {
        throw GException::file_not_found(G_CIF, cif,
              "Calibration Index File (CIF) not found.");
    }

    // Return cif
    return cif;
}


/*==========================================================================
 =                                                                         =
 =                                Friends                                  =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Output operator
 *
 * @param[in] os Output stream.
 * @param[in] caldb Calibration database.
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GCaldb& caldb)
{
     // Write calibration database in output stream
    os << caldb.print();

    // Return output stream
    return os;
}


/***********************************************************************//**
 * @brief Log operator
 *
 * @param[in] log Logger.
 * @param[in] caldb Calibration database.
 ***************************************************************************/
GLog& operator<< (GLog& log, const GCaldb& caldb)
{
    // Write calibration database into logger
    log << caldb.print();

    // Return logger
    return log;
}