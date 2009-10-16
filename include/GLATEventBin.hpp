/***************************************************************************
 *                 GLATEventBin.hpp  -  LAT event bin class                *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009 by Jurgen Knodlseder                                *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GLATEventBin.hpp
 * @brief GLATEventBin class interface definition.
 * @author J. Knodlseder
 */

#ifndef GLATEVENTBIN_HPP
#define GLATEVENTBIN_HPP

/* __ Includes ___________________________________________________________ */
#include "GEventBin.hpp"
#include "GModels.hpp"
#include "GVector.hpp"


/***********************************************************************//**
 * @class GLATEventBin
 *
 * @brief GLATEventBin class interface defintion.
 ***************************************************************************/
class GLATEventBin : public GEventBin {

    // Friend classes
    friend class GLATEventCube;

    // I/O friends
    friend std::ostream& operator<< (std::ostream& os, const GLATEventBin& bin);

public:
    // Constructors and destructors
    GLATEventBin();
    GLATEventBin(const GLATEventBin& bin);
    virtual ~GLATEventBin();

    // Operators
    GLATEventBin& operator= (const GLATEventBin& bin);

    // Methods
    double   counts(void) const { return *m_counts; }
    double   model(GModels& models, GVector* gradient);
    GSkyDir* dir(void) { return m_dir; }
    double*  energy(void) { return m_energy; }
    double*  time(void) { return &m_time; }
    
protected:
    // Protected methods
    void          init_members(void);
    void          copy_members(const GLATEventBin& bin);
    void          free_members(void);
    GLATEventBin* clone(void) const;

    // Protected data area (defines all LAT specific event attributes)
    double*  m_counts;      //!< Counts
    double   m_time;        //!< Bin time (constant for LAT cube)
    GSkyDir* m_dir;         //!< Pointer to bin direction
    double*  m_energy;      //!< Pointer to bin energy

};

#endif /* GLATEVENTBIN_HPP */
