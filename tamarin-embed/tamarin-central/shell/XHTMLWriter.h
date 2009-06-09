/*
 * XHTMLWriter.h
 *
 *  Created on: Apr 28, 2009
 *      Author: victor
 */

#ifndef XHTMLWRITER_H_
#define XHTMLWRITER_H_

namespace avmshell
{
	class XHTMLWriter : public ClassClosure
	{
	public:
		XHTMLWriter(VTable* cvtable);

		Stringp parse(XMLObject* xml);
		void out(XMLObject* xml);

		XMLObject* newTextNode(Stringp s);

		Stringp escapeXML(Stringp s);
		Stringp escapeXMLAttr(Stringp s);

		void __toXMLString(XMLObject* xml, StringBuffer &s, AtomArray *AncestorNamespace, int indentLevel = 0) const;

		bool prettyPrinting;
	};
}

#endif /* XHTMLWRITER_H_ */
