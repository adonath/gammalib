/***************************************************************************
 *           GCTAInstDir.i  -  CTA instrument direction class              *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010 by Jurgen Knodlseder                                *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GCTAInstDir.i
 * @brief GCTAInstDir class python bindings
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAInstDir.hpp"
%}


/***********************************************************************//**
 * @class GCTAInstDir
 *
 * @brief Python bindings for the CTA instrument direction class
 ***************************************************************************/
class GCTAInstDir : public GInstDir {
public:
    // Constructors and destructors
    GCTAInstDir(void);
    GCTAInstDir(const GCTAInstDir& dir);
    virtual ~GCTAInstDir(void);

    // Methods
    void         clear(void);
    GCTAInstDir* clone(void) const;
    void         skydir(const GSkyDir& dir) { m_dir=dir; }
    void         radec(const double& ra, const double& dec) { m_dir.radec(ra,dec); }
    void         radec_deg(const double& ra, const double& dec) { m_dir.radec_deg(ra,dec); }
    void         lb(const double& l, const double& b) { m_dir.lb(l,b); }
    void         lb_deg(const double& l, const double& b) { m_dir.lb_deg(l,b); }
    GSkyDir      skydir(void) const { return m_dir; }
    double       l(void) const { return m_dir.l(); }
    double       l_deg(void) const { return m_dir.l_deg(); }
    double       b(void) const { return m_dir.b(); }
    double       b_deg(void) const { return m_dir.b_deg(); }
    double       ra(void) const { return m_dir.ra(); }
    double       ra_deg(void) const { return m_dir.ra_deg(); }
    double       dec(void) const { return m_dir.dec(); }
    double       dec_deg(void) const { return m_dir.dec_deg(); }
    double       dist(GSkyDir& dir) const { return m_dir.dist(dir); }
    double       dist_deg(GSkyDir& dir) const { return m_dir.dist_deg(dir); }
    double       dist(GCTAInstDir& dir) const;
    double       dist_deg(GCTAInstDir& dir) const;
};


/***********************************************************************//**
 * @brief GCTAInstDir class extension
 ***************************************************************************/
%extend GCTAInstDir {
    GCTAInstDir copy() {
        return (*self);
    }
};