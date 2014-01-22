/*
-----------------------------------------------------------------------------
This source file is part of OSTIS (Open Semantic Technology for Intelligent Systems)
For the latest info, see http://www.ostis.net

Copyright (c) 2010-2014 OSTIS

OSTIS is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OSTIS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with OSTIS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#ifndef _uiSc2SCnJsonTranslator_h_
#define _uiSc2SCnJsonTranslator_h_

#include "uiTranslatorFromSc.h"

struct sScElementInfo
{
    typedef std::list<sScElementInfo*> tScElementInfoList;

    typedef enum
    {
        VT_NODE,
        VT_EDGE,
        VT_SET,
        VT_CONTOUR

    } eVisualType;

    sc_type type;
    sc_addr addr;
    sc_addr srcAddr;
    sc_addr trgAddr;
    sScElementInfo* source;
    sScElementInfo* target;
    tScElementInfoList outputArcs;
    tScElementInfoList inputArcs;

    eVisualType visualType;
    bool isInSentenceTree;
};

class uiSCnSentenceNode
{
    typedef enum
    {
        ST_KEYWORD,
        ST_PREDICATE,
        ST_NODE
    } eSentenceNodeType;

public:
    typedef std::list<uiSCnSentenceNode*> tSentenceNodeList;

    //! Create keyword sentence node
    explicit uiSCnSentenceNode(sScElementInfo *keywordEl);
    virtual ~uiSCnSentenceNode();

    //! Build tree based on keyword. This node must be a keyword
    void buildTree();
    //! Balance tree
    void balance();
    //! Generates json for specified tree
    const String& json();

    //! Append new child tree node to this one
    uiSCnSentenceNode* createChildNode(eSentenceNodeType type);

protected:
    //! Destroy tree recursively
    void destroyChilds();

    //! Removes node from childs list
    void _removeChildNode(uiSCnSentenceNode *child);
    //! Appends new node to childs list
    void _appendChildNode(uiSCnSentenceNode *child);
    //! Check if specified tree node exists in child list of this one
    bool _hasChildNode(uiSCnSentenceNode *child) const;


private:
    explicit uiSCnSentenceNode();

    //! Create child predicate node
    void _createChildPredicate(sScElementInfo *arc);

private:
    //! Pointer to parent sentence tree node
    uiSCnSentenceNode *mParent;
    //! Type of sentence tree node
    eSentenceNodeType mType;
    //! List of conotur article root nodes
    tSentenceNodeList mContourSentences;
    //! List of child set sentences
    tSentenceNodeList mChildSetSentences;
    //! List of child sentences
    tSentenceNodeList mChildSentences;
    //! Pointer to sc-element info
    sScElementInfo *mElementInfo;
    //! String that contains cache of generated json for tree
    String mJSONData;
};


/*!
 * \brief Class that translates sc-construction into
 * SCn-code (json representation)
 */
class uiSc2SCnJsonTranslator : public uiTranslateFromSc
{
public:
    explicit uiSc2SCnJsonTranslator();
    virtual ~uiSc2SCnJsonTranslator();


    static sc_result ui_translate_sc2scn(sc_event *event, sc_addr arg);

protected:
    //! @copydoc uiTranslateFromSc::runImpl
    void runImpl();

    /*! Translate one sc-element semantic neighborhood
     * @param addr sc-addr of sc-element to translate
     * @param isKeyword Keyword flag
     * @return Returns string that json representaion of sc-element
     */
    String translateElement(sc_addr addr, bool isKeyword);

    //! Check if specified sc-element included in output construction
    bool isInOutputConstruction(sc_addr addr) const;

    //! Collect information of trsnslated sc-elements and store it
    void collectScElementsInfo();

protected:
    //! List of keywords
    tScAddrList mKeywordsList;
    //! Collection of objects information
    typedef std::map<sc_addr, sScElementInfo*> tScElemetsInfoMap;
    tScElemetsInfoMap mScElementsInfo;
    //! Pull of sc-elements information (used to prevent many memory allocations)
    sScElementInfo *mScElementsInfoPool;
    //! List of articles root elements
    uiSCnSentenceNode::tSentenceNodeList mRootSentences;
};



#endif // _uiSc2SCnJsonTranslator_h_
