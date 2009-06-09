/*
 * XHTMLWriter.cpp
 *
 *  Created on: Apr 28, 2009
 *      Author: victor
 */
#import "avmshell.h"

namespace avmshell
{
	XHTMLWriter::XHTMLWriter(VTable *cvtable) : ClassClosure(cvtable)
    {
		createVanillaPrototype();
		prettyPrinting = false;
    }

	XMLObject* XHTMLWriter::newTextNode(Stringp s)
	{
		if (!s) s = core()->kEmptyString;
		XMLObject *xml = new (core()->GetGC()) XMLObject(toplevel()->xmlClass());
		xml->setNode( new (core()->GetGC()) TextE4XNode(0, s) );
		return xml;
	}

	Stringp XHTMLWriter::escapeXML(Stringp s)
	{
		return core()->EscapeElementValue(s, false);
	}

	Stringp XHTMLWriter::escapeXMLAttr(Stringp s)
	{
		return core()->EscapeAttributeValue(s->atom());
	}

	Stringp XHTMLWriter::parse(XMLObject* xml)
	{
		AtomArray *AncestorNamespaces = new (MMgc::GC::GetGC(this)) AtomArray();
		StringBuffer s(core());
		this->__toXMLString(xml, s, AncestorNamespaces, 0);
		return core()->newStringUTF8(s.c_str());
	}

	void XHTMLWriter::out(XMLObject* xml)
	{
		//TODO: we should stream directly to cout
		AtomArray *AncestorNamespaces = new (MMgc::GC::GetGC(this)) AtomArray();
		StringBuffer s(core());
		this->__toXMLString(xml, s, AncestorNamespaces, 0);
		HTTPClass::sWrite(s.c_str());
	}



	void XHTMLWriter::__toXMLString(XMLObject* xml, StringBuffer &s, AtomArray *AncestorNamespaces, int indentLevel) const
	{
		E4XNode* m_node = xml->getNode();

		AvmCore *core = this->core();

		if (prettyPrinting)
		{
			for (int i = 0; i < indentLevel; i++)
			{
				s << " ";
			}
		}
		if (xml->getClass() == E4XNode::kText) // CDATA checked below
		{
			if (prettyPrinting)
			{
				// v = removing leading and trailing whitespace from x.value
				// return escapeElementValue (v);

				s << core->EscapeElementValue(m_node->getValue(), true);
				return;
			}
			else
			{
				s << core->EscapeElementValue(m_node->getValue(), false);
				return;
			}
		}
		if (xml->getClass() == E4XNode::kCDATA)
		{
			s << "<![CDATA[" << m_node->getValue() << "]]>";
			return;
		}
		if (xml->getClass() == E4XNode::kAttribute)
		{
			s << core->EscapeAttributeValue (m_node->getValue()->atom());
			return;
		}
		if (xml->getClass() == E4XNode::kComment)
		{
			s << "<!--";
			s << m_node->getValue();
			s << "-->";
			return;
		}
		if (xml->getClass() == E4XNode::kProcessingInstruction) // step 7
		{
			s << "<?";
			Multiname m;
			AvmAssert (m_node->getQName(core, &m) != 0);
			if (m_node->getQName(core, &m))
			{
				s << m.getName() << " ";
			}
			s << m_node->getValue() << "?>";
			return;
		}

		// We're a little different than the spec here.  Instead of each XMLObject
		// keeping track of its entire in-scope namespace list (all the way to the
		// topmost parent), the XMLObject only knows about its own declared nodes.
		// So when were converting to a string, we need to build the inScopeNamespace
		// list here.

		AtomArray *inScopeNS = new (core->GetGC()) AtomArray();
		m_node->BuildInScopeNamespaceList (core, inScopeNS);
		uint32 origLength = AncestorNamespaces->getLength();

		// step 8 - ancestorNamespaces passed in
		// step 9/10 - add in our namespaces into ancestorNamespaces if there are no conflicts
		for (uint32 i = 0; i < inScopeNS->getLength(); i++)
		{
			Namespace *ns = AvmCore::atomToNamespace (inScopeNS->getAt(i));
			uint32 j;
			for (j = 0; j < AncestorNamespaces->getLength(); j++)
			{
				Namespace *ns2 = AvmCore::atomToNamespace (AncestorNamespaces->getAt(j));
#ifdef STRING_DEBUG
				Stringp u1 = ns->getURI();
				Stringp p1 = core->string(ns->getPrefix());
				Stringp u2 = ns2->getURI();
				Stringp p2 = core->string(ns2->getPrefix());
#endif
				if ((ns->getURI() == ns2->getURI()) && (ns->getPrefix() == ns2->getPrefix()))
					break;
			}

			if (j == AncestorNamespaces->getLength()) // a match was not found
			{
				AncestorNamespaces->push (ns->atom());
			}
		}

		// step 11 - new ISNS changes
		// If this node's namespace has an undefined prefix, generate a new one
		Multiname m;
		AvmAssert (getNode()->getQName (core, &m));
		xml->getNode()->getQName (core, &m);
		Namespace *thisNodesNamespace = xml->GetNamespace (m, AncestorNamespaces);
		AvmAssert(thisNodesNamespace != 0);
		if (thisNodesNamespace->getPrefix() == undefinedAtom)
		{
			// find a prefix and add this namespace to our list
			thisNodesNamespace = xml->GenerateUniquePrefix (thisNodesNamespace, AncestorNamespaces);
			AncestorNamespaces->push (thisNodesNamespace->atom());
		}

		String *nsPrefix = core->string (thisNodesNamespace->getPrefix());

		// If any of this node's attribute's namespaces have an undefined prefix, generate a new one
		for (uint32 i = 0; i < m_node->numAttributes(); i++)
		{
			E4XNode *an = m_node->getAttribute(i);
			AvmAssert(an != 0);
			AvmAssert(an->getClass() == E4XNode::kAttribute);
			Multiname nam;
			AvmAssert(an->getQName(core, &nam));
			an->getQName(core, &nam);
			Namespace *ns = xml->GetNamespace (nam, AncestorNamespaces);
			AvmAssert(ns != 0);
			if (ns->getPrefix() == undefinedAtom)
			{
				// find a prefix and add this namespace to our list
				ns = xml->GenerateUniquePrefix (ns, AncestorNamespaces);

				AncestorNamespaces->push (ns->atom());
			}
		}
		// step 12
		s << "<";
		// step13 - insert namespace prefix if we have one
		if (nsPrefix != core->kEmptyString)
		{
			s << nsPrefix << ":";
		}

		// step 14
		AvmAssert (!m.isAnyName());
		s << m.getName();

		// step 15 - attrAndNamespaces = sum of x.attributes and namespaceDeclarations

		// step 16
		// for each an in attrAndNamespaces
		for (uint32 i = 0; i < m_node->numAttributes(); i++)
		{
			// step 17a
			s << " ";
			E4XNode *an = m_node->getAttribute(i);
			AvmAssert(an != 0);
			AvmAssert(an->getClass() == E4XNode::kAttribute);
			Multiname nam;
			AvmAssert(an->getQName(core, &nam));
			an->getQName(core, &nam);

			// step16b-i - ans = an->getName->getNamespace(AncestorNamespace);
			AvmAssert(nam.isAttr());
			Namespace *attr_ns = xml->GetNamespace (nam, AncestorNamespaces);

			//!!@step16b-ii - should never get hit now with revised 10.2.1 step 11.
			AvmAssert(attr_ns->getPrefix() != undefinedAtom);

			// step16b-iii
			if (attr_ns && attr_ns->hasPrefix ())
			{
				s << core->string(attr_ns->getPrefix()) << ":";
			}
			//step16b-iv
			s << nam.getName();

			//step16c - namespace case - see below

			//step 16d
			s << "=\"";
			//step 16e
			s << core->EscapeAttributeValue(an->getValue()->atom());
			//step 16f - namespace case
			//step 16g
			s << "\"";
		}

		// This adds any NS that were added to our ancestor namespace list (from origLength on up)
		for (uint32 i = origLength; i < AncestorNamespaces->getLength(); i++)
		{
			Namespace *an = AvmCore::atomToNamespace(AncestorNamespaces->getAt(i));
			if (an->getURI() != core->kEmptyString)
			{
				s << " xmlns";
				AvmAssert (an->getPrefix() != undefinedAtom);
				if (an->getPrefix() != core->kEmptyString->atom())
				{
					// 17c iii
					s << ":" << core->string(an->getPrefix());
				}
				// 17d
				s << "=\"";
				//step 17f - namespace case
				s << an->getURI();
				//step 17g
				s << "\"";
			}
		}

//		if (thisNodesNamespace)
//			AncestorNamespaces->push (thisNodesNamespace->atom());

		// step 18
		if (!m_node->numChildren() && nsPrefix == core->kEmptyString)
		{
			//non self-closing tags
			const char* closing[] = {
					"iframe","script","style","title",
					"textarea","pre",
					"div","p",
					"span","b","strong","i","em",
					"h1","h2","h3",
					"table"
					};
			for (int i=0; i<17; i++)
			{
				if (m.getName()->equalsLatin1(closing[i]))
				{
					s << "></" << m.getName() << ">";
					return;
				}
			}

			s << "/>";
			return;
		}

		// step 19
		s << ">";


		// step 20
		E4XNode *firstChild = m_node->_getAt(0);
		AvmAssert(firstChild != 0);
		bool bIndentChildren = ((xml->_length() > 1) || (firstChild->getClass() & ~(E4XNode::kText | E4XNode::kCDATA)));

		// step 21/22
		int nextIndentLevel = 0;
		if (prettyPrinting && bIndentChildren)
		{
			nextIndentLevel = indentLevel + toplevel()->xmlClass()->get_prettyIndent();
		}

		// We need to prune any namespaces with duplicate prefixes in our AncestorNamespace
		// array to prevent shadowing of similar namespaces.  Bug 153363.
		//	var x = <order xmlns:x="x">
		//	<item id="1" xmlns:x="x2">
		//		<menuName xmlns:x="x" x:foo='10'>burger</menuName>
		//		<price>3.95</price>
		//	</item>
		//	</order>;
		//
		// The namespace for menuName should be output even though the identical namespace
		// was output for the top node.  (Since the item node is using an incompatible
		// namespace with the same prefix.)
		AtomArray *newNamespaceArray = new (core->GetGC()) AtomArray();
		uint32 anLen = AncestorNamespaces->getLength();
		for (uint32 i = 0; i < anLen; i++)
		{
			Namespace *first = AvmCore::atomToNamespace(AncestorNamespaces->getAt(i));
			if (i < origLength)
			{
				uint32 j;
				for (j = origLength; j < anLen; j++)
				{
					Namespace *second = AvmCore::atomToNamespace(AncestorNamespaces->getAt(j));
					if (second->getPrefix() == first->getPrefix())
					{
						break;
					}
				}

				// No match, push our namespace on the list.
				if (j == anLen)
				{
					newNamespaceArray->push (first->atom());
				}
			}
			else
			{
				newNamespaceArray->push (first->atom());
			}
		}
		uint32 namespaceLength = newNamespaceArray->getLength();

		// step 23
		for (uint32 i = 0; i < xml->_length(); i++)
		{
			// step 23b
			E4XNode *child = m_node->_getAt(i);
			XMLObject *xo = new (core->GetGC()) XMLObject(toplevel()->xmlClass(), child);
			if (prettyPrinting && bIndentChildren)
			{
				s << "\n";
			}
			__toXMLString (xo, s, newNamespaceArray, nextIndentLevel);

			// Our __toXMLString call might have added new namespace onto our list.  We don't want to
			// save these new namespaces so clear them out here.
			newNamespaceArray->setLength (namespaceLength);
		}

		// Part of the latest spec
		if (prettyPrinting && bIndentChildren)
		{
			s << "\n";
		}

		//step 24
		if (prettyPrinting && bIndentChildren)
		{
			for (int i = 0; i < indentLevel; i++)
			{
				s << " ";
			}
		}

		//step 25
		s << "</";

		//step 26
		if (nsPrefix != core->kEmptyString)
		{
			s << nsPrefix << ":";
		}

		//step 27
		s << m.getName() << ">";

		//step 28
		return;
	}
}
