/***************************************************************************
 *                 GFitsTable.cpp  - FITS table base class                 *
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
 * @file GFitsTable.cpp
 * @brief FITS table abstract base class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstring>
#include <iostream>
#include <vector>
#include "GException.hpp"
#include "GTools.hpp"
#include "GFitsCfitsio.hpp"
#include "GFits.hpp"
#include "GFitsTable.hpp"
#include "GFitsTableBitCol.hpp"
#include "GFitsTableByteCol.hpp"
#include "GFitsTableBoolCol.hpp"
#include "GFitsTableStringCol.hpp"
#include "GFitsTableUShortCol.hpp"
#include "GFitsTableShortCol.hpp"
#include "GFitsTableULongCol.hpp"
#include "GFitsTableLongCol.hpp"
#include "GFitsTableLongLongCol.hpp"
#include "GFitsTableFloatCol.hpp"
#include "GFitsTableDoubleCol.hpp"
#include "GFitsTableCFloatCol.hpp"
#include "GFitsTableCDoubleCol.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_ACCESS1                              "GFitsTable::operator[](int&)"
#define G_ACCESS2                      "GFitsTable::operator[](std::string&)"
#define G_APPEND_COLUMN           "GFitsTable::append_column(GFitsTableCol*)"
#define G_INSERT_COLUMN      "GFitsTable::insert_column(int, GFitsTableCol*)"
#define G_INSERT_ROWS                   "GFitsTable::insert_rows(int&, int&)"
#define G_REMOVE_ROWS                   "GFitsTable::remove_rows(int&, int&)"
#define G_DATA_OPEN                            "GFitsTable::data_open(void*)"
#define G_DATA_SAVE                                 "GFitsTable::data_save()"
#define G_GET_TFORM                             "GFitsTable::get_tform(int&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_DEBUG_SAVE      //!< Debug data_save() method


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 *
 * Construct an instance of GFitsTable with zero rows. 
 ***************************************************************************/
GFitsTable::GFitsTable(void) : GFitsHDU()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Table constructor
 *
 * @param[in] nrows Number of rows in table.
 *
 * Construct an instance of GFitsTable with a given number of rows.
 ***************************************************************************/
GFitsTable::GFitsTable(int nrows) : GFitsHDU()
{
    // Initialise class members for clean destruction
    init_members();

    // Store numnber of rows
    m_rows = nrows;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] table Table from which to construct instance.
 ***************************************************************************/
GFitsTable::GFitsTable(const GFitsTable& table) : GFitsHDU(table)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(table);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GFitsTable::~GFitsTable(void)
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
 * @param[in] table Table.
 ***************************************************************************/
GFitsTable& GFitsTable::operator=(const GFitsTable& table)
{
    // Execute only if object is not identical
    if (this != &table) {

        // Copy base class members
        this->GFitsHDU::operator=(table);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(table);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Returns reference to table column
 *
 * @param[in] colnum Column number [0,...,ncols()-1].
 *
 * @exception GException::fits_no_data
 *            No data found in table.
 * @exception GException::out_of_range
 *            Column number is out of range.
 ***************************************************************************/
GFitsTableCol& GFitsTable::operator[](const int& colnum)
{
    // If there is no data then throw an exception
    if (m_columns == NULL) {
        throw GException::fits_no_data(G_ACCESS1, "No columns in table.");
    }

    // Compile option: raise exception if column number is out of range
    #if defined(G_RANGE_CHECK)
    if (colnum < 0 || colnum >= m_cols)
        throw GException::out_of_range(G_ACCESS1, colnum, 0, m_cols-1);
    #endif

    // Get column pointer
    GFitsTableCol* ptr = m_columns[colnum];
    if (ptr == NULL) {
        throw GException::fits_no_data(G_ACCESS1, "No data for this column.");
    }

    // Return reference
    return *ptr;
}


/***********************************************************************//**
 * @brief Returns reference to table column (const version)
 *
 * @param[in] colnum Column number [0,...,ncols()-1].
 *
 * @exception GException::fits_no_data
 *            No data found in table.
 * @exception GException::out_of_range
 *            Column number is out of range.
 ***************************************************************************/
const GFitsTableCol& GFitsTable::operator[](const int& colnum) const
{
    // If there is no data then throw an exception
    if (m_columns == NULL) {
        throw GException::fits_no_data(G_ACCESS1, "No columns in table.");
    }

    // Compile option: raise exception if column number is out of range
    #if defined(G_RANGE_CHECK)
    if (colnum < 0 || colnum >= m_cols)
        throw GException::out_of_range(G_ACCESS1, colnum, 0, m_cols-1);
    #endif

    // Get column pointer
    const GFitsTableCol* ptr = m_columns[colnum];
    if (ptr == NULL) {
        throw GException::fits_no_data(G_ACCESS1, "No data for this column.");
    }

    // Return reference
    return *ptr;
}


/***********************************************************************//**
 * @brief Returns reference to table column
 *
 * @param[in] colname Column name.
 *
 * @exception GException::fits_no_data
 *            No data found in table.
 * @exception GException::fits_column_not_found
 *            Column name not found.
 ***************************************************************************/
GFitsTableCol& GFitsTable::operator[](const std::string& colname)
{
    // If there is no data then throw an exception
    if (m_columns == NULL) {
        throw GException::fits_no_data(G_ACCESS2, "No columns in table.");
    }

    // Get column pointer
    GFitsTableCol* ptr = ptr_column(colname);

    // If column has not been found throw an exception
    if (ptr == NULL) {
        throw GException::fits_column_not_found(G_ACCESS2, colname);
    }

    // Return reference
    return *ptr;
}


/***********************************************************************//**
 * @brief Returns reference to table column (const version)
 *
 * @param[in] colname Column name.
 *
 * @exception GException::fits_no_data
 *            No data found in table.
 * @exception GException::fits_column_not_found
 *            Column name not found.
 ***************************************************************************/
const GFitsTableCol& GFitsTable::operator[](const std::string& colname) const
{
    // If there is no data then throw an exception
    if (m_columns == NULL) {
        throw GException::fits_no_data(G_ACCESS2, "No columns in table.");
    }

    // Get column pointer
    GFitsTableCol* ptr = ptr_column(colname);

    // If column has not been found throw an exception
    if (ptr == NULL) {
        throw GException::fits_column_not_found(G_ACCESS2, colname);
    }

    // Return reference
    return *ptr;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Append column to the table
 *
 * @param[in] column Column which should be appended to table.
 *
 * A column will be appended at the end of the table. See
 *   GFitsTable::insert_column
 * for more details on the method.
 ***************************************************************************/
void GFitsTable::append_column(GFitsTableCol& column)
{
    // Inserting at the end correspond to appending
    insert_column(m_cols, column);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Insert column into the table
 *
 * @param[in] colnum Column number to be inserted.
 * @param[in] column Column which should be inserted into the table.
 *
 * @exception GException::fits_bad_col_length
 *            The length of the column is incompatible with the number of
 *            rows in the table.
 *
 * A column will be inserted at position 'colnum' of the table. If the
 * position is beyond the end of the table the column will be appended.
 * Any 'colnum' value smaller than 0 will be set automatically to 0.
 *
 * If the table is empty and has 0 rows, the number of rows will be set to
 * the length of the column.
 *
 * The length of the column to be inserted has to be identical to the number
 * of rows in the table.
 ***************************************************************************/
void GFitsTable::insert_column(int colnum, GFitsTableCol& column)
{
    // Make sure that 'colnum' is valid. Since we add one column the total
    // number of columns at return will be m_cols+1, hence the column
    // index is comprised between [0,m_cols]
    if (colnum <      0) colnum = 0;
    if (colnum > m_cols) colnum = m_cols;

    // If the table is empty and has 0 rows then set the number of rows in
    // the table to the length of the column
    if (m_columns == NULL && m_rows == 0)
        m_rows = column.length();

    // Throw exception if the column length is incompatible with number of
    // rows in the table
    if (m_rows != column.length())
        throw GException::fits_bad_col_length(G_INSERT_COLUMN,
                                              column.length(), m_rows);

    // If no column data exist then allocate them now
    if (m_columns == NULL) {
        m_columns    = new GFitsTableCol*[1];
        m_cols       = 1;
        m_columns[0] = NULL;
        colnum       = 0;     // We insert at the first place
    }

    // ... otherwise make space to insert column
    else {

        // Allocate fresh memory
        GFitsTableCol** tmp = new GFitsTableCol*[m_cols+1];

        // Copy over old column pointers. Leave some space at the position
        // where we want to insert the column
        int src;
        int dst;
        for (src = 0, dst = 0; dst < m_cols+1; ++dst) {
            if (dst == colnum)
                tmp[dst] = NULL;
            else {
                tmp[dst] = m_columns[src];
                src++;
            }
        }

        // Free old column pointer array
        delete [] m_columns;

        // Connect new memory
        m_columns = tmp;

        // Increment column counter
        m_cols++;

    } // endelse: we made space to insert a column

    // Copy column by cloning it
    m_columns[colnum] = column.clone();

    // Reset column number since column does not already exist in FITS
    // file
    m_columns[colnum]->m_colnum = 0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Append rows to the table
 *
 * @param[in] nrows Number of rows to be appended.
 *
 * This method appends rows to a FITS table. This implies that all columns
 * will be loaded into memory.
 ***************************************************************************/
void GFitsTable::append_rows(const int& nrows)
{
    // Set row number for insertion to end of the file
    int rownum = this->nrows();
    
    // Insert rows
    insert_rows(rownum, nrows);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Insert rows into the table
 *
 * @param[in] rownum Row after which rows should be inserted (0=first row).
 * @param[in] nrows Number of rows to be inserted.
 *
 * @exception GException::fits_invalid_row
 *            Specified rownum is invalid.
 *
 * This method inserts rows into a FITS table. This implies that all columns
 * will be loaded into memory.
 ***************************************************************************/
void GFitsTable::insert_rows(const int& rownum, const int& nrows)
{
    // Make sure that rownum is valid
    if (rownum < 0 || rownum > m_rows)
        throw GException::fits_invalid_row(G_INSERT_ROWS, rownum, m_rows);

    // Continue only if there are rows to be inserted
    if (nrows > 0) {

        // Insert rows for all columns
        for (int icol = 0; icol < m_cols; ++icol) {
            m_columns[icol]->insert(rownum, nrows);
        }
        
        // Increment number of rows in table
        m_rows += nrows;

    } // endfor: there were rows to be inserted

    // Return
    return;
}


/***********************************************************************//**
 * @brief Remove rows from the table
 *
 * @param[in] rownum Row after which rows should be removed (0=first row).
 * @param[in] nrows Number of rows to be removed.
 *
 * @exception GException::fits_invalid_row
 *            Specified rownum is invalid.
 * @exception GException::fits_invalid_nrows
 *            Invalid number of rows specified.
 *
 * This method inserts rows into a FITS table. This implies that all columns
 * will be loaded into memory.
 ***************************************************************************/
void GFitsTable::remove_rows(const int& rownum, const int& nrows)
{
    // Make sure that rownum is valid
    if (rownum < 0 || rownum >= m_rows)
        throw GException::fits_invalid_row(G_REMOVE_ROWS, rownum, m_rows-1);
    
    // Make sure that we don't remove beyond the limit
    if (nrows < 0 || nrows > m_rows-rownum)
        throw GException::fits_invalid_nrows(G_REMOVE_ROWS, nrows, m_rows-rownum);

    // Continue only if there are rows to be removed
    if (nrows > 0) {

        // Insert rows for all columns
        for (int icol = 0; icol < m_cols; ++icol) {
            m_columns[icol]->remove(rownum, nrows);
        }
        
        // Decrement number of rows in table
        m_rows -= nrows;

    } // endfor: there were rows to be removed

    // Return
    return;
}


/***********************************************************************//**
 * @brief Return number of rows in table
 ***************************************************************************/
int GFitsTable::nrows(void) const
{
    // Return number of rows
    return m_rows;
}


/***********************************************************************//**
 * @brief Return number of columns in table
 ***************************************************************************/
int GFitsTable::ncols(void) const
{
    // Return number of columns
    return m_cols;
}


/***********************************************************************//**
 * @brief Checks the presence of a column in table
 *
 * @param[in] colname Column name.
 ***************************************************************************/
bool GFitsTable::hascolumn(const std::string& colname) const
{
    // Get pointer in column
    GFitsTableCol* ptr = ptr_column(colname);
    
    // Return state
    return (ptr != NULL);
}


/***********************************************************************//**
 * @brief Print table information
 ***************************************************************************/
std::string GFitsTable::print(void) const
{
    // Initialise result string
    std::string result;

    // Append header
    result.append("=== GFitsTable ===\n");

    // Append HDU information
    result.append(print_hdu());

    // Append table type
    result.append(parformat("Table type"));
    switch (m_type) {
    case GFitsHDU::HT_ASCII_TABLE:
        result.append("ASCII table\n");
        break;
    case GFitsHDU::HT_BIN_TABLE:
        result.append("Binary table\n");
        break;
    default:
        result.append("Unknown\n");
        break;
    }

    // Append table dimensions
    result.append(parformat("Number of rows")+str(m_rows)+"\n");
    result.append(parformat("Number of columns")+str(m_cols)+"\n");

    // Append header information
    result.append(m_header.print());

    // Append table columns
    if (m_columns != NULL) {
        for (int i = 0; i < m_cols; ++i) {
            result.append("\n");
            if (m_columns[i] != NULL)
                result.append(m_columns[i]->print());
            else
                result.append(" Column "+str(i)+" undefined");
        }
    }
    else
        result.append(" Table columns undefined");

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                             Protected methods                           =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Open Table
 *
 * @param[in] vptr FITS file pointer.
 *
 * @exception GException::fits_hdu_not_found
 *            Specified HDU not found in FITS file.
 * @exception GException::fits_error
 *            A CFITSIO error occured during loading the table.
 * @exception GException::fits_unknown_coltype
 *            FITS column of unsupported type has been found in the FITS file.
 * @exception GException::fits_inconsistent_tdim
 *            The TDIM information provided in the header is inconsistent
 *            with the size of the column.
 *
 * This method loads a description of the table into memory. Column data are
 * not loaded at this point to save memory. They will be loaded on request
 * later. 
 ***************************************************************************/
void GFitsTable::data_open(void* vptr)
{
    // Move to HDU
    int status = 0;
    status     = __ffmahd(FPTR(vptr), (FPTR(vptr)->HDUposition)+1, NULL, &status);
    if (status != 0)
        throw GException::fits_hdu_not_found(G_DATA_OPEN, (FPTR(vptr)->HDUposition)+1,
                                             status);

    // Save FITS file pointer
    FPTR_COPY(m_fitsfile, vptr);

    // Determine number of rows in table
    long nrows  = 0;
    status      = __ffgnrw(FPTR(m_fitsfile), &nrows, &status);
    if (status != 0)
        throw GException::fits_error(G_DATA_OPEN, status);
    else
        m_rows = (int)nrows;

    // Determine number of columns in table
    status = __ffgncl(FPTR(m_fitsfile), &m_cols, &status);
    if (status != 0)
        throw GException::fits_error(G_DATA_OPEN, status);

    // Allocate and initialise memory for column pointers. Note that this
    // initialisation is needed to allow for a clean free_members() call
    // in case of any exception.
    if (m_columns != NULL) delete [] m_columns;
    m_columns = new GFitsTableCol*[m_cols];
    for (int i = 0; i < m_cols; ++i)
        m_columns[i] = NULL;

    // Get table column information
    int  typecode = 0;
    long repeat   = 0;
    long width    = 0;
    for (int i = 0; i < m_cols; ++i) {

        // Get column name
        char keyname[10];
        char value[80];
        sprintf(keyname, "TTYPE%d", i+1);
        status = __ffgkey(FPTR(m_fitsfile), keyname, value, NULL, &status);
        if (status != 0)
            throw GException::fits_error(G_DATA_OPEN, status);
        value[strlen(value)-1] = '\0';

        // Get column definition
        status = __ffgtcl(FPTR(m_fitsfile), i+1, &typecode, &repeat, &width,
                          &status);
        if (status != 0)
            throw GException::fits_error(G_DATA_OPEN, status);

        // Check for unsigned columns
        unsigned long offset = 0;
        sprintf(keyname, "TZERO%d", i+1);
        status = __ffgky(FPTR(m_fitsfile), __TULONG, keyname, &offset, NULL, &status);
        if (status == 0) {
            if (typecode == __TSHORT && offset == 32768u)
                typecode = __TUSHORT;
            else if (typecode == __TLONG && offset == 2147483648u)
                typecode = __TULONG;
            else if (typecode == __TINT && offset == 2147483648u)
                typecode = __TUINT;
            else {
                std::ostringstream message;
                message << ", but column " << value << " has typecode " << typecode
                        << " and unexpected associated TZERO=" << offset << ".";
                throw GException::fits_error(G_DATA_OPEN, 0, message.str());
            }
        }
        else
            status = 0;

        // Get column unit (optional, leave blank if not found)
        char unit[80];
        sprintf(keyname, "TUNIT%d", i+1);
        status = __ffgkey(FPTR(m_fitsfile), keyname, unit, NULL, &status);
        if (status != 0) {
            status = 0;
            unit[0] = '\0';
            unit[1] = '\0';
        }
        else
            unit[strlen(unit)-1] = '\0';

        // Get column dimension (optional, leave blank if not found)
        char dim[80];
        sprintf(keyname, "TDIM%d", i+1);
        status = __ffgkey(FPTR(m_fitsfile), keyname, dim, NULL, &status);
        if (status != 0) {
            status = 0;
            dim[0] = '\0';
            dim[1] = '\0';
        }
        else {
            dim[strlen(dim)-1] = '\0';
        }
        
        // If found, extract column dimension into vector array of integers
        std::vector<int> vdim;
        std::string      sdim = strip_chars(strip_whitespace(&(dim[1])),"()");
        if (sdim.length() > 0) {
            std::vector<std::string> elements = split(sdim, ",");
            for (int k = 0; k < elements.size(); ++k) {
                vdim.push_back(toint(elements[k]));
            }
        }

        // Allocate column
        m_columns[i] = alloc_column(typecode);
        if (m_columns[i] == NULL) {
            std::ostringstream colname;
            colname << value;
            throw GException::fits_unknown_coltype(G_DATA_OPEN, colname.str(), typecode);
        }

        // Store column definition
        m_columns[i]->name(strip_whitespace(&(value[1])));
        m_columns[i]->unit(strip_whitespace(&(unit[1])));
        m_columns[i]->dim(vdim);
        m_columns[i]->m_colnum = i+1;
        m_columns[i]->m_type   = typecode;
        m_columns[i]->m_repeat = repeat;
        m_columns[i]->m_width  = width;
        m_columns[i]->m_length = m_rows;
        m_columns[i]->connect(FPTR(m_fitsfile));

        // Extract column vector size
        if (m_columns[i]->m_repeat == 1) { // ASCII tables
            m_columns[i]->m_number = 1;
        }
        else {                             // Binary tables
            if (typecode == __TSTRING) {
                m_columns[i]->m_number = m_columns[i]->m_repeat /
                                         m_columns[i]->m_width;
            }
            else {
                m_columns[i]->m_number = m_columns[i]->m_repeat;
            }
        }
        
        // If TDIM information was set then check its consistency
        if (!vdim.empty()) {
            
            // Compute expectation
            int num = vdim[0];
            for (int k = 1; k < vdim.size(); ++k) {
                num *= vdim[k];
            }
                
            // Compare with real size
            if (num != m_columns[i]->m_number) {
                throw GException::fits_inconsistent_tdim(G_DATA_OPEN,
                                                         vdim,
                                                         m_columns[i]->m_number);
            }
                
        } // endif: Valid TDIM information was found

    } // endfor: looped over all columns

    // Return
    return;
}


/***********************************************************************//**
 * @brief Save table into FITS file
 *
 * @exception GException::fits_error
 *            A CFITSIO error occured in this method.
 * @exception GException::fits_bad_col_length
 *            Table columns have inconsistent lengths.
 *
 * This method saves a table into a FITS file.
 *
 * In case that no table HDU exists it will be created by appending a new
 * HDU to the existing file. The definition of this HDU will be based on the
 * table definition in the class instance.
 *
 * The method also verifies the consistency of all table columns. Table
 * columns need to have identical lengths to be saved into a FITS table.
 * All columns with a length of zero will be excluded from saving, and if
 * they exist in the FITS file, they will be removed from the file.
 *
 * @todo This method should also update the header. Even easier, this method
 *       should save the header into the file using the m_header.save()
 *       method.
 *       Only this assures coherence between the files !!!! Once this has
 *       been implemented (also in the GFitsImage method) we should delete
 *       the m_header.save() call in GFitsHDU::save.
 ***************************************************************************/
void GFitsTable::data_save(void)
{
    // Debug definition: Dump method entry
    #if defined(G_DEBUG_SAVE)
    std::cout << "GFitsTable::save: entry" << std::endl;
    #endif

    // Make sure that column lengths are consistent with table length.
    // Columns with zero length will not be considered (why?)
    for (int i = 0; i < m_cols; ++i) {
        if (m_columns[i] != NULL && m_columns[i]->length() > 0) {
            if (m_columns[i]->length() != m_rows) {
                throw GException::fits_bad_col_length(G_DATA_SAVE,
                                                      m_columns[i]->length(),
                                                      m_rows);
            }
        }
    }

    // Move to HDU
    int status = 0;
    status = __ffmahd(FPTR(m_fitsfile), m_hdunum+1, NULL, &status);

    // If HDU does not exist in file then create it now
    if (status == 107) {

        // Reset status
        status = 0;

        // Initialise number of fields
        int tfields = 0;

        // Setup cfitsio column definition arrays
        char** ttype = NULL;
        char** tform = NULL;
        char** tunit = NULL;
        if (m_cols > 0) {
            ttype = new char*[m_cols];
            tform = new char*[m_cols];
            tunit = new char*[m_cols];
            for (int i = 0; i < m_cols; ++i) {
                ttype[i] = NULL;
                tform[i] = NULL;
                tunit[i] = NULL;
            }
            for (int i = 0; i < m_cols; ++i) {
                ttype[tfields] = get_ttype(i);
                tform[tfields] = get_tform(i);
                tunit[tfields] = get_tunit(i);
                if (ttype[tfields] != NULL && tform[tfields] != NULL && 
                    tunit[tfields] != NULL)
                    tfields++;
            }
        }

        // Create FITS table
        status = __ffcrtb(FPTR(m_fitsfile), m_type, m_rows, tfields, ttype, tform,
                          tunit, NULL, &status);
        if (status != 0)
            throw GException::fits_error(G_DATA_SAVE, status);

        // De-allocate column definition arrays
        if (m_cols > 0) {
            for (int i = 0; i < m_cols; ++i) {
                if (ttype[i] != NULL) delete [] ttype[i];
                if (tform[i] != NULL) delete [] tform[i];
                if (tunit[i] != NULL) delete [] tunit[i];
            }
            if (ttype != NULL) delete [] ttype;
            if (ttype != NULL) delete [] tform;
            if (ttype != NULL) delete [] tunit;
        }

        // Connect all existing columns to FITS table
        if (m_columns != NULL) {
            for (int i = 0; i < m_cols; ++i) {
                if (m_columns[i] != NULL) {
                    FPTR_COPY(m_columns[i]->m_fitsfile, m_fitsfile);
                    m_columns[i]->m_colnum = i+1;
                }
            }
        }

        // Debug option: Signal table creation
        #if defined(G_DEBUG_SAVE)
        std::cout << "GFitsTable::save: created new table" << std::endl;
        #endif

    }
    
    // ... otherwise we signal a FITS error
    else if (status != 0) {
        throw GException::fits_error(G_DATA_SAVE, status);
    }

    // Determine number of columns in table
    int num_cols = 0;
    status = __ffgncl(FPTR(m_fitsfile), &num_cols, &status);
    if (status != 0) {
        throw GException::fits_error(G_DATA_SAVE, status);
    }
    
    // Debug option: Log number of columns in FITS table
    #if defined(G_DEBUG_SAVE)
    std::cout << "GFitsTable::save: FITS table contains ";
    std::cout << num_cols << " columns." << std::endl;
    #endif

    // If we have no columns in the table then delete all columns from
    // FITS table
    if (m_columns == NULL && num_cols > 0) {
        for (int i = 0; i < num_cols; ++i) {
            status = __ffdcol(FPTR(m_fitsfile), i, &status);
            if (status != 0) {
                throw GException::fits_error(G_DATA_SAVE, status);
            }
        }
    }

    // ... otherwise update the FITS table
    else {

        // Determine number of rows in table
        long num_rows = 0;
        status        = __ffgnrw(FPTR(m_fitsfile), &num_rows, &status);
        if (status != 0) {
            throw GException::fits_error(G_DATA_SAVE, status);
        }

        // Debug option: Log number of rows in FITS table
        #if defined(G_DEBUG_SAVE)
        std::cout << "GFitsTable::save: FITS table contains ";
        std::cout << num_rows << " rows." << std::endl;
        #endif

        // If the table length differs from number of rows in the FITS file
        // then readjust FITS table length. We do this by adding or
        // deleting rows at the end of the table as we anyways rewrite the
        // entire table later
        if (m_rows > num_rows) {
        
            // Insert rows at the end of the table
            long long firstrow = num_rows;
            long long nrows    = m_rows - num_rows;
            status = __ffirow(FPTR(m_fitsfile), firstrow, nrows, &status);
            if (status != 0) {
                throw GException::fits_error(G_DATA_SAVE, status);
            }
            
            // Debug option: Log row adding
            #if defined(G_DEBUG_SAVE)
            std::cout << "GFitsTable::save: Added " << nrows;
            std::cout << " rows to FITS table." << std::endl;
            #endif
            
        }
        else if (m_rows < num_rows) {

            // Delete rows at the end of the table
            long long firstrow = num_rows;
            long long nrows    = num_rows - m_rows;
            status = __ffdrow(FPTR(m_fitsfile), firstrow, nrows, &status);
            if (status != 0) {
                throw GException::fits_error(G_DATA_SAVE, status);
            }
            
            // Debug option: Log row adding
            #if defined(G_DEBUG_SAVE)
            std::cout << "GFitsTable::save: Deleted " << nrows;
            std::cout << " rows from FITS table." << std::endl;
            #endif
            
        }
        
        // Update all columns. The 'm_colnum' field specifies where in the
        // FITS file the column resides. If 'm_colnum=0' then we have a new
        // column that does not yet exist. In this case we append a new column
        // to the FITS file.
        for (int i = 0; i < m_cols; ++i) {

            // Only consider valid columns
            if (m_columns[i] != NULL) {

                // If column has no correspondance than add new column in
                // FITS table and link column to table.
                if (m_columns[i]->m_colnum == 0) {

                    // Increment number of columns in FITS file
                    num_cols++;

                    // Append column to FITS file
                    status = __fficol(FPTR(m_fitsfile), num_cols, get_ttype(i),
                                      get_tform(i), &status);
                    if (status != 0) {
                        throw GException::fits_error(G_DATA_SAVE, status);
                    }

                    // Connect all column to FITS table by copying over the
                    // FITS file pointer.
                    FPTR_COPY(m_columns[i]->m_fitsfile, m_fitsfile);
                    m_columns[i]->m_colnum = num_cols;

                } // endif: column appended to FITS file

                // Now write column into FITS file (only if length is positive)
                if (m_columns[i]->length() > 0) {
                    m_columns[i]->save();
                }

            } // endif: column was valid
        } // endfor: looped over all table columns

        // Delete all unused columns from FITS file. We do this from last to
        // first so that the column numbers remain valid. Also note that
        // FITS column counting starts from 1.
        for (int colnum = num_cols; colnum > 0; --colnum) {

            // Get column name from FITS file
            char keyname[10];
            char value[80];
            sprintf(keyname, "TTYPE%d", colnum);
            status = __ffgkey(FPTR(m_fitsfile), keyname, value, NULL, &status);
            if (status != 0) {
                throw GException::fits_error(G_DATA_SAVE, status);
            }
            value[strlen(value)-1] = '\0';
            std::string colname = strip_whitespace(&(value[1]));
            
            // Check if this column is actually in our list of columns
            bool used = false;
            for (int i = 0; i < m_cols; ++i) {
                if (m_columns[i]           != NULL &&
                    m_columns[i]->length() > 0 &&
                    m_columns[i]->name()   == colname) {
                    used = true;
                    break;
                }
            }

            // If column is not used then delete it now from FITS table
            if (!used) {

                // Delete column
                status = __ffdcol(FPTR(m_fitsfile), colnum, &status);
                if (status != 0) {
                    throw GException::fits_error(G_DATA_SAVE, status);
                }

                // Debug option: Log column deletion
                #if defined(G_DEBUG_SAVE)
                std::cout << "GFitsTable::save: Deleted obsolete column ";
                std::cout << value << " from FITS table." << std::endl;
                #endif
                
            } // endif: deleted column

        } // endfor: Looped over all FITS columns

    } // endelse: FITS table has been updated

    // Now update the header for all columns (unit and TDIM information)
    for (int i = 0; i < m_cols; ++i) {

        // Only consider valid columns
        if (m_columns[i] != NULL) {

            // Get column number
            int colnum = m_columns[i]->m_colnum;

            // Update column units if available
            if (m_columns[i]->unit().length() > 0) {

                // Build keyname
                std::string keyname = "TUNIT"+str(colnum);

                // Update header card
                card(keyname, m_columns[i]->unit(), "physical unit of field");
            }

            // Write TDIM keyword if available
            if (m_columns[i]->dim().size() > 0) {
        
                // Build keyname
                std::string keyname = "TDIM"+str(colnum);
            
                // Build value string
                std::string value = "("+str(m_columns[i]->dim()[0]);
                for (int k = 1; k < m_columns[i]->dim().size(); ++k) {
                    value += ","+str(m_columns[i]->dim()[k]);
                }
                value += ")";
                
                // Update header card
                card(keyname, value, "dimensions of field");
            
            } // endif: wrote TDIM keyword

        } // endif: column was valid

    } // endfor: looped over all columns

    // Debug definition: Dump method exit
    #if defined(G_DEBUG_SAVE)
    std::cout << "GFitsTable::save: exit" << std::endl;
    #endif

    // Return
    return;
}


/***********************************************************************//**
 * @brief Close table
 ***************************************************************************/
void GFitsTable::data_close(void)
{
    // Free members
    free_members();

    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Connect table data to FITS file
 *
 * @param[in] vptr FITS file pointer.
 *
 * Connects the table columns to the file specified by the FITS file pointer.
 * This method does nothing if the file pointer in not valid.
 ***************************************************************************/
void GFitsTable::data_connect(void* vptr)
{
    // Continue only if file pointer is valid
    if (vptr != NULL) {

        // Connect all columns
        if (m_columns != NULL) {
            for (int i = 0; i < m_cols; ++i) {
                if (m_columns[i] != NULL) m_columns[i]->connect(vptr);
            }
        }

    } // endif: file pointer was valid

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns pointer to column type
 *
 * @param[in] colnum Column number for which type is to be returned
 *
 * This methods allocates memory for the character string that holds the
 * column type. The client has to de-allocate this memory after usage.
 * In case that the column does not exist a NULL pointer is returned.
 ***************************************************************************/
char* GFitsTable::get_ttype(const int& colnum) const
{
    // Initialise result with NULL pointer
    char* ptr = NULL;

    // Get type only if column exists
    if (m_columns != NULL && colnum >=0 && colnum < m_cols && 
        m_columns[colnum] != NULL) {
        int size = m_columns[colnum]->m_name.length();
        ptr      = new char[size+1];
        std::strncpy(ptr, m_columns[colnum]->m_name.c_str(), size);
        ptr[size] = '\0';
   }

    // Return result
    return ptr;
}


/***********************************************************************//**
 * @brief Returns pointer to column format
 *
 * @param[in] colnum Column number (starting from 0).
 *
 * @exception GException::fits_unknown_tabtype
 *            Table is neither ASCII nor Binary.
 *
 * This methods allocates memory for the character string that holds the
 * column format. The client has to de-allocate this memory after usage.
 * In case that the column does not exist a NULL pointer is returned.
 ***************************************************************************/
char* GFitsTable::get_tform(const int& colnum) const
{
    // Initialise result with NULL pointer
    char* ptr = NULL;

    // Get type only if column exists
    if (m_columns != NULL && colnum >=0 && colnum < m_cols &&
        m_columns[colnum] != NULL) {

        // Get table type specific format
        int size;
        switch (m_type) {
        case GFitsHDU::HT_ASCII_TABLE:
            size = m_columns[colnum]->ascii_format().length();
            ptr  = new char[size+1];
            std::strncpy(ptr, m_columns[colnum]->ascii_format().c_str(), size);
            break;
        case GFitsHDU::HT_BIN_TABLE:
            size = m_columns[colnum]->binary_format().length();
            ptr  = new char[size+1];
            std::strncpy(ptr, m_columns[colnum]->binary_format().c_str(), size);
            break;
        default:
            throw GException::fits_unknown_tabtype(G_GET_TFORM, m_type);
            break;
        }
        ptr[size] = '\0';
    }

    // Return result
    return ptr;
}


/***********************************************************************//**
 * @brief Returns pointer to column unit
 *
 * @param[in] colnum Column number (starting from 0).
 *
 * This methods allocates memory for the character string that holds the
 * column unit. The client has to de-allocate this memory after usage.
 * In case that the column does not exist a NULL pointer is returned.
 ***************************************************************************/
char* GFitsTable::get_tunit(const int& colnum) const
{
    // Initialise result with NULL pointer
    char* ptr = NULL;

    // Get type only if column exists
    if (m_columns != NULL && colnum >=0 && colnum < m_cols && 
        m_columns[colnum] != NULL) {
        int size = m_columns[colnum]->m_unit.length();
        ptr      = new char[size+1];
        std::strncpy(ptr, m_columns[colnum]->m_unit.c_str(), size);
        ptr[size] = '\0';
    }

    // Return result
    return ptr;
}


/*==========================================================================
 =                                                                         =
 =                              Private methods                            =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GFitsTable::init_members(void)
{
    // Initialise members
    m_type    = -1;
    m_rows    = 0;
    m_cols    = 0;
    m_columns = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] table Table to copy
 ***************************************************************************/
void GFitsTable::copy_members(const GFitsTable& table)
{
    // Copy attributes
    m_type = table.m_type;
    m_rows = table.m_rows;
    m_cols = table.m_cols;

    // Copy column definition
    if (table.m_columns != NULL && m_cols > 0) {
        m_columns = new GFitsTableCol*[m_cols];
        for (int i = 0; i < m_cols; ++i) {
            if (table.m_columns[i] != NULL)
                m_columns[i] = table.m_columns[i]->clone();
            else
                m_columns[i] = NULL;
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 *
 * De-allocates all column pointers
 ***************************************************************************/
void GFitsTable::free_members(void)
{
    // Free memory
    if (m_columns != NULL) {
        for (int i = 0; i < m_cols; ++i) {
            if (m_columns[i] != NULL) delete m_columns[i];
        }
        delete [] m_columns;
    }

    // Mark memory as freed
    m_columns = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Allocates column
 *
 * @param[in] typecode cfitsio type code
 *
 * Allocates a table column depending on the cfitsio type code. If type code
 * is not found then return a NULL pointer.
 ***************************************************************************/
GFitsTableCol* GFitsTable::alloc_column(int typecode) const
{
    // Initialise table column pointer
    GFitsTableCol* ptr = NULL;

    // Allocate column
    switch (typecode) {
    case __TBIT:
        ptr = new GFitsTableBitCol;
        break;
    case __TBYTE:
        ptr = new GFitsTableByteCol;
        break;
    case __TLOGICAL:
        ptr = new GFitsTableBoolCol;
        break;
    case __TSTRING:
        ptr = new GFitsTableStringCol;
        break;
    case __TUSHORT:
        ptr = new GFitsTableUShortCol;
        break;
    case __TSHORT:
        ptr = new GFitsTableShortCol;
        break;
    case __TULONG:
        ptr = new GFitsTableULongCol;
        break;
    case __TLONG:
        ptr = new GFitsTableLongCol;
        break;
    case __TFLOAT:
        ptr = new GFitsTableFloatCol;
        break;
    case __TLONGLONG:
        ptr = new GFitsTableLongLongCol;
        break;
    case __TDOUBLE:
        ptr = new GFitsTableDoubleCol;
        break;
    case __TCOMPLEX:
        ptr = new GFitsTableCFloatCol;
        break;
    case __TDBLCOMPLEX:
        ptr = new GFitsTableCDoubleCol;
        break;
    default:
        break;
    }

    // Return
    return ptr;
}


/***********************************************************************//**
 * @brief Returns pointer of column with given name
 *
 * @param[in] colname Name of column.
 *
 * This method returns a pointer on the column with the specified name. It
 * returns NULL if no column with the given name was found, or if the
 * column is not allocated.
 ***************************************************************************/
GFitsTableCol* GFitsTable::ptr_column(const std::string& colname) const
{
    // Initialise pointer
    GFitsTableCol* ptr = NULL;

    // If there are columns then search for the specified name
    if (m_columns != NULL) {
        for (int i = 0; i < m_cols; ++i) {
            if (m_columns[i]->name() == colname) {
                ptr = m_columns[i];
                break;
            }
        }
    }
    
    // Return pointer
    return ptr;
}
