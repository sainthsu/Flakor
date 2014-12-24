#ifndef _FK_URI_H_
#define _FK_URI_H_

/**
 * copyright Saint Hsu
 *local file system：
 *asset://flakor.png
 *local:///sdcard/images/flakor.png
 *file:///sdcard/images/flakor.png same as local
 *internet：
 *http://flakor.org/img/image.png
 *https://flakor.org/img/image.png
 */
#include "base/lang/FKString.h"

/** HierarchicalUri implementation for resource
 For reference, from RFC 2396:

    "4.3. Parsing a URI Reference

       A URI reference is typically parsed according to the four main
       components and fragment identifier in order to determine what
       components are present and whether the reference is relative or
       absolute.  The individual components are then parsed for their
       subparts and, if not opaque, to verify their validity.

       Although the BNF defines what is allowed in each component, it is
       ambiguous in terms of differentiating between an authority component
       and a path component that begins with two slash characters.  The
       greedy algorithm is used for disambiguation: the left-most matching
       rule soaks up as much of the URI reference string as it is capable of
       matching.  In other words, the authority component wins."

    The "four main components" of a hierarchical URI consist of
    <scheme>://<authority><path>?<query>
*/

FLAKOR_NS_BEGIN

class Uri
{
    protected:
		static char* DEFAULT_ENCODING = "UTF-8";
		
		int type;		
		const char* realPath;
		const char* filename;

		//both decoded
		const char* scheme;
		const char* host;
		const char* port;
		const char* path;
		//TODO const char* authority;
		const char* fregment;
		const char* query;
    public:
		enum {
                ASSET,
                LOCAL,
                INTERNET
        };

		static Uri* parse(String uriString);
                static Uri* parse(std::string uriString);
		static Uri* parse(char* uriString);

        /**
     	* Returns true if this URI is relative, i.e.&nbsp;if it doesn't contain an
     	* explicit scheme.
     	*
     	* @return true if this URI is relative, false if it's absolute
     	*/
    	bool isRelative();

    	/**
     	* Returns true if this URI is absolute, i.e.&nbsp;if it contains an
     	* explicit scheme.
     	*
     	* @return true if this URI is absolute, false if it's relative
     	*/
    	bool isAbsolute();

		/**
     	* Gets the scheme of this URI. Example: "http"
     	*
     	* @return the scheme or null if this is a relative URI
     	*/
    	String getScheme();

    	/**
     	 * Gets the scheme-specific part of this URI, i.e.&nbsp;everything between
     	 * the scheme separator ':' and the fragment separator '#'. If this is a
     	 * relative URI, this method returns the entire URI. Decodes escaped octets.
     	 *
     	 * <p>Example: "//www.google.com/search?q=android"
     	 *
     	 * @return the decoded scheme-specific-part
     	 */
         String getSchemeSpecificPart();

    	/**
     	 * Gets the scheme-specific part of this URI, i.e.&nbsp;everything between
         * the scheme separator ':' and the fragment separator '#'. If this is a
         * relative URI, this method returns the entire URI. Leaves escaped octets
         * intact.
         *
         * <p>Example: "//www.google.com/search?q=android"
         *
     	 * @return the decoded scheme-specific-part
     	 */
    	 virtual String getEncodedSchemeSpecificPart();

    /**
     * Gets the decoded authority part of this URI. For
     * server addresses, the authority is structured as follows:
     * {@code [ userinfo '@' ] host [ ':' port ]}
     *
     * <p>Examples: "google.com", "bob@google.com:80"
     *
     * @return the authority for this URI or null if not present
     */
     virtual String getAuthority();

    /**
     * Gets the encoded authority part of this URI. For
     * server addresses, the authority is structured as follows:
     * {@code [ userinfo '@' ] host [ ':' port ]}
     *
     * <p>Examples: "google.com", "bob@google.com:80"
     *
     * @return the authority for this URI or null if not present
     */
     virtual String getEncodedAuthority();

	
	    /**
     * Gets the encoded host from the authority for this URI. For example,
     * if the authority is "bob@google.com", this method will return
     * "google.com".
     *
     * @return the host for this URI or null if not present
     */
     virtual String getHost();

    /**
     * Gets the port from the authority for this URI. For example,
     * if the authority is "google.com:80", this method will return 80.
     *
     * @return the port for this URI or -1 if invalid or not present
     */
     virtual int getPort();

    /**
     * Gets the decoded path.
     *
     * @return the decoded path, or null if this is not a hierarchical URI
     * (like "mailto:nobody@google.com") or the URI is invalid
     */
     virtual String getPath();

    /**
     * Gets the encoded path.
     *
     * @return the encoded path, or null if this is not a hierarchical URI
     * (like "mailto:nobody@google.com") or the URI is invalid
     */
    virtual String getEncodedPath();

    /**
     * Gets the decoded query component from this URI. The query comes after
     * the query separator ('?') and before the fragment separator ('#'). This
     * method would return "q=android" for
     * "http://www.google.com/search?q=android".
     *
     * @return the decoded query or null if there isn't one
     */
     virtual String getQuery();

    /**
     * Gets the encoded query component from this URI. The query comes after
     * the query separator ('?') and before the fragment separator ('#'). This
     * method would return "q=android" for
     * "http://www.google.com/search?q=android".
     *
     * @return the encoded query or null if there isn't one
     */
     virtual String getEncodedQuery();

    /**
     * Gets the decoded fragment part of this URI, everything after the '#'.
     *
     * @return the decoded fragment or null if there isn't one
     */
     virtual String getFragment();

    /**
     * Gets the encoded fragment part of this URI, everything after the '#'.
     *
     * @return the encoded fragment or null if there isn't one
     */
     virtual String getEncodedFragment();
}

FLAKOR_NS_END

#endif
