/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2016 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include "RequiredPlugin.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/PluginManager.h>
#include <sofa/helper/logging/Messaging.h>

using sofa::helper::system::PluginManager;

namespace sofa
{

namespace component
{

namespace misc
{

SOFA_DECL_CLASS(RequiredPlugin)

int RequiredPluginClass = core::RegisterObject("Load required plugin")
        .add< RequiredPlugin >();

RequiredPlugin::RequiredPlugin()
    : pluginName( initData(&pluginName, "pluginName", "plugin name (or several names if you need to load different plugins or a plugin with several alternate names)"))
    , suffixMap ( initData(&suffixMap , "suffixMap", "standard->custom suffixes pairs (to be used if the plugin is compiled outside of Sofa with a non standard way of differenciating versions), using ! to represent empty suffix"))
    , stopAfterFirstNameFound( initData(&stopAfterFirstNameFound , false, "stopAfterFirstNameFound", "Stop after the first plugin name that is loaded successfully"))
    , stopAfterFirstSuffixFound( initData(&stopAfterFirstSuffixFound , true, "stopAfterFirstSuffixFound", "For each plugin name, stop after the first suffix that is loaded successfully"))
    , requireOne ( initData(&requireOne , true, "requireOne", "Display an error message if no plugin names were successfully loaded"))
    , requireAll ( initData(&requireAll , false, "requireAll", "Display an error message if any plugin names failed to be loaded"))
{
    this->f_printLog.setValue(true); // print log by default, to identify which pluging is responsible in case of a crash during loading
}

void RequiredPlugin::parse(sofa::core::objectmodel::BaseObjectDescription* arg)
{
    Inherit1::parse(arg);
    if (!pluginName.getValue().empty())
        loadPlugin();
}

void RequiredPlugin::loadPlugin()
{
    sofa::helper::system::PluginManager* pluginManager = &sofa::helper::system::PluginManager::getInstance();
    std::string defaultSuffix = pluginManager->getDefaultSuffix();
    const helper::vector<helper::fixed_array<std::string,2> >& sMap = suffixMap.getValue();
    helper::vector<std::string> suffixVec;
    if (!sMap.empty())
    {
        std::string skey = (defaultSuffix.empty() ? std::string("!") : defaultSuffix);
        for (std::size_t i = 0; i < sMap.size(); ++i)
        {
            if (sMap[i][0] == skey)
            {
                suffixVec.push_back(sMap[i][1] == std::string("!") ? std::string(""):sMap[i][1]);
            }
        }
    }
    if (suffixVec.empty())
        suffixVec.push_back(defaultSuffix);
    const helper::vector<std::string>& nameVec = pluginName.getValue();
    helper::vector< std::string > loaded;
    helper::vector< std::string > failed;
    std::ostringstream errmsg;
    for (std::size_t nameIndex = 0; nameIndex < nameVec.size(); ++nameIndex)
    {
        const std::string& name = nameVec[nameIndex];
        std::cout << "Loading " << name << std::endl;
        bool nameLoaded = false;
        for (std::size_t suffixIndex = 0; suffixIndex < suffixVec.size(); ++suffixIndex)
        {
            const std::string& suffix = suffixVec[suffixIndex];
            std::string pluginPath = name;
            bool result = pluginManager->findPlugin(pluginPath, suffix, false).empty();
            std::cout << "\tPlugin path" << name << std::endl;
            if (!result && !pluginManager->pluginIsLoaded(pluginPath))
            {
                result = pluginManager->loadPlugin(pluginPath, &errmsg, suffix);
            }
            if (result)
            {
                sout << "Loaded " << pluginPath << sendl;
                loaded.push_back(pluginPath);
                nameLoaded = true;
                if (stopAfterFirstSuffixFound.getValue()) break;
            }
        }
        if (!nameLoaded)
        {
            failed.push_back(name);
        }
        else
        {
            if (stopAfterFirstNameFound.getValue()) break;
        }
    }
    if (!failed.empty())
    {
        if ((requireAll.getValue() || (requireOne.getValue() && loaded.empty())))
        {
            serr << errmsg.str();
            serr << "Required plugin"<<(failed.size()>1?"s":"")<<" failed to load: " << failed << sendl;
        }
        else
        {
            sout << errmsg.str();
            sout << "Optional/alternate plugin"<<(failed.size()>1?"s":"")<<" failed to load: " << failed << sendl;
        }
    }
    pluginManager->init();

}

}

}

}
