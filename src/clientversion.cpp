// Copyright (c) 2012-2017 The Bitcoin Core developers
// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "clientversion.h"

#include "tinyformat.h"


/**
 * Name of client reported in the 'version' message. Report the same name
 * for both bczd and bcz-qt, to make it harder for attackers to
 * target servers or GUI users specifically.
 */
const std::string CLIENT_NAME("BCZ Core");

/**
 * Client version number
 */
#define CLIENT_VERSION_SUFFIX ""

//! git will put "#define GIT_ARCHIVE 1" on the next line inside archives.
#define GIT_ARCHIVE 1
#ifdef GIT_ARCHIVE
#define GIT_COMMIT_ID "$Format:%H$"
#define GIT_COMMIT_DATE "$Format:%cD$"
#endif

#define BUILD_DESC_WITH_SUFFIX(maj, min, rev, build, suffix) \
    "v" DO_STRINGIZE(maj) "." DO_STRINGIZE(min) "." DO_STRINGIZE(rev) "." DO_STRINGIZE(build) "-" DO_STRINGIZE(suffix)

#define BUILD_DESC_FROM_COMMIT(maj, min, rev, build, commit) \
    "v" DO_STRINGIZE(maj) "." DO_STRINGIZE(min) "." DO_STRINGIZE(rev) "." DO_STRINGIZE(build) "-g" commit

#define BUILD_DESC_FROM_UNKNOWN(maj, min, rev, build) \
    "v" DO_STRINGIZE(maj) "." DO_STRINGIZE(min) "." DO_STRINGIZE(rev) "." DO_STRINGIZE(build) "-travis"

#ifndef BUILD_DESC
#ifdef BUILD_SUFFIX
#define BUILD_DESC BUILD_DESC_WITH_SUFFIX(CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION, CLIENT_VERSION_BUILD, BUILD_SUFFIX)
#elif defined(GIT_COMMIT_ID)
#define BUILD_DESC BUILD_DESC_FROM_COMMIT(CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION, CLIENT_VERSION_BUILD, GIT_COMMIT_ID)
#else
#define BUILD_DESC BUILD_DESC_FROM_UNKNOWN(CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION, CLIENT_VERSION_BUILD)
#endif
#endif

#ifndef BUILD_DATE
#ifdef GIT_COMMIT_DATE
#define BUILD_DATE GIT_COMMIT_DATE
#else
#define BUILD_DATE __DATE__ ", " __TIME__
#endif
#endif

const std::string CLIENT_BUILD(BUILD_DESC CLIENT_VERSION_SUFFIX);
const std::string CLIENT_DATE(BUILD_DATE);

static std::string FormatVersion(int nVersion)
{
    if (nVersion % 100 == 0)
        return strprintf("%d.%d.%d", nVersion / 1000000, (nVersion / 10000) % 100, (nVersion / 100) % 100);
    else
        return strprintf("%d.%d.%d.%d", nVersion / 1000000, (nVersion / 10000) % 100, (nVersion / 100) % 100, nVersion % 100);
}

std::string FormatFullVersion()
{
    return CLIENT_BUILD;
}

/**
 * Format the subversion field according to BIP 14 spec (https://github.com/bitcoin/bips/blob/master/bip-0014.mediawiki)
 */
std::string FormatSubVersion(const std::string& name, int nClientVersion, const std::vector<std::string>& comments)
{
    std::ostringstream ss;
    ss << "/";
    ss << name << ":" << FormatVersion(nClientVersion);
    if (!comments.empty()) {
        std::vector<std::string>::const_iterator it(comments.begin());
        ss << "(" << *it;
        for (++it; it != comments.end(); ++it)
            ss << "; " << *it;
        ss << ")";
    }
    ss << "/";
    return ss.str();
}
