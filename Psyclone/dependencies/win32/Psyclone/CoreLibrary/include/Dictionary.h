/***************************** License **********************************

 Copyright (C) 2008 by Communicative Machines
 http://www.cmlabs.com   All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

************************************************************************/


// Dictionary.h: interface for the ObjectDictionary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICTIONARY_H__F779901A_22FC_4BBF_B741_54E6987384DE__INCLUDED_)
#define AFX_DICTIONARY_H__F779901A_22FC_4BBF_B741_54E6987384DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Collection.h"

namespace cmlabs {

class Dictionary;
class Dictionary : public CollectionBase  
{
public:
	Dictionary();
	Dictionary(const JString& xml);
	Dictionary(XMLNode *node);
	virtual ~Dictionary();
	Dictionary(const Dictionary &c);
	const Dictionary& operator = (const Dictionary& c);
	bool addAll(const CollectionBase* c);
	bool addAll(const CollectionBase& c);

	bool contains(const JString& key) const;
	bool containsIgnoreCase(const JString& key) const;

	JString get(int pos);
	JString get(int pos) const;
	JString get(const JString& key) const;
	JString take(int pos);
	JString take(const JString& key);
	JString getIgnoreCase(const JString& key) const;
	JString operator [](int pos) const;
	JString operator [](const JString& key) const;
	Collection getAllKeys();

	bool put(const JString& key, const JString& value);
	bool remove(const JString& key);
	bool remove(int pos);
	bool putFirst(const JString& key, const JString& value);
	bool putLast(const JString& key, const JString& value);
	bool putAfter(int pos, const JString& key, const JString& value);

	JString getFirst();
	JString getLast();
	JString getNext();
	JString getPrevious();

	JString getKey(int pos);
	JString getKey(int pos) const;
	JString getFirstKey();
	JString getLastKey();
	JString getNextKey();
	JString getPreviousKey();

	bool removeNoDelete(int pos);
	bool removeAllNoDelete();
	void noDelete();
	void doDelete();

	bool unitTest();
};

} // namespace cmlabs

#endif // !defined(AFX_DICTIONARY_H__F779901A_22FC_4BBF_B741_54E6987384DE__INCLUDED_)
