/***************************************************************************
 *                       GLog.hpp - Information logger                     *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2012 by Juergen Knoedlseder                         *
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
 * @file GLog.hpp
 * @brief Information logger class definition
 * @author Juergen Knoedlseder
 */

#ifndef GLOG_HPP
#define GLOG_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include <iostream>

/* __ Forward declarations _______________________________________________ */


/***********************************************************************//**
 * @class GLog
 *
 * @brief Information logger interface defintion.
 *
 * This class implements an interface for logging of text messages to the
 * standard output and/or error streams and into an ASCII log file. It
 * provides a C++ style interface (allowing to stream messages using code
 * such as
 * log << "This is a message";
 * and a C style interface that allows making use of C style text formating
 * using code such as
 * log("These are %d messages with %d items.", n, m);
 * The logger interface implements an internal character buffer that is
 * flushed once a maximum size parameter is exceeded. The buffer limit can
 * be modified using the max_size() method; by default it is set to 8192
 * characters. The logger allows prepending of the current date and the task
 * name.
 ***************************************************************************/
class GLog {

public:
    // Constructors and destructors
    GLog(void);
    GLog(const std::string& filename, bool clobber = false);
    GLog(const GLog& log);
    ~GLog(void);

    // Operators
    GLog& operator= (const GLog& log);
    void  operator()(const char *msgFormat, ...);
    GLog& operator<<(GLog& log);
    GLog& operator<<(const std::string& str);
    GLog& operator<<(const char* str);
    GLog& operator<<(const char& value);
    GLog& operator<<(const unsigned char& value);
    GLog& operator<<(const bool& value);
    GLog& operator<<(const int& value);
    GLog& operator<<(const unsigned int& value);
    GLog& operator<<(const double& value);
    GLog& operator<<(std::ostream& (*fn)(std::ostream&));

    // Methods
    void        clear(void);
    int         size(void) const;
    void        open(const std::string& filename, bool clobber = false);
    void        close(void);
    void        flush(bool force = false);
    void        date(bool flag);
    void        cout(bool flag);
    void        cerr(bool flag);
    void        name(const std::string& name);
    void        max_size(int size);
    void        indent(int indent);
    void        header0(const std::string& arg) { header(arg, 0); }
    void        header1(const std::string& arg) { header(arg, 1); }
    void        header2(const std::string& arg) { header(arg, 2); }
    void        header3(const std::string& arg) { header(arg, 3); }
    bool        date(void) const { return m_use_date; }
    bool        cout(void) const { return m_stdout; }
    bool        cerr(void) const { return m_stderr; }
    std::string name(void) const { return m_name; }
    int         max_size(void) const { return m_max_length; }
    int         indent(void) const { return m_indent; }
    std::string filename(void) const { return m_filename; }

protected:
    // Protected methods
    void        init_members(void);
    void        copy_members(const GLog& log);
    void        free_members(void);
    void        header(const std::string& arg, int level);
    std::string strdate(void) const;
    std::string prefix(void) const;
    void        append(std::string arg);

    // Protected data members
    int         m_max_length;    //!< Maximum buffer length
    int         m_indent;        //!< Indentation of text
    bool        m_stdout;        //!< Dump in standard output
    bool        m_stderr;        //!< Dump in standard error
    bool        m_use_date;      //!< Dump date in prefix
    FILE*       m_file;          //!< Log file pointer
    std::string m_filename;      //!< Log file name
    std::string m_name;          //!< Name for prefix
    std::string m_buffer;        //!< Output string buffer
};

#endif /* GLOG_HPP */
