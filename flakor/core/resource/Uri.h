#ifndef _FK_URI_H_
#define _FK_URI_H_

/**
 * copyright Saint Hsu
 *local file system：
 *asset://flakor.png for android assets
 *local:///sdcard/images/flakor.png
 *bundle://flakor.png for ios bundle
 *file:///sdcard/images/flakor.png same as local
 *internet：
 *http://flakor.org/img/image.png
 *https://flakor.org/img/image.png
 */
#include "base/lang/Str.h"

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
    public:
        static const char* DEFAULT_ENCODING;
		
        int type;
		String* origin;
        String* realPath;
        String* filename;

        //both decoded
        String* scheme;
        String* host;
        int port;
        String* path;
        //TODO const char* authority;
        String* fragment;
        String* query;
    public:

        enum {
           ASSET=1,
           BUNDLE,
           LOCAL,
           INTERNET,
           OTHER
        };

        Uri();
        virtual ~Uri();

        static Uri* parse(String* uriString);
        static Uri* parse(const std::string&& uriString);
        static Uri* parse(const char* uriString);

		bool isFrom(char* source);
		bool equal(Uri* other);

        /**
     	* Returns true if this URI is relative, i.e.&nbsp;if it doesn't contain an
     	* explicit scheme.
     	*
     	* @return true if this URI is relative, false if it's absolute
     	*/
        bool isRelative() const;

    	/**
     	* Returns true if this URI is absolute, i.e.&nbsp;if it contains an
     	* explicit scheme.
     	*
     	* @return true if this URI is absolute, false if it's relative
     	*/
        bool isAbsolute() const;

		/**
     	* Gets the scheme of this URI. Example: "http"
     	*
     	* @return the scheme or null if this is a relative URI
     	*/
        String* getScheme() const;

    	/**
     	 * Gets the scheme-specific part of this URI, i.e.&nbsp;everything between
     	 * the scheme separator ':' and the fragment separator '#'. If this is a
     	 * relative URI, this method returns the entire URI. Decodes escaped octets.
     	 *
     	 * <p>Example: "//www.google.com/search?q=android"
     	 *
     	 * @return the decoded scheme-specific-part
     	 */
         String* getSchemeSpecificPart() const;

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
         virtual String* getEncodedSchemeSpecificPart() const;

    	/**
     	* Gets the decoded authority part of this URI. For
     	* server addresses, the authority is structured as follows:
     	* {@code [ userinfo '@' ] host [ ':' port ]}
     	*
     	* <p>Examples: "google.com", "bob@google.com:80"
     	*
     	* @return the authority for this URI or null if not present
     	*/
        virtual String* getAuthority() const;

        /**
        * Gets the encoded authority part of this URI. For
        * server addresses, the authority is structured as follows:
        * {@code [ userinfo '@' ] host [ ':' port ]}
        *
        * <p>Examples: "google.com", "bob@google.com:80"
        *
        * @return the authority for this URI or null if not present
        */
        virtual String* getEncodedAuthority() const;


        /**
        * Gets the encoded host from the authority for this URI. For example,
        * if the authority is "bob@google.com", this method will return
        * "google.com".
        *
        * @return the host for this URI or null if not present
        */
        virtual String* getHost() const;

        /**
        * Gets the port from the authority for this URI. For example,
        * if the authority is "google.com:80", this method will return 80.
        *
        * @return the port for this URI or -1 if invalid or not present
        */
        virtual int getPort() const;

        /**
        * Gets the decoded path.
        *
        * @return the decoded path, or null if this is not a hierarchical URI
        * (like "mailto:nobody@google.com") or the URI is invalid
        */
        virtual String* getPath() const;

        /**
        * Gets the encoded path.
        *
        * @return the encoded path, or null if this is not a hierarchical URI
        * (like "mailto:nobody@google.com") or the URI is invalid
        */
        virtual String* getEncodedPath() const;

        /**
        * Gets the decoded query component from this URI. The query comes after
        * the query separator ('?') and before the fragment separator ('#'). This
        * method would return "q=android" for
        * "http://www.google.com/search?q=android".
        *
        * @return the decoded query or null if there isn't one
        */
        virtual String* getQuery() const;

        /**
        * Gets the encoded query component from this URI. The query comes after
        * the query separator ('?') and before the fragment separator ('#'). This
        * method would return "q=android" for
        * "http://www.google.com/search?q=android".
        *
        * @return the encoded query or null if there isn't one
        */
        virtual String* getEncodedQuery() const;

        /**
        * Gets the decoded fragment part of this URI, everything after the '#'.
        *
        * @return the decoded fragment or null if there isn't one
        */
        virtual String* getFragment() const;

        /**
        * Gets the encoded fragment part of this URI, everything after the '#'.
        *
        * @return the encoded fragment or null if there isn't one
        */
        virtual String* getEncodedFragment() const;
};

FLAKOR_NS_END

#endif
