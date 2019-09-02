//
// Created by rick on 2019/8/30.
//

#include "Profile.h"

Profile *Profile::m_instance = nullptr;

Profile::Profile(Private::Profile* impl,
                 Private::PathConverter* pathConverter):m_profileImpl(impl),
                 m_pathConverterImple(pathConverter){
    ensureDirectoryExists(SpecialFolder::Cache);
    ensureDirectoryExists(SpecialFolder::Config);
    ensureDirectoryExists(SpecialFolder::Data);
}

Profile::~Profile() = default;

void Profile::initialize(const QString& rootProfilePath,
                         const QString& configurationName,
                         bool convertPathsToProfileRelative) {
    std::unique_ptr<Private::Profile> profile(rootProfilePath.isEmpty()?
    static_cast<Private::Profile *>(new Private::DefaultProfile))
}