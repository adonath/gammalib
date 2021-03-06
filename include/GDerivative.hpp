/***************************************************************************
 *                  GDerivative.hpp  -  Derivative class                   *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011-2012 by Juergen Knoedlseder                         *
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
 * @file GDerivative.hpp
 * @brief GDerivative class interface definition.
 * @author Juergen Knoedlseder
 */

#ifndef GDERIVATIVE_HPP
#define GDERIVATIVE_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GBase.hpp"
#include "GFunction.hpp"


/***********************************************************************//**
 * @class GDerivative
 *
 * @brief Numerical derivatives class interface defintion
 *
 * This class allows to compute numerical derivatives using various methods.
 * The function to be derived is implemented by the abstract GFunction
 * class.
 ***************************************************************************/
class GDerivative : public GBase {

public:

    // Constructors and destructors
    GDerivative(void);
    explicit GDerivative(GFunction* func);
    GDerivative(const GDerivative& dx);
    virtual ~GDerivative(void);

    // Operators
    GDerivative& operator= (const GDerivative& dx);

    // Methods
    void             clear(void);
    GDerivative*     clone(void) const;
    void             max_iter(const int& max_iter) { m_max_iter=max_iter; }
    void             eps(const double& eps) { m_eps=eps; }
    void             step_frac(const double& f) { m_step_frac=f; }
    void             silent(const bool& silent) { m_silent=silent; }
    const int&       iter(void) const { return m_iter; }
    const int&       max_iter(void) const { return m_max_iter; }
    const double&    eps(void) const { return m_eps; }
    const double&    step_frac(void) const { return m_step_frac; }
    const bool&      silent(void) const { return m_silent; }
    void             function(GFunction* func) { m_func = func; }
    const GFunction* function(void) const { return m_func; }
    double           value(const double& x, double step = 0.0);
    double           ridder(const double& x, const double& h, double& err);
    double           minuit2(const double& x, double& err);
    double           difference(const double& x, const double& h);
    std::string      print(void) const;

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GDerivative& dx);
    void free_members(void);
    void set_tiny(void);

    // Tiny number computation
    class tiny {
    public:
        tiny(void) : m_one(1.0) {}
        ~tiny(void) {}
        double one(void) const;
        double operator()(double eps) const;
    private:
        double m_one;
    };

    // Protected members
    GFunction* m_func;         //!< Pointer to function
    double     m_eps;          //!< Derivative precision
    double     m_step_frac;    //!< Value fraction to use for initial step
    double     m_tiny;         //!< Tiny number for minuit2
    int        m_max_iter;     //!< Maximum number of iterations
    int        m_iter;         //!< Number of iterations used
    bool       m_silent;       //!< Suppress warnings
};

#endif /* GDERIVATIVE_HPP */
