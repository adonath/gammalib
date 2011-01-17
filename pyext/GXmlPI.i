/***************************************************************************
 *                 GXmlPI.i - XML PI node class definition                 *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GXmlPI.i
 * @brief GXmlPI class python bindings
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GXmlPI.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GXmlPI
 *
 * @brief XML PI node class interface defintion.
 *
 * This class implements a XML Processing Instruction.
 ***************************************************************************/
class GXmlPI : public GXmlNode {
public:
    // Constructors and destructors
    GXmlPI(void);
    GXmlPI(const GXmlPI& node);
    GXmlPI(const std::string& segment);
    ~GXmlPI(void);

    // Implemented virtual methods
    void     clear(void);
    void     write(FILE* fptr, int indent = 0) const;
    //void     print(std::ostream& os, int indent = 0) const;
    NodeType type(void) const { return NT_PI; }
};


/***********************************************************************//**
 * @brief GXmlPI class extension
 ***************************************************************************/
%extend GXmlPI {
//    char *__str__() {
//        return tochar(self->print());
//    }
};


/***********************************************************************//**
 * @brief GXmlPI type casts
 ***************************************************************************/
%inline %{
    GXmlPI* cast_GXmlPI(GXmlNode* node) {
        if (node->type() != GXmlNode::NT_PI)
            throw GException::xml_bad_node_type("cast_GXmlPI(GXmlNode*)",
                                                "",
                                                "Expecting GXmlPI node.");
        return dynamic_cast<GXmlPI*>(node);
    }
%};