
////////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is redistributable on the same terms as the PCRE 4.3 license, except copyright
// notice must be attributed to:
//
// (C) 2003 Zachary Hansen xaxxon@slackworks.com
//
// Distribution under the GPL or LGPL overrides any other restrictions, as in the PCRE license
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PME_H
#define PME_H

#include <string>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <stdio.h>

//using namespace std;

#include <pcre.h>



/// PME wraps the PCRE C API into a more perl-like syntax
/**
 * PME wraps the PCRE C API into a more perl-liek syntax.  It supports single matching,
 *   global matching (where the regex resumes where it left off on the previous iteration),
 *   single and global substitutions (including using backrefs in the substituted strings),
 *   splits based on regex comparisons, and a syntactically easy way to get substrings out f
 *   from backrefs and splits.
 *
 *    \subpage page_readme @PME documentation (readme)
 */
typedef std::vector < std::string > StringVector;

/** PME base class to perform match, split, search and replace on strings using PCRE regexpes.
 *
 * PME wraps the PCRE C API into a more perl-like syntax.
 *
 * For more information, see the \subpage page_readme  section.
 * */   

class PME
{
public:

	/// default constructor -- virtually worthless
	PME( );

	/// s is the regular expression, opts are PCRE flags bit-wise or'd together
	PME(const std::string & s, unsigned opts );

	/// s is the regular expression, opts is a perl-like string of modifier letters "gi" is global and case insensitive
	PME(const std::string & s, const std::string & opts = "");

	/// s is the regular expression, opts are PCRE flags bit-wise or'd together
	PME(const char * s, unsigned opts );

	/// s is the regular expression, opts is a perl-like string of modifier letters "gi" is global and case insensitive  
	PME ( const char * s, const std::string & opts = "" );

	/// PME copy constructor
	PME(const PME & r);

	/// PME = PME assignment operator
	const PME & operator = (const PME & r);

	/// destructor
	~PME();

	/// stores results from matches
	typedef std::pair<int, int> markers;
	
	/// returns options set on this object
	unsigned				options();

	/// sets new options on the object -- doesn't work?
	void					options(unsigned opts);

	/// runs a match on s against the regex 'this' was created with -- returns the number of matches found
	int         			match(const std::string & s, unsigned offset = 0);

	/// splits s based on delimiters matching the regex.
	int         			split(const std::string & s, ///< string to split on
								  unsigned maxfields = 0 ///< maximum number of fields to be split out.  0 means split all fields, but discard any trailing empty bits.  Negative means split all fields and keep trailing empty bits.  Positive means keep up to N fields including any empty fields less than N.  Anything remaining is in the last field.
);

	/// substitutes out whatever matches the regex for the second paramter
	std::string             sub ( const std::string & s, 
								  const std::string & r,
								  int dodollarsubstitution = 1 );

	/// study the regular expression to make it faster
	void                    study();

	/// returns the substring from the internal m_marks vector requires having run match or split first
	std::string             operator[](int);

	/// resets the regex object -- mostly useful for global matching
	void                    reset();

	/// returns the number of back references returned by the last match/sub call
	int                     NumBackRefs ( ) { return nMatches; }

	/// whether this regex is valid
	int IsValid ( ) { return nValid; }

	/// returns a vector of strings for the last match/split/sub
	StringVector GetStringVector ( );

	/// returns the  end of last match when m_isglobal != 0   

	int GetLastGlobalPosition() {return lastglobalposition;}


protected:

	/// used internally for operator[]
	/** \deprecated going away */
	std::string		       	substr(const std::string & s,
								const std::vector<markers> & marks, unsigned index);


	pcre * re; ///< pcre structure from pcre_compile

	unsigned _opts; ///< bit-flag options for pcre_compile

	pcre_extra * extra;	///< results from pcre_study

	int nMatches; ///< number of matches returned from last pcre_exec call

	std::vector<markers> m_marks; ///< last set of indexes of matches

	std::string laststringmatched; ///< copy of the last string matched
	void * addressoflaststring; ///< used for checking for change of string in global match

	int m_isglobal; ///< non-pcre flag for 'g' behaviour
	int lastglobalposition; ///< end of last match when m_isglobal != 0
	
	/// compiles the regex -- automatically called on construction
	void compile(const std::string & s);

	/// used to make a copy of a regex object
	static pcre * clone_re(pcre * re);

	/// takes perl-style character modifiers and determines the corresponding PCRE flags
	unsigned int DeterminePcreOptions ( const std::string & opts = "" );
	
	/// deals with $1-type constructs in the replacement string in a substitution
	std::string UpdateReplacementString ( const std::string & r );
	

	/// flag as to whether this regex is valid (compiled without error)
	int                     nValid;

};

/**  \mainpage  PME (PCRE Made Easy) - C++ PCRE Wrapper Library 

  \htmlonly
<a href="http://developer.berlios.de">
<img src="http://developer.berlios.de/bslogo.php?group_id=0" width="124" height="32" border="0" alt="BerliOS Logo" /></a>
\endhtmlonly

     \section sec_intr Introduction.
 
  PME is a C++ wrapper around the PCRE library. It has been designed to emulate Perl regular expression in both syntax and behaviour. PME requires PCRE version 4.3+ be installed.

The developement of PME was halted in 2004 (source: http://xaxxon.slackworks.com/).

I decided to maintain PME, so I put it 
on berlios (the author of PME was not reachable).

- News: 1.0.5: first new revision based on Berlios, with enhanced documentation.
- News: 1.0.3 fixes a few bugs. It is VERY highly recommended you upgrade to it. 
    
 
 
\section sec_feat Features:

    - Perl-like regex modifiers (imsxUg)
    - "Global" style matches where each match resumes from the previous one's end
    - Easy backreference access
    - Substitutions with backreference access in the replacement string
    - Splits are all similar to their Perl counterparts. 

PME has been optimized for simplicity and ease of use.

It performs technically unneccessary string copies to facilitate simple backreference and split field access.



\section sec_status Current Status

The current version of PME is <tt> 1.0.5</tt>. Same as <tt> 1.0.4</tt> but with enhanced documentation
and a new member method PME::GetLastGlobalPosition()     

The API should remain constant.


\section sec_doc Documentation/HOWTO   
- \subpage page_readme @PME documentation (readme)   


\section sec_downlowd Downloada

\subsection svn_repos SVN repository
  - trunk:      https://svn.berlios.de/svnroot/repos/pme/trunk  


\subsection tarball Tarballs

  
  - <a href="http://pre.adrien.mageos.com">PME 1.0.5</a> - updated documentation - first revision in Berlios
  - <a href="http://xaxxon.slackworks.com/pme/pme-1.0.4.tar.gz">PME 1.0.4</a> - visual studio 6 support
  - <a href="http://xaxxon.slackworks.com/pme/pme-1.0.3.tar.gz">PME 1.0.3</a>
  - <a href="http://xaxxon.slackworks.com/pme/pme-0.9.1.tar.gz">PME 0.9.1</a>


*/


/**

 \page page_readme Readme

 \section sec_read_desc Description
 PME is a C++ wrapper around the PCRE library written by Philip Hazel (ph10@cam.ac.uk). 

 It greatly reduces the syntactic complexity of PCRE and tries to very closely simulate the behaviour of perl.

 Both substitution <tt>(s/foo/bar/)</tt> and split (<tt>split /\s+\, $string, -1)</tt> are implemented, along with global matching.

 \section sec_read_license License Information

 PME is distributed under the same licensing as PCRE. 
 The general idea is that you either have to distribute it under the GPL, LGPL,
 or give proper credit to the use and source of this software package. 

 See the LICENSE file provided with PCRE for a full description of the licencing terms.

 \section  sec_read_copyright Copyright Notice
\verbatim
PME written by:
(C) Zachary Hansen
xaxxon@slackworks.com
maintained by Adrien Pré 
wanabi@free.fr
\endverbatim

\section sec_read_basic Basic API Instructions
The basic API into PME is very straight forward.  To create a PME object:
\code
PME MyPME ( "([a-z]+)", "" );
\endcode

To set modifiers on the regex, such as case insensitivity, put the following characters into the second string.

These characters correspond to the modifier characters following a regex in Perl.

  - \c i    Case insensitivity.  (\c PCRE_CASELESS)
  - \c m    Multiline.  <tt>[^$]</tt> match begin and end of line not just end of string (\c PCRE_MULTILINE)
  - \c s    Dot all.  Period matches any character including newline (\c PCRE_DOTALL)
  - \c x    Extended regex.  Ignores whitespace in the regex that isn't backslashed or in a character class.  Makes \c # a comment character just like it is in perl (\c PCRE_EXTENDED)
  - \c U    Ungreedy.  Default behaviour for \c * and \c + becomes ungreedy and \c ? postfix changes them individually to greedy (\c PCRE_UNGREEDY)
  - \c g    Global.  PME performs matches from the end of the previous match each time it is run (No PCRE equivalent)


The following declaration creates a PME object with global matching, case-insensitivity, line-based anchor characters, and periods matching all characters including newlines.  
\code  
PME MyPME ( "([a-z]+), "gims" );
std::string MyString = "abc def";
\endcode

Once a PME object has been created, it may be used.  The most basic operation that can be done with a PME is a match.
\code  
MyPME.match ( MyString );
\endcode


This match finds one backreference, <tt>"abc"</tt>.  To access a backreference, use array subscript notation. 

The number inside the brackets corresponds to the perl backreference number.  Subscript 0 returns the entire matched string.

The following prints out <tt>"abc\n"</tt> to standard out.
\code  
std::cout << MyPME [ 1 ] << std::endl;
\endcode


Because the <tt>"g"</tt> flag was used in the creation of MyRegex, if you call the <tt>match()</tt> method again, it
will continue at the point the previous match left off.  The following will print <tt>"def\n"</tt>.
\code
MyRegex.match ( MyString );
std::cout << MyRegex [ 1 ] << std::endl;
\endcode   
Substitution is another operation that can be done on a PME object.

The following code doubles all letters, both capital and lower-case.  Without the \c 'g' modifier,
it would only substitute the first letter.  Without the 'i' modifier, it would only substitute lower-case letter. 
After running this code, MySubString contains <tt>aaBBcc123</tt>. 
Note that you can use backreferences inside the replacement string.
\code  
Regex MySubRegex ( "[a-z]", "gi" );
std::string MySubString ( "aBc123" );
MySubRegex.sub ( MySubString, "$1$1" );
\endcode
Splitting is the third main operation that can be done with a PME object.  

The following code will print out each of the following on seperate lines:\c "abc", \c "def", \c "ghi".

\code
PME MySplitRegex ( "|" );
std::string MySplitString ( "abc|def|ghi" );

int NumSplits = MySplitRegex.split ( MySplitString );
for ( int i = 1; i <= NumSplits; i++ ) {
    std::cout << MySplitRegex [ i ] << std::endl;
}
\endcode

<tt>PME::GetStringVector</tt> -- returns a StringVector (typedef std::vector < std::string > StringVector) of all the strings captured either from a match, sub, or split.  This is useful if you want to run the regex on multiple strings and then use the results later.


\section full_doc Full Documentation:

Full documentation will go here eventually, but 90% of the functionality is described previously in the "Basic API Documentation" section.

\section contact_info Contact Information:

\verbatim


actual maintainer: Adrien Pré email: wanabi@free.fr

original Author contact info:
    email: xaxxon@slackworks.com                                               
    web: PCRE outdated website http://xaxxon.slackworks.com/pme/index.html	 
\endverbatim



*/ 





#endif // PME_H
