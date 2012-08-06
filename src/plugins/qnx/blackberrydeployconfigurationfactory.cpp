/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (C) 2011 - 2012 Research In Motion
**
** Contact: Research In Motion (blackberry-qt@qnx.com)
** Contact: KDAB (info@kdab.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#include "blackberrydeployconfigurationfactory.h"

#include "qnxconstants.h"
#include "blackberrydeployconfiguration.h"
#include "blackberrycreatepackagestep.h"
#include "blackberrydeploystep.h"
#include "blackberrydeviceconfigurationfactory.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/profileinformation.h>
#include <projectexplorer/target.h>
#include <qt4projectmanager/qt4project.h>
#include <remotelinux/genericdirectuploadstep.h>

using namespace Qnx;
using namespace Qnx::Internal;

BlackBerryDeployConfigurationFactory::BlackBerryDeployConfigurationFactory(QObject *parent) :
    ProjectExplorer::DeployConfigurationFactory(parent)
{
}

BlackBerryDeployConfigurationFactory::~BlackBerryDeployConfigurationFactory()
{
}

QList<Core::Id> BlackBerryDeployConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent) const
{
    QList<Core::Id> result;

    Qt4ProjectManager::Qt4Project *project = qobject_cast<Qt4ProjectManager::Qt4Project *>(parent->project());
    if (!project)
        return result;

    Core::Id deviceType = ProjectExplorer::DeviceTypeProfileInformation::deviceTypeId(parent->profile());
    if (deviceType != BlackBerryDeviceConfigurationFactory::deviceType())
        return result;

    result << Core::Id(Constants::QNX_BB_DEPLOYCONFIGURATION_ID);
    return result;
}

QString BlackBerryDeployConfigurationFactory::displayNameForId(const Core::Id id) const
{
    if (id == Constants::QNX_BB_DEPLOYCONFIGURATION_ID)
        return tr("Deploy to BlackBerry Device");

    return QString();
}

bool BlackBerryDeployConfigurationFactory::canCreate(ProjectExplorer::Target *parent,
                                              const Core::Id id) const
{
    return availableCreationIds(parent).contains(id);
}

ProjectExplorer::DeployConfiguration *BlackBerryDeployConfigurationFactory::create(
        ProjectExplorer::Target *parent, const Core::Id id)
{
    if (!canCreate(parent, id))
        return 0;

    BlackBerryDeployConfiguration *dc = new BlackBerryDeployConfiguration(parent);
    dc->stepList()->insertStep(0, new BlackBerryCreatePackageStep(dc->stepList()));
    dc->stepList()->insertStep(1, new BlackBerryDeployStep(dc->stepList()));
    return dc;
}

bool BlackBerryDeployConfigurationFactory::canRestore(ProjectExplorer::Target *parent,
                                               const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::DeployConfiguration *BlackBerryDeployConfigurationFactory::restore(
        ProjectExplorer::Target *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;

    BlackBerryDeployConfiguration *dc = new BlackBerryDeployConfiguration(parent);
    if (dc->fromMap(map))
        return dc;

    delete dc;
    return 0;
}

bool BlackBerryDeployConfigurationFactory::canClone(ProjectExplorer::Target *parent,
                                             ProjectExplorer::DeployConfiguration *source) const
{
    return canCreate(parent, source->id());
}

ProjectExplorer::DeployConfiguration *BlackBerryDeployConfigurationFactory::clone(
        ProjectExplorer::Target *parent, ProjectExplorer::DeployConfiguration *source)
{
    if (!canClone(parent, source))
        return 0;
    BlackBerryDeployConfiguration *old = static_cast<BlackBerryDeployConfiguration *>(source);
    return new BlackBerryDeployConfiguration(parent, old);
}
